/***************************************************************************
 * Copyright (c) 2025                                                      *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@gmail.com> | <vcputtini@vcplinux.com.br>
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
#ifndef BORGRUNNERTHREAD_H
#define BORGRUNNERTHREAD_H

#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QTime>

#include <map>

#include "types_global.h"

/*!
 * \brief The BorgThreadRunner class
 */
class BorgThreadRunner : public QThread
{
  Q_OBJECT

public:
  enum class BorgCommands
  {
    Extract = 0x00,
    Check,
    Rename,
    List,
    Diff,
    Delete,
    Prune,
    Compact,
    Info,
    ReCreate,
    ImportTar,
    ExportTar,
    None
  };

  explicit BorgThreadRunner(const QStringList& argsList,
                            QObject* parent = nullptr);
  explicit BorgThreadRunner(BorgCommands cmd_,
                            const QStringList& argsList,
                            QObject* parent = nullptr);
  ~BorgThreadRunner() override;

  void setCommand(BorgCommands& cmd_);
  static BorgCommands fromStrToCommand(const QString& text_ = nullptr);
  static QStringList splitLineCommand(const QString& line_ = nullptr);

signals:
  void borgOutput(const QString& line);
  void borgOutputList(const QStringList& list);
  void borgError(const QString& errorLine);
  void borgFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
  void run() override;

private:
  BorgCommands borgCommand_;
  QString command_;
  QStringList borgArgs_;
  AppTypes::NamedValueList getBorgEnv();

  static const std::map<BorgCommands, QString> initializeCommandsMap();
  static const std::map<BorgCommands, QString> BorgCommands_m_;

  static const QString& getCommand(BorgCommands cmd_);
};

#endif // BORGRUNNERTHREAD_H
