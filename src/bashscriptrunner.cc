/***************************************************************************
 * Copyright (c) 2026                                              *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "bashscriptrunner.h"

#include <QDebug>
#include <QThread>

BashWorker::BashWorker(QString script, QStringList args, QObject* parent)
  : QObject(parent)
  , m_script(std::move(script))
  , m_args(std::move(args))
  , m_process(this) // QProcess with parent = this ensures automatic cleaning.
{
  /*! \note The QProcess was configured to run inside the Worker thread.
   *  Do NOT use setProcessChannelMode here if you want to separate
   * stdout/stderr.
   */
}

void
BashWorker::process()
{
  // Ensures that this method is running on the correct QThread.
  Q_ASSERT(QThread::currentThread() == thread());

  // We connect the process outputs to the Worker signals.
  // We use Qt::DirectConnection because Worker and QProcess live on the same
  // thread.
  QObject::connect(
    &m_process,
    &QProcess::readyReadStandardOutput,
    this,
    [this]() {
      const QByteArray data = m_process.readAllStandardOutput();
      if (!data.isEmpty()) {
        // Trim to avoid repeated blank lines.
        emit sig_outputReceived(QString::fromLocal8Bit(data).trimmed());
      }
    },
    Qt::DirectConnection);

  QObject::connect(
    &m_process,
    &QProcess::readyReadStandardError,
    this,
    [this]() {
      const QByteArray data = m_process.readAllStandardError();
      if (!data.isEmpty()) {
        emit sig_errorReceived(QString::fromLocal8Bit(data).trimmed());
      }
    },
    Qt::DirectConnection);

  QObject::connect(
    &m_process,
    qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
    this,
    [this](int exitCode, QProcess::ExitStatus status) {
      emit sig_finished(exitCode, status);
    },
    Qt::DirectConnection);

  QStringList fullArgs = m_args;
  fullArgs.prepend(m_script);

  m_process.setProcessChannelMode(QProcess::MergedChannels);
  m_process.start(R"(bash)", fullArgs);

  if (!m_process.waitForStarted(5000)) {
    // We avoid blocking indefinitely; 5 seconds seems like a reasonable,
    // adjustable value.
    emit sig_errorReceived(
      QStringLiteral("Error: Could not start bash or script: %1")
        .arg(m_script));
    emit sig_finished(-1, QProcess::CrashExit);
    return;
  }
}

void
BashWorker::terminate()
{
  if (m_process.state() != QProcess::NotRunning) {
    m_process.kill();
    m_process.waitForFinished(3000);
  }
}

// ======================= BashScriptRunner =======================

BashScriptRunner::BashScriptRunner(QObject* parent)
  : QObject(parent)
{
}

BashScriptRunner::~BashScriptRunner()
{
  // No manual management here, as QThreads and Workers are
  // cleared via deleteLater() when the threads issue finished().
}

void
BashScriptRunner::runScript(const QString& scriptName, const QStringList& args)
{
  if (m_isRunning) {
    qWarning() << "BashScriptRunner::runScript Called while already running:"
               << scriptName;
    return;
  }

  auto* thread = new QThread();
  auto* worker = new BashWorker(scriptName, args);

  worker->moveToThread(thread);

  QObject::connect(thread, &QThread::started, worker, &BashWorker::process);

  QObject::connect(worker,
                   &BashWorker::sig_outputReceived,
                   this,
                   &BashScriptRunner::sig_bashOutput);

  QObject::connect(worker,
                   &BashWorker::sig_errorReceived,
                   this,
                   &BashScriptRunner::sig_bashError);

  QObject::connect(
    worker,
    &BashWorker::sig_finished,
    this,
    [this, thread, worker](int exitCode, QProcess::ExitStatus status) {
      emit sig_bashFinished(exitCode, status);
      setRunning(false);
      thread->quit();
    });

  QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  setRunning(true);
  thread->start();
}

void
BashScriptRunner::setRunning(bool running)
{
  if (m_isRunning == running)
    return;

  m_isRunning = running;
  emit runningChanged();
}
