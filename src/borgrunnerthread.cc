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
#include "borgrunnerthread.h"
#include "QDebug"
#include <QProcessEnvironment>

#include "settingshandler.h"

/*!
 * \brief BorgThreadRunner::BorgThreadRunner
 * \param argsList
 * \param parent
 */
BorgThreadRunner::BorgThreadRunner(const QStringList& argsList, QObject* parent)
  : QThread(parent)
  , borgArgs_(argsList)
{
}

BorgThreadRunner::BorgThreadRunner(BorgCommands cmd_,
                                   const QStringList& argsList,
                                   QObject* parent)
  : QThread(parent)
  , borgArgs_(argsList)
  , command_(std::move(getCommand(cmd_)))
{
}

/*!
 * \brief BorgThreadRunner::~BorgThreadRunner
 */
BorgThreadRunner::~BorgThreadRunner()
{
  requestInterruption();
  wait();
}

QStringList
BorgThreadRunner::splitLineCommand(const QString& line_)
{
  return QProcess::splitCommand(line_);
}

/*!
 * \brief BorgThreadRunner::run
 */
void
BorgThreadRunner::run()
{
  QProcess process;

  AppTypes::NamedValueList list_ = getBorgEnv();

  QProcessEnvironment env_ = QProcessEnvironment::systemEnvironment();
  for (auto& vars_ : list_) {
    env_.insert(vars_.first, vars_.second);
  }

  process.setProcessEnvironment(env_);
  process.setProcessChannelMode(QProcess::MergedChannels); // stdout + stderr

  if (!command_.isEmpty()) {
    borgArgs_.prepend(command_);
    // qDebug() << "fun: " << __FUNCTION__ << " antes do start " << borgArgs_;
    process.start("borg", borgArgs_);
  }
  if (!process.waitForStarted()) {
    emit borgError("Error: Process could not be started.");
    emit borgError("QProcess::error: " + QString::number(process.error()));
    emit borgError("Details: " + process.errorString());
    emit borgFinished(-1, QProcess::CrashExit);
    return;
  }

  // Better visual results
  QByteArray buffer_{};
  QStringList lines_{};

  while (process.state() == QProcess::Running) {
    if (process.waitForReadyRead(100)) {
      buffer_.append(process.readAll());

      int index_{ 0 };
      while ((index_ = buffer_.indexOf('\n')) != -1) {
        QByteArray lineBytes_ = buffer_.left(index_ + 1); // include the '\n'
        buffer_.remove(0, index_ + 1);

        const QString line_ = QString::fromLocal8Bit(std::move(lineBytes_));
        lines_.append(std::move(line_)); // automatically includes the \n
      }
    }

    if (isInterruptionRequested()) {
      process.kill();
      break;
    }
  }

  if (!buffer_.isEmpty()) {
    QString line_ = QString::fromLocal8Bit(std::move(buffer_));
    lines_.append(std::move(line_));
  }

  emit borgOutput(lines_.join(""));
  emit borgOutputList(lines_);

  process.waitForFinished();
  emit borgFinished(process.exitCode(), process.exitStatus());
}

/*!
 * \brief BorgThreadRunner::getBorgEnv
 * \return
 */
AppTypes::NamedValueList
BorgThreadRunner::getBorgEnv()
{
  SettingsHandler shandler_;
  AppTypes::NamedValueList list_ = shandler_.readDirAndFiles();
  return list_;
}

/*!
 * \brief BorgThreadRunner::setCommand
 * \param cmd_
 */
void
BorgThreadRunner::setCommand(BorgCommands& cmd_)
{
  command_ = std::move(getCommand(cmd_));
}

/*!
 * \brief BorgThreadRunner::fromStrToCommand
 * \param text_
 * \return
 */
BorgThreadRunner::BorgCommands
BorgThreadRunner::fromStrToCommand(const QString& text_)
{
  for (auto it_ = BorgCommands_m_.cbegin(); it_ != BorgCommands_m_.cend();
       ++it_) {
    if (it_->second == text_) {
      return it_->first;
    }
  }
  // key not found
  return BorgCommands::None;
}

// --- Commands Threatment

/*!
 * \brief BorgThreadRunner::initializeCommandsMap
 * \return
 */
const std::map<BorgThreadRunner::BorgCommands, QString>
BorgThreadRunner::initializeCommandsMap()
{
  return { { BorgCommands::Extract, "extract" },
           { BorgCommands::Check, "check" },
           { BorgCommands::Rename, "rename" },
           { BorgCommands::List, "list" },
           { BorgCommands::Diff, "diff" },
           { BorgCommands::Delete, "delete" },
           { BorgCommands::Prune, "prune" },
           { BorgCommands::Compact, "compact" },
           { BorgCommands::Info, "info" },
           { BorgCommands::ReCreate, "recreate" },
           { BorgCommands::ImportTar, "import-tar" },
           { BorgCommands::ExportTar, "export-tar" },
           { BorgCommands::None, "none" } };
}

/*!
 * \brief BorgThreadRunner::BorgCommands_m_
 */
const std::map<BorgThreadRunner::BorgCommands, QString>
  BorgThreadRunner::BorgCommands_m_ = BorgThreadRunner::initializeCommandsMap();

/*!
 * \brief BorgThreadRunner::getCommand
 * \param cmd_
 * \return
 */
const QString&
BorgThreadRunner::getCommand(BorgThreadRunner::BorgCommands cmd_)
{
  static const QString unknown_ = "unknown";
  auto it_ = BorgCommands_m_.find(cmd_);
  return it_ != BorgCommands_m_.end() ? it_->second : unknown_;
}
