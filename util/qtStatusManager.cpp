/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QtCore>

#include <QApplication>
#include <QHash>
#include <QLabel>
#include <QList>
#include <QProgressBar>

#include "../core/qtDebug.h"

#include "qtStatusManager.h"
#include "qtStatusSourcePrivate.h"

QTE_IMPLEMENT_D_FUNC(qtStatusManager)

//-----------------------------------------------------------------------------
class qtStatusManagerPrivate
{
public:
  struct StatusInfo
    {
    StatusInfo()
      : progressAvailable(false), progressMinimum(0), progressMaximum(0),
        progressValue(-1)
      {}

    QString text;
    bool progressAvailable;
    int progressMinimum;
    int progressMaximum;
    int progressValue;
    QString progressFormat;
    };

  qtStatusManagerPrivate(qtStatusManager* q)
    : q_ptr(q), debugArea(qtDebug::InvalidArea) {}

  void setLastSender(qtStatusSource&);
  void removeSendersByObject(const QObject*);
  void update();

protected:
  QTE_DECLARE_PUBLIC_PTR(qtStatusManager)

  qtDebugAreaAccessor debugArea;

  QList<QLabel*> labels;
  QList<QProgressBar*>progressBars;

  QList<qtStatusSource> senders;
  QHash<const QObject*, StatusInfo> status;

private:
  QTE_DECLARE_PUBLIC(qtStatusManager)
};

//-----------------------------------------------------------------------------
QDebug& operator<<(QDebug& dbg, qtStatusManagerPrivate::StatusInfo si)
{
  dbg.nospace();
  dbg << "( text = " << si.text
      << ", progressAvailable = " << si.progressAvailable
      << ", progressMinimum = " << si.progressMinimum
      << ", progressMaximum = " << si.progressMaximum
      << ", progressValue = " << si.progressValue
      << ", progressFormat = " << si.progressFormat
      << " )";
  return dbg.space();
}

//-----------------------------------------------------------------------------
void qtStatusManagerPrivate::setLastSender(qtStatusSource& source)
{
  QTE_Q(qtStatusManager);

  // Clear this sender's status when it goes away
  q->connect(source.d_ptr.data(), SIGNAL(ownerDestroyed(qtStatusSource)),
             q, SLOT(removeSource(qtStatusSource)), Qt::UniqueConnection);

  // Move sender to top of the list, if not already there
  this->senders.removeAll(source);
  this->senders.append(source);

  // Check if the sender was deleted while we were adding it
  if (source.isOwnerDestroyed())
    {
    q->removeSource(source);
    }
}

//-----------------------------------------------------------------------------
void qtStatusManagerPrivate::removeSendersByObject(const QObject* obj)
{
  QList<qtStatusSource>::iterator iter = this->senders.begin();
  while (iter != this->senders.end())
    {
    if (iter->owner() == obj)
      {
      iter = this->senders.erase(iter);
      }
    else
      {
      ++iter;
      }
    }
}

//-----------------------------------------------------------------------------
void qtStatusManagerPrivate::update()
{
  StatusInfo si;

  if (!this->senders.isEmpty())
    {
    si = this->status[this->senders.last().owner()];

    qtDebug(this->debugArea)
        << "updating status using sender" << this->senders.last()
        << "status" << si;
    }
  else
    {
    qtDebug(this->debugArea) << "clearing status";
    }

  for each (auto const label, this->labels)
    label->setText(si.text);

  for each (auto const progressBar, this->progressBars)
    {
    progressBar->setVisible(si.progressAvailable);
    if (si.progressAvailable)
      {
      if (si.progressValue < 0)
        {
        if (progressBar->minimum() != 0 || progressBar->maximum() != 0)
          {
          progressBar->setRange(0, 0);
          }
        }
      else
        {
        progressBar->setFormat(si.progressFormat);
        progressBar->setRange(si.progressMinimum, si.progressMaximum);
        progressBar->setValue(si.progressValue);
        }
      }
    }
}

//-----------------------------------------------------------------------------
qtStatusManager::qtStatusManager(QObject* parent)
  : QObject(parent), d_ptr(new qtStatusManagerPrivate(this))
{
  qRegisterMetaType<qtStatusSource>("qtStatusSource");
}

//-----------------------------------------------------------------------------
qtStatusManager::~qtStatusManager()
{
}

//-----------------------------------------------------------------------------
void qtStatusManager::setDebugArea(qtDebugAreaAccessor area)
{
  QTE_D(qtStatusManager);
  d->debugArea = area;
}

//-----------------------------------------------------------------------------
void qtStatusManager::addStatusLabel(QLabel* obj)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea) << "adding label" << obj;

  d->labels.append(obj);
  connect(obj, SIGNAL(destroyed(QObject*)),
          this, SLOT(removeObject(QObject*)));

  d->update();
}

//-----------------------------------------------------------------------------
void qtStatusManager::addProgressBar(QProgressBar* obj)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea) << "adding progress bar" << obj;

  d->progressBars.append(obj);
  connect(obj, SIGNAL(destroyed(QObject*)),
          this, SLOT(removeObject(QObject*)));

  d->update();
}

//-----------------------------------------------------------------------------
void qtStatusManager::removeStatusLabel(QLabel* obj)
{
  QTE_D(qtStatusManager);
  qtDebug(d->debugArea) << "removing label" << obj;
  d->labels.removeAll(obj);
}

//-----------------------------------------------------------------------------
void qtStatusManager::removeProgressBar(QProgressBar* obj)
{
  QTE_D(qtStatusManager);
  qtDebug(d->debugArea) << "removing progress bar" << obj;
  d->progressBars.removeAll(obj);
}

//-----------------------------------------------------------------------------
void qtStatusManager::removeObject(QObject* obj)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea) << "removing object" << obj;

  d->labels.removeAll(static_cast<QLabel*>(obj));
  d->progressBars.removeAll(static_cast<QProgressBar*>(obj));
}

//-----------------------------------------------------------------------------
void qtStatusManager::removeSource(qtStatusSource source)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea) << "removing source" << source;

  // Clear this object's status
  if (d->status.contains(source.owner()))
    {
    bool needUpdate = (d->senders.last() == source);
    d->status.remove(source.owner());
    d->senders.removeAll(source);
    if (needUpdate)
      {
      d->update();
      }
    }

}

//-----------------------------------------------------------------------------
void qtStatusManager::transferOwnership(const QObject* from,
                                        qtStatusSource to)
{
  QTE_D(qtStatusManager);

  if (d->status.contains(from))
    {
    qtDebug(d->debugArea)
        << "transferring status ownership from" << from << "to" << to;

    d->status.insert(to.owner(), d->status[from]);
    d->removeSendersByObject(from);
    d->setLastSender(to);
    }
}

//-----------------------------------------------------------------------------
void qtStatusManager::setStatusText(qtStatusSource source, QString text)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea) << "setting status text" << source << text;

  if (text.isEmpty())
    {
    // No text means we should clear this sender's status
    d->senders.removeAll(source);
    d->status.remove(source.owner());
    }
  else
    {
    qtStatusManagerPrivate::StatusInfo& si = d->status[source.owner()];
    si.text = text;
    d->setLastSender(source);
    }

  d->update();
}

//-----------------------------------------------------------------------------
void qtStatusManager::setProgress(qtStatusSource source, bool available,
                                  qreal value)
{
  this->setProgress(source, available, value * 10000.0, 0, 10000);
}

//-----------------------------------------------------------------------------
void qtStatusManager::setProgress(qtStatusSource source, bool available,
                                  int value, int steps, QString format)
{
  this->setProgress(source, available, value, 0, steps, format);
}

//-----------------------------------------------------------------------------
void qtStatusManager::setProgress(qtStatusSource source, bool available,
                                  int value, int minimum, int maximum,
                                  QString format)
{
  QTE_D(qtStatusManager);

  qtDebug(d->debugArea)
      << "setting progress" << source
      << "( available =" << available
      << "value =" << value << '/' << minimum << '-' << maximum
      << "format =" << format << ')';

  qtStatusManagerPrivate::StatusInfo& si = d->status[source.owner()];
  si.progressAvailable = available;
  si.progressMinimum = minimum;
  si.progressMaximum = maximum;
  si.progressValue = value;
  si.progressFormat = format;

  d->setLastSender(source);
  d->update();
}
