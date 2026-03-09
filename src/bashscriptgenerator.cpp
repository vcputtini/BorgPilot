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
#include "bashscriptgenerator.h"

#include <QSettings>
#include <qstringliteral.h>

#include <sstream>

#include "bashscripttemplates.h"
#include "progid.h"
#include "settingshandler.h"

/*!
 * \brief BashScriptGenerator::BashScriptGenerator
 * \param fileName_
 * \param parent
 */
BashScriptGenerator::BashScriptGenerator(const QString& fileName_,
                                         QObject* parent)
  : QObject{ parent }
  , out_file_{ std::move(fileName_) }
  , fname_{ std::move(fileName_) }
{
  out_file_.setFileName(fname_);
  out_file_.setPermissions(QFileDevice::ReadUser);
  if (!out_file_.open(QIODevice::ReadWrite | QIODevice::Text)) {
    fileError(tr("Error trying to open file for writing/recording."));
    return;
  }
  out_ts_.setDevice(&out_file_);
}

/*!
 * \brief BashScriptGenerator::BashScriptGenerator
 * \param fileName_
 * \param data_create_m_
 * \param parent
 */
BashScriptGenerator::BashScriptGenerator(
  const QString& fileName_,
  AppTypes::RepoistoryCreateDataMap data_create_m_,
  QObject* parent)
  : QObject{ parent }
  , out_file_{ std::move(fileName_) }
  , data_map_create_{ data_create_m_ }
  , fname_{ std::move(fileName_) }
{
  out_file_.setFileName(fname_);
  out_file_.setPermissions(QFileDevice::ReadUser);
  if (!out_file_.open(QIODevice::ReadWrite | QIODevice::Text)) {
    fileError(tr("Error trying to open file for writing/recording."));
    return;
  }
  out_ts_.setDevice(&out_file_);
}

/*!
 * \public
 * \brief BashScriptGenerator::setRepositoryPath
 * \param path_
 */
void
BashScriptGenerator::setRepositoryPath(const QString& path_) noexcept
{
  repoPath_ = std::move(path_);
}

/*!
 * \public
 * \brief BashScriptGenerator::append
 * \param index_
 * \param data_
 * \return
 */
BashScriptGenerator&
BashScriptGenerator::append(int index_, AppTypes::InitRepositoryTuple data_)
{
  data_map_.insert(std::move(index_), std::move(data_));
  return *this;
}

/*!
 * \public
 * \brief BashScriptGenerator::setCommons
 * \param value_  => Common Options
 */
void
BashScriptGenerator::setCommons(const QString& value_)
{
  commons_ = std::move(value_);
}

/*!
 * \public
 * \brief BashScriptGenerator::writeScript()
 * \note Creates the common base structure for the repository initialization
 * scripts and for running the backup.
 */
void
BashScriptGenerator::writeScript() noexcept
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());
  SettingsHandler sHandler_;

  BashScriptTemplates* bst_ = new BashScriptTemplates();

  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_HEADER);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_VARS);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_CHECK_PRIVILEGES);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_ENSURE_DIRS);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_CHECK_COMMANDS);

  AppTypes::NamedValueList vars_export_list_ = sHandler_.readDirAndFiles();
  QString exported_vars_ = QString(
    bst_->getScriptTemplate(BashScriptTemplates::ScriptTpl::TPL_BASH_BORG_ENV)
      .arg(vars_export_list_.at(0).second,
           vars_export_list_.at(1).second,
           vars_export_list_.at(2).second,
           vars_export_list_.at(3).second,
           vars_export_list_.at(4).second,
           vars_export_list_.at(5).second,
           vars_export_list_.at(6).second));
  out_ts_ << exported_vars_;

  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_LOG);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_PID_TREATMENT);
  out_ts_ << bst_->getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_CLEANUP);

  if (data_map_create_.isEmpty()) {
    commandForInitialization();
  } else {
    commandForBackup();
  }

  switch (mainModel_) {
    case BashScriptGenerator::MainModel::Create: {
      std::string res_ = std::string(BashScriptTemplates::bash_main_def0) +
                         std::string(BashScriptTemplates::bash_main_def2);
      out_ts_ << res_.c_str();
      break;
    }
    case BashScriptGenerator::MainModel::Initialization: {
      std::string res_ = std::string(BashScriptTemplates::bash_main_def1) +
                         std::string(BashScriptTemplates::bash_main_def2);
      out_ts_ << res_.c_str();
      break;
    }
    case BashScriptGenerator::MainModel::None:
      break;
  }

  out_file_.flush();
  out_file_.close();
}

/*!
 * \brief BashScriptGenerator::setMainModel
 * \param model_
 */
void
BashScriptGenerator::setMainModel(MainModel model_)
{
  mainModel_ = std::move(model_);
}

/*!
 * \private
 * \brief BashScriptGenerator::commandForInitialization
 *
 * \note e.g.: borg --info --iec --show-rc  init -e none /dados/backup_teste/B01
 */
void
BashScriptGenerator::commandForInitialization() noexcept
{
  std::stringstream temp_ss_;

  BashScriptTemplates bst_;
  out_ts_ << bst_.getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_BORG_INIT_REPO);

  using repoFields = AppTypes::InitRepositoryFields;
  for (auto a_ = data_map_.constBegin(); a_ != data_map_.constEnd(); ++a_) {
    const auto f0_ =
      std::move(std::get<0>(a_.value()).simplified()); // repo name
    if (f0_.isEmpty()) {
      continue;
    }
    temp_ss_ << "borg ";

    QString encmode_{};
    switch (std::move(std::get<2>(a_.value()))) {
      case 0: {
        encmode_ = std::move(
          QString::fromStdString(command_.get(Command::EncryptModes::None)));
        break;
      }
      case 1: {
        encmode_ = std::move(
          QString::fromStdString(command_.get(Command::EncryptModes::Keyfile)));
        break;
      }
      case 2: {
        encmode_ = std::move(QString::fromStdString(
          command_.get(Command::EncryptModes::Repokey_blake2)));
        break;
      }
    }

    const auto f3_ = std::move(std::get<3>(a_.value())); // append-only
    const auto f4_ = std::move(std::get<4>(a_.value())); // make dir path
    const auto f5_ =
      std::move(std::get<5>(a_.value()).simplified()); // quota; e.g.: 1T, 5G

    temp_ss_ << " " << commons_.toStdString() << " "
             << command_.get(Command::Command_EC::Init) << " -e "
             << encmode_.toStdString();

    if (f3_) {
      temp_ss_ << " " << command_.get(Command::InitArguments::Append_only);
    }
    if (f4_) {
      temp_ss_ << " " << command_.get(Command::InitArguments::Make_parents_dir);
      ;
    }
    if (f5_ != "0") {
      temp_ss_ << QString(" %0 %1")
                    .arg(command_.get(Command::InitArguments::Storage_quata))
                    .arg(f4_)
                    .toStdString();
    }

    temp_ss_ << QString(" %0/%1\n").arg(repoPath_, f0_).toStdString();
    out_ts_ << temp_ss_.str().c_str();
    temp_ss_.str(""); // clear contents
  } // a_
  out_ts_ << "__log_info \"Initialization completed successfully.\"";
  out_ts_ << "\n\n}\n\n";
}

/*!
 * \private
 * \brief BashScriptGenerator::commandForBackup()
 *
 * \note e.g.: borg --info --iec --show-rc create --stats --list
 * /dados/backup_teste/B01::{hostname}-{user}-{now:%Y-%m-%d} /dados/teste
 */
void
BashScriptGenerator::commandForBackup() noexcept
{
  std::stringstream temp_ss_;

  BashScriptTemplates bst_;
  out_ts_ << bst_.getScriptTemplate(
    BashScriptTemplates::ScriptTpl::TPL_BASH_BORG_CREATE);

  for (auto a_ = data_map_create_.constBegin();
       a_ != data_map_create_.constEnd();
       ++a_) {
    auto key_ = a_.key(); // repo name
    if (key_.isEmpty()) {
      continue;
    }

    for (const auto& i_ : std::as_const(a_->first)) { // commons
      temp_ss_ << " " << i_.toStdString();
    }

    temp_ss_ << " create "; // borgbackup reserved word

    for (const auto& i_ : std::as_const(a_->second)) { // Optionals
      if (i_.contains("#")) {
        const auto parts_ = i_.split(u'#');
        for (const auto& p_ : parts_) {
          temp_ss_ << " " << p_.toStdString();
        }
        continue;
      }
      temp_ss_ << " " << i_.toStdString();
    }

    if (auto tmp_ = key_.split('#').at(0); tmp_.contains("@")) {
      temp_ss_ << " " << "ssh://" + key_.split('#').at(0).toStdString() << " "
               << key_.split('#').at(1).toStdString();
    } else {
      temp_ss_ << " " << key_.split('#').at(0).toStdString() << " "
               << key_.split('#').at(1).toStdString();
    }

    out_ts_ << "    borg " << temp_ss_.str().c_str() << '\n';
    temp_ss_.str(""); // clear contents

  } // a_
  out_ts_ << "\n    __log_info \"Backup completed successfully.\"";
  out_ts_ << "\n\n}\n\n";
}
