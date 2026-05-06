/***************************************************************************
 * Copyright (c) 2025                                                *
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

#include "types_global.h"

#include "progid.h"
#include "scriptgen.h"
#include "scripttemplates.h"
#include "settingshandler.h"

/*!
 * \brief ScriptGen::ScriptGen
 * \param fileName_
 * \param parent
 */
ScriptGen::ScriptGen(const QString& fileName_, QObject* parent)
  : QObject{ parent }
  , out_file_{ std::move(fileName_) }
  , fname_{ std::move(fileName_) }
{
  out_file_.setFileName(fname_);
  out_file_.setPermissions(QFileDevice::ReadUser);
  if (!out_file_.open(QIODevice::ReadWrite | QIODevice::Text)) {
    fError(tr("Error trying to open file for writing/recording."));
    return;
  }
  out_ts_.setDevice(&out_file_);
}

/*!
 * \private
 * \brief ScriptGen::fError
 * \param text_
 */
void
ScriptGen::fError(const QString& text_)
{
  emit fileError(text_);
}

/*!
 * \brief ScriptGen::append
 * \param index_
 * \param data_
 * \return
 */
ScriptGen&
ScriptGen::append(int index_, AppTypes::InitRepositoryTuple data_)
{
  data_map_.insert(std::move(index_), std::move(data_));
  return *this;
}

/*!
 * \brief ScriptGen::setRepositoryPath
 * \param path_
 */
void
ScriptGen::setRepositoryPath(const QString& path_) noexcept
{
  repoPath_ = std::move(path_);
}

/*!
 * \brief ScriptGen::sg_fnc_globals
 */
void
ScriptGen::sg_fnc_globals() noexcept
{
  out_ts_ << QString("function %0 ()")
               .arg(ScriptTemplates::NF_Pattern_)
               .arg("GLOBALS")
          << '\n';
  out_ts_ << "{\n";
  out_ts_ << "}\n";
}

/*!
 * \brief ScriptGen::sg_fnc_exports
 */
void
ScriptGen::sg_fnc_exports() noexcept
{
  out_ts_ << QString("function %0 ()")
               .arg(ScriptTemplates::NF_Pattern_)
               .arg("EXPORTS")
          << '\n';
  out_ts_ << "{\n";
  out_ts_ << "}\n";
}

/*!
 * \brief ScriptGen::sg_fnc_log
 */
void
ScriptGen::sg_fnc_log() noexcept
{
  out_ts_
    << QString("function %0 ()").arg(ScriptTemplates::NF_Pattern_).arg("LOG")
    << '\n';
  out_ts_ << "{\n";
  out_ts_ << "}\n";
}

/*!
 * \brief ScriptGen::sg_fnc_constrols
 */
void
ScriptGen::sg_fnc_constrols() noexcept
{
  out_ts_ << QString("function %0 ()")
               .arg(ScriptTemplates::NF_Pattern_)
               .arg("CONTROLS")
          << '\n';
  out_ts_ << "{\n";
  out_ts_ << "}\n";
}

/*!
 * \brief ScriptGen::sg_fnc_modules
 */
void
ScriptGen::sg_fnc_modules() noexcept
{
  out_ts_ << QString("function %0 ()")
               .arg(ScriptTemplates::NF_Pattern_)
               .arg("MODULES")
          << '\n';
  out_ts_ << "{\n";
  out_ts_ << "}\n";
}

/*!
 * \brief ScriptGen::sg_skeleton
 */
void
ScriptGen::sg_skeleton() noexcept
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());
  SettingsHandler sHandler_;

  ScriptTemplates* tpl_ = new ScriptTemplates();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_INIT_SCRIPT);
  out_ts_ << tpl_->scrTemplate();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_COPYRIGHT);
  out_ts_ << tpl_->scrTemplate();

  // variables
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_TODAY);
  out_ts_ << tpl_->scrTemplate();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_PID_FILE_);
  out_ts_ << tpl_->scrTemplate();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_LOG_PATH_);
  out_ts_ << tpl_->scrTemplate();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_LOG_FILE_);
  out_ts_ << tpl_->scrTemplate();

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_VARS_EXPORT);

  AppTypes::NamedValueList vars_export_list_ = sHandler_.readDirAndFiles();
  QString vars_export_ = QString(tpl_->scrTemplate())
                           .arg(vars_export_list_.at(0).second,
                                vars_export_list_.at(1).second,
                                vars_export_list_.at(2).second,
                                vars_export_list_.at(3).second,
                                vars_export_list_.at(4).second,
                                vars_export_list_.at(5).second,
                                vars_export_list_.at(6).second);
  out_ts_ << vars_export_;

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_EXTERNAL_COMMANDS_VALIDATIONS);
  out_ts_ << tpl_->scrTemplate();

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_FUNC_LOGGER);
  out_ts_ << tpl_->scrTemplate();

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_CHEK_PID);
  out_ts_ << tpl_->scrTemplate();

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_PRUNE_PID_FILE_);
  out_ts_ << tpl_->scrTemplate();
}

/*!
 * \brief ScriptGen::sg_fnc_init_repos
 * \param commons_
 */
void
ScriptGen::sg_fnc_init_repos(const QString commons_) noexcept
{
  sg_skeleton();

  ScriptTemplates* tpl_ = new ScriptTemplates();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_FUNC_HEADER);

  out_ts_ << ScriptTemplates::ENDL_ << ScriptTemplates::ScriptTemplates::ENDL_
          << QString(tpl_->scrTemplate()).arg("INIT") << ScriptTemplates::ENDL_
          << "{" << ScriptTemplates::ENDL_;

  out_ts_ << "\t{";

  out_ts_ << ScriptTemplates::ENDL_
          << "\t\t__logging \"Repository Initialization\" \"\""
          << ScriptTemplates::ENDL_;

  using repoFields = AppTypes::InitRepositoryFields;
  for (auto a_ = data_map_.constBegin(); a_ != data_map_.constEnd(); ++a_) {
    auto f0_ = std::move(std::get<0>(a_.value()).simplified()); // repo name
    if (f0_.isEmpty()) {
      continue;
    }

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

    auto f3_ = std::move(std::get<3>(a_.value())); // append-only
    auto f4_ = std::move(std::get<4>(a_.value())); // make dir path
    auto f5_ =
      std::move(std::get<5>(a_.value()).simplified()); // quota; e.g.: 1T, 5G

    out_ts_ << '\n'
            << '\t' << '\t' << Command::BORG_ << ScriptTemplates::SPC_
            << commons_ << ScriptTemplates::SPC_
            << QString::fromStdString(command_.get(Command::Command_EC::Init))
            << " -e " << encmode_;

    if (f3_) {
      out_ts_ << ScriptTemplates::SPC_
              << QString::fromStdString(
                   command_.get(Command::InitArguments::Append_only));
    }
    if (f4_) {
      out_ts_ << ScriptTemplates::SPC_
              << QString::fromStdString(
                   command_.get(Command::InitArguments::Make_parents_dir));
      ;
    }
    if (f5_ != "0") {
      out_ts_ << QString(" %0 %1")
                   .arg(QString::fromStdString(
                     command_.get(Command::InitArguments::Storage_quata)))
                   .arg(f4_);
    }

    out_ts_ << QString(" %0/%1").arg(repoPath_, f0_) << " 2>> $__log_file"
            << ScriptTemplates::ENDL_;
    tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_EXEC_STATUS_TEST);
    out_ts_ << tpl_->scrTemplate();
  }

  out_ts_ << '\t' << "} >> $__log_file" << ScriptTemplates::ENDL_;

  out_ts_ << "}" << ScriptTemplates::ENDL_ << ScriptTemplates::ENDL_;

  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_CALL_FUNC);
  out_ts_ << QString(tpl_->scrTemplate()).arg("INIT").remove("(").remove(")")
          << ScriptTemplates::ENDL_ << "__prune_pid_file"
          << ScriptTemplates::ENDL_ << ScriptTemplates::ENDL_
          << ScriptTemplates::ENDL_ << "exit" << ScriptTemplates::ENDL_
          << ScriptTemplates::EOF_;

  out_file_.flush();
  out_file_.close();
}

/*!
 * \brief ScriptGen::sg_fnc_create
 * \param data_map_create_
 *
 * \note borg [common options] create [options] ARCHIVE [PATH...]
 */
void
ScriptGen::sg_fnc_create(
  AppTypes::RepoistoryCreateDataMap data_map_create_) noexcept
{
  sg_skeleton();

  ScriptTemplates* tpl_ = new ScriptTemplates();
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_FUNC_HEADER);

  out_ts_ << ScriptTemplates::ENDL_ << ScriptTemplates::ScriptTemplates::ENDL_
          << QString(tpl_->scrTemplate()).arg("CREATE")
          << ScriptTemplates::ENDL_ << "{" << ScriptTemplates::ENDL_;

  // qDebug() << __FUNCTION__ << "  " << data_map_create_;

  out_ts_ << "\t{";
  out_ts_ << ScriptTemplates::ENDL_ << "\t\t__logging \"Backup\" \"\""
          << ScriptTemplates::ENDL_;

  for (auto a_ = data_map_create_.constBegin();
       a_ != data_map_create_.constEnd();
       ++a_) {
    auto f0_ = a_.key(); // repo name
    if (f0_.isEmpty()) {
      continue;
    }

    out_ts_ << '\t' << '\t' << Command::BORG_;

    for (const auto& i_ : std::as_const(a_->first)) { // Commons
      out_ts_ << ScriptTemplates::SPC_ << i_.toUtf8();
    }

    out_ts_ << ScriptTemplates::SPC_
            << command_.get(Command::Command_EC::Create).c_str();

    for (const auto& i_ : std::as_const(a_->second)) { // Optionals
      if (i_.contains("#")) {
        const auto parts_ = i_.split(u'#');
        for (const auto& p_ : parts_) {
          out_ts_ << ScriptTemplates::SPC_ << p_;
        }
        continue;
      }
      out_ts_ << ScriptTemplates::SPC_ << i_;
    }

    if (auto tmp_ = f0_.split('#').at(0); tmp_.contains("@")) {
      out_ts_ << ScriptTemplates::SPC_ << "ssh://" + f0_.split('#').at(0) << " "
              << f0_.split('#').at(1);
    } else {
      out_ts_ << ScriptTemplates::SPC_ << f0_.split('#').at(0) << " "
              << f0_.split('#').at(1);
    }
    out_ts_ << " 2>> $__log_file" << ScriptTemplates::ENDL_;
    tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_EXEC_STATUS_TEST);
    out_ts_ << tpl_->scrTemplate();
  }
  out_ts_ << '\n' << '\t' << "} >> $__log_file" << ScriptTemplates::ENDL_;

  out_ts_ << ScriptTemplates::ENDL_ << "}" << ScriptTemplates::ENDL_
          << ScriptTemplates::ENDL_;
  tpl_->setToken(ScriptTemplates::ScriptTpl::TPL_CALL_FUNC);
  out_ts_ << QString(tpl_->scrTemplate()).arg("CREATE").remove("(").remove(")")
          << ScriptTemplates::ENDL_ << "__prune_pid_file"
          << ScriptTemplates::ENDL_ << ScriptTemplates::ENDL_ << "exit"
          << ScriptTemplates::ENDL_ << ScriptTemplates::EOF_;

  out_file_.flush();
  out_file_.close();
}
