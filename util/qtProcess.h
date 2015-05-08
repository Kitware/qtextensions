/*
Copyright 2015 Kitware, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Kitware, Inc. nor the names of any contributors may be used
   to endorse or promote products derived from this software without specific
   prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __qtProcess_h
#define __qtProcess_h

#include "../core/qtGlobal.h"

#include <QtCore/QProcess>

class qtProcessPrivate;

class QTE_EXPORT qtProcess : public QProcess
{
  Q_OBJECT

public:
  qtProcess(QObject* parent = nullptr);
  virtual ~qtProcess();

  /// Wait for the process to finish while still processing events.
  ///
  /// This method "joins" the process and does not return until the process is
  /// finished (no longer running). Application events will be processed while
  /// the process is executing using an internal QEventLoop.
  void join();

  /// Report process output using qDebug() and/or qWarning().
  ///
  /// This reports any and all output produces by the process. Anything that
  /// the process wrote to stdout is reported via qDebug(). Anything that the
  /// process wrote to stderr is reported via qWarning().
  void report();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtProcess)

private:
  QTE_DECLARE_PRIVATE(qtProcess)
  Q_DISABLE_COPY(qtProcess)
};

#endif
