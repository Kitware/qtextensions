// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtStatusForwarder_h
#define __qtStatusForwarder_h

#include "qtStatusNotifier.h"

/// Helper class to redirect status notifications from one source to itself.
///
/// This class provides a base class for users that wish to both send their own
/// status notifications, and also pass on notifications from another source as
/// if the other source's notifications had come from themselves. This works by
/// connecting other status notifiers to the forwarder class and reemitting
/// their notifications with the original qtStatusSource replaces by the
/// qtStatusSource of the forwarder instance.
class QTE_EXPORT qtStatusForwarder : public qtStatusNotifier
{
  Q_OBJECT

public:
  qtStatusForwarder();
  virtual ~qtStatusForwarder();

  /// Connect another notifier to this forwarder.
  ///
  /// This connects the notifications of the specified notifier to this
  /// forwarder instance. The notifications of the specified notifier will be
  /// reemitted from this forwarder using its status source.
  void connect(qtStatusNotifier*);
  using QObject::connect;

protected slots:
  void forwardStatusMessage(qtStatusSource, QString);
  void forwardProgress(qtStatusSource, bool, qreal);
  void forwardProgress(qtStatusSource, bool, int, int);
};

#endif
