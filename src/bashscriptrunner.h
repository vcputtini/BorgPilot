/***************************************************************************
 * Copyright (c) 2025-2026                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@vcplinux.com.br>                                        *
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
#ifndef BASHSCRIPTRUNNER_H
#define BASHSCRIPTRUNNER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

/**
 * \brief Worker que executará o processo em um QThread.
 *
 * Nota de design:
 * - Mantemos o QProcess como membro da classe (não local),
 *garantindo o vitalício atrelado ao vitalício do Trabalhador.
 * - O Worker roda dentro do thread, usando
 * o loop de eventos do QThread para receber sinais do QProcess.
 */
class BashWorker : public QObject
{
  Q_OBJECT
public:
  explicit BashWorker(QString script,
                      QStringList args,
                      QObject* parent = nullptr);
  ~BashWorker() override = default;

public slots:
  /**
   * \brief Entry point for execution within the QThread.
   *
   * This slot will be called when the QThread issues started().
   * Do not block this function with manual loops; rely on the signals
   * from the QProcess and the thread's event loop.
   */
  void process();

  /**
   * \brief Force termination of the process, if it is still running.
   * Allows canceling execution from outside, if necessary.
   */
  void terminate();

signals:
  void sig_outputReceived(const QString& line);
  void sig_errorReceived(const QString& error);
  void sig_finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
  QString m_script;
  QStringList m_args;
  QProcess m_process; // QProcess maintained as a member, not as a raw pointer.
};

/**
 * \brief Controller to manage the Thread and interface with QML/Widgets.
 *
 * Responsibilities:
 * - Create/destroy QThread and BashWorker.
 * - Expose a simple API (runScript) for QML/C++ (future use).
 * - Reissue exit/error/termination signals to the UI.
 */
class BashScriptRunner : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isRunning READ isRunning NOTIFY runningChanged)

public:
  explicit BashScriptRunner(QObject* parent = nullptr);
  ~BashScriptRunner() override;

  Q_INVOKABLE void runScript(const QString& scriptName,
                             const QStringList& args = {});

  bool isRunning() const { return m_isRunning; }

signals:
  void sig_bashOutput(const QString& line);
  void sig_bashError(const QString& errorLine);
  void sig_bashFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void runningChanged();

private:
  void setRunning(bool running);

  bool m_isRunning = false;
};

#endif // BASHSCRIPTRUNNER_H
