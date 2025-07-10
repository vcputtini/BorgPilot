/***************************************************************************
 * Copyright (c) 2025                                                 *
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
#include "settingshandler.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>
#include <QMessageBox>
#include <QtAlgorithms>

#include "progid.h"
#include "types_global.h"

/*!
 * \brief SettingsHandler::SettingsHandler
 * \param parent
 */
SettingsHandler::SettingsHandler(QObject* parent)
  : QObject{ parent }
{
}

/*!
 * \brief SettingsHandler::isScriptExists
 * \param name_
 * \return
 */
bool
SettingsHandler::isScriptExists(const QString& name_)
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  if (auto rc_ = settings_.value(QString("INITREPO_%0").arg(name_), -1);
      rc_.toInt() != -1) {
    return true; // name repository exists
  }
  return false;
}

/*!
 * \brief SettingsHandler::testPermissions
 *
 * ConstantValueDescription
 * QFileDevice::ReadOwner0x4000The file is readable by the owner of the file.
 * QFileDevice::WriteOwner0x2000The file is writable by the owner of the file.
 * QFileDevice::ExeOwner0x1000The file is executable by the owner of the file.
 * QFileDevice::ReadUser0x0400The file is readable by the user.
 * QFileDevice::WriteUser0x0200The file is writable by the user.
 * QFileDevice::ExeUser0x0100The file is executable by the user.
 * QFileDevice::ReadGroup0x0040The file is readable by the group.
 * QFileDevice::WriteGroup0x0020The file is writable by the group.
 * QFileDevice::ExeGroup0x0010The file is executable by the group.
 * QFileDevice::ReadOther0x0004The file is readable by others.
 * QFileDevice::WriteOther0x0002The file is writable by others.
 * QFileDevice::ExeOther0x0001The file is executable by others.
 */
bool
SettingsHandler::testPermissions(QString& errorText_)
{
  const QString filePath_ = QString("%0/.config/%1/qBorgPilot.conf")
                              .arg(QDir::homePath(), ProgId::strOrganization());

  QFileInfo fileInfo(filePath_);
  if (!fileInfo.exists()) {
    errorText_ = tr("Configuration file does not exist: %0")
                   .arg(qUtf8Printable(filePath_));
    return false;
  }

  // File directory path
  QDir dir = fileInfo.dir();
  const QString homePath = QDir::homePath();

  // Recursively checking the path to $HOME
  while (!dir.isRoot()) {
    const QString dirPath = dir.absolutePath();
    QFileInfo dirInfo(dirPath);
    QFile::Permissions perms = dirInfo.permissions();

    if (!(perms & QFile::ExeUser)) {
      errorText_ = tr("No execute (access) permission on directory: %0")
                     .arg(qUtf8Printable(dirPath));
      return false;
    }

    if (!(perms & QFile::ReadUser)) {
      errorText_ = tr("No read (access) permission on directory: %0")
                     .arg(qUtf8Printable(dirPath));
      return false;
    }

    if (dirPath == fileInfo.dir().absolutePath() &&
        !(perms & QFile::WriteUser)) {
      errorText_ = tr("No write (access) permission on directory: %0")
                     .arg(qUtf8Printable(dirPath));
      return false;
    }

    // Stop if we reach $HOME
    if (dirPath == homePath) {
      break;
    }

    dir.cdUp();
  }

  // Tests the opening of the file (if it exists)
  if (fileInfo.exists()) {
    QFile file(filePath_);
    if (!file.open(QIODevice::ReadOnly)) {
      errorText_ =
        tr("No read permission on file: %0").arg(qUtf8Printable(filePath_));
      return false;
    }
    file.close();

    if (!file.open(QIODevice::ReadWrite)) {
      errorText_ =
        tr("No write permission on file: %0").arg(qUtf8Printable(filePath_));
      return false;
    }
    file.close();

    // Checks for insecure permissions (group write/others)
    QFile::Permissions filePerms = fileInfo.permissions();
    if (filePerms & (QFile::WriteGroup | QFile::WriteOther)) {
      errorText_ = tr("Group or others have write permission on the file: %0")
                     .arg(qUtf8Printable(filePath_));
      return false;
    }
  }

  return true;
}

/*!
 * \brief SettingsHandler::saveGeneral
 * \param list_
 * \return
 */
bool
SettingsHandler::saveGeneral(AppTypes::NamedValueList list_)
{
  return saveSettings<>(list_, "General");
}

/*!
 * \brief SettingsHandler::saveDirAndFiles
 * \param list_
 * \return
 */
bool
SettingsHandler::saveDirAndFiles(AppTypes::NamedValueList list_)
{
  return saveSettings<>(list_, "DirAndFiles");
}

/*!
 * \brief SettingsHandler::saveAutoAnswerers
 * \param list_
 * \return
 */
bool
SettingsHandler::saveAutoAnswerers(AppTypes::NamedValueList list_)
{
  return saveSettings<>(list_, "AutoAnswerers");
}

/*!
 * \brief SettingsHandler::readGeneral
 * \return
 */
AppTypes::NamedValueList
SettingsHandler::readGeneral()
{
  return readSettings<>("General", EnvVars_GBL_m_);
}

/*!
 * \brief SettingsHandler::readDirAndFiles
 * \return
 */
AppTypes::NamedValueList
SettingsHandler::readDirAndFiles()
{
  return readSettings<AppTypes::NamedValueList>("DirAndFiles", EnvVars_DF_m_);
}

/*!
 * \brief SettingsHandler::readAutoAnswerers
 * \return
 */
AppTypes::NamedValueList
SettingsHandler::readAutoAnswerers()
{
  return readSettings<>("AutoAnswerers", AutoAnswerers_m_);
}

/*!
 * \brief SettingsHandler::readSettings
 * \param section_
 * \param t_
 * \return
 */
template<typename TList, typename T2>
TList
SettingsHandler::readSettings(const QString section_, T2 t_)
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  TList list_{};
  const int size = settings_.beginReadArray(section_);
  for (int i = 0; i < size; ++i) {
    settings_.setArrayIndex(i);
    QString key = settings_.value("key").toString();
    QString value = settings_.value("value").toString();
    list_.append(qMakePair(key, value));
  }
  settings_.endArray();
  return list_;
}

/*!
 * \brief SettingsHandler::saveSettings
 * \param list_
 * \param group_
 * \return
 */
template<typename TList, typename TGroup>
bool
SettingsHandler::saveSettings(TList list_, TGroup group_)
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  if (settings_.isWritable()) {
    settings_.beginWriteArray(group_);
    for (int i = 0; i < list_.size(); ++i) {
      settings_.setArrayIndex(i);
      settings_.setValue("key", list_[i].first);
      settings_.setValue("value", list_[i].second);
    }
    settings_.endArray();
    return true;
  }

  return false;
}

/*!
 * \brief SettingsHandler::saveInitDefs
 * \param defs_
 * \return
 */
bool
SettingsHandler::saveInitScriptDefs(ScriptDefs defs_)
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());
  if (settings_.isWritable()) {
    settings_.beginGroup(
      QString(std::move(grp_name_)).arg(std::move(defs_.scrName_)));

    settings_.setValue(std::move(mode_), std::move(defs_.mode_));
    settings_.setValue(std::move(name_), std::move(defs_.scrName_));
    if (defs_.mode_ == 1) {
      settings_.setValue(std::move(destrepo_),
                         QString("%1").arg(defs_.destRepo_));
    } else {
      settings_.setValue(std::move(destrepo_), std::move(defs_.destRepo_));
    }
    settings_.setValue(std::move(cmoptions_), std::move(defs_.cmOptions_));

    for (int i_ = 0; i_ < AppTypes::MaxRepositoriesRows; ++i_) {
      auto a_ = std::move(defs_.initDefs_.value(i_));
      settings_.setValue(QString(reponame_).arg(i_), std::get<0>(a_));
      settings_.setValue(QString(archive_).arg(i_), std::get<1>(a_));
      settings_.setValue(QString(encmode_).arg(i_), std::get<2>(a_));
      settings_.setValue(QString(append_).arg(i_), std::get<3>(a_));
      settings_.setValue(QString(mkparentdir_).arg(i_), std::get<4>(a_));
      settings_.setValue(QString(stoquota_).arg(i_), std::get<5>(a_));
    }

    settings_.endGroup();
    return true;
  }
  return false;
}

/*!
 * \brief SettingsHandler::readInitScriptDefs
 * \param script_name_
 * \return
 */
AppTypes::NamedValueList
SettingsHandler::readInitScriptDefs(const QString script_name_)
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  const QString groupname_ = std::move(QString(grp_name_).arg(script_name_));

  AppTypes::NamedValueList list_{};
  const QList<QString> vars_{ reponame_, archive_,     encmode_,
                              append_,   mkparentdir_, stoquota_ };

  list_.append(
    qMakePair(name_, settings_.value(groupname_ + "/" + name_).toString()));
  list_.append(
    qMakePair(mode_, settings_.value(groupname_ + "/" + mode_).toString()));
  list_.append(
    qMakePair(name_, settings_.value(groupname_ + "/" + name_).toString()));
  list_.append(qMakePair(
    destrepo_, settings_.value(groupname_ + "/" + destrepo_).toString()));
  list_.append(qMakePair(
    cmoptions_, settings_.value(groupname_ + "/" + cmoptions_).toString()));

  for (int i_ = 0; i_ < AppTypes::MaxRepositoriesRows; ++i_) {
    for (const auto& a_ : vars_) {
      const QString id_ = std::move(groupname_ + "/" + QString(a_).arg(i_));
      const QString var_id_ = std::move(QString(a_).arg(i_));
      list_.append(qMakePair(var_id_, settings_.value(id_).toString()));
    }
  }
  return list_;
}

// ===========================================================================
/*!
 * \brief SettingsHandlerCreate::save
 * \param scriptName
 * \param scriptData
 * \return
 */
bool
SettingsHandlerCreate::save(
  const QString& scriptName,
  const QMultiHash<QString, QPair<QList<QString>, QList<QString>>>& scriptData)
{
  QSettings settings_(ProgId::strOrganization(),
                      ProgId::strInternalName() + "_" + scriptName);

  if (settings_.isWritable()) {
    settings_.beginGroup(scriptName);

    int cnt_{ 0 };
    for (auto it_ = scriptData.constBegin(); it_ != scriptData.constEnd();
         ++it_) {
      settings_.beginGroup(QString("REPO_%0").arg(cnt_));
      settings_.setValue("NAME", it_.key());
      settings_.setValue("COMMONS", it_.value().first.join(' '));
      settings_.setValue("OPTIONALS", it_.value().second.join(' '));
      settings_.endGroup(); // close repository group
      ++cnt_;
    }
    settings_.endGroup(); // Close the script group
    settings_.sync();
    return true;
  }
  return false;
}

/*!
 * \brief SettingsHandlerCreate::load
 * \param scriptName
 * \return
 */
AppTypes::RepoistoryCreateDataMap
SettingsHandlerCreate::read(const QString& scriptName)
{

  QSettings settings(ProgId::strOrganization(),
                     ProgId::strInternalName() + "_" + scriptName);

  AppTypes::RepoistoryCreateDataMap map_ret_{};
  QString repoName_{};
  QList<QString> commons_lst_{};
  QList<QString> optionals_lst_{};

  settings.beginGroup(scriptName);

  for (auto& group_ : settings.childGroups()) {
    settings.beginGroup(group_);
    for (auto& ckey_ : settings.childKeys()) {
      if (ckey_ == "NAME") {
        repoName_ = settings.value(ckey_).toString();
      } else if (ckey_ == "COMMONS") {
        QString value_ = settings.value(ckey_).toString();
        commons_lst_.append(value_.split(' '));
      } else if (ckey_ == "OPTIONALS") {
        QString value_ = settings.value(ckey_).toString();
        optionals_lst_.append(value_.split(' '));
      }
    }
    settings.endGroup();
    map_ret_.insert(repoName_, qMakePair(commons_lst_, optionals_lst_));
    commons_lst_.clear();
    optionals_lst_.clear();
  }

  return map_ret_;
}
