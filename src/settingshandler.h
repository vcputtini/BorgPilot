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
#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QMap>
#include <QMultiHash>
#include <QObject>
#include <QPair>
#include <QSettings>
#include <QString>

#include "environmentvars.h"
#include "types_global.h"

/*!
 * \brief The SettingsHandler class
 */
class SettingsHandler
  : public QObject
  , public EnvironmentVars
{
  Q_OBJECT

public:
  explicit SettingsHandler(QObject* parent = nullptr);

  bool isScriptExists(const QString& name_);

  bool saveGeneral(AppTypes::NamedValueList list_);
  bool saveDirAndFiles(AppTypes::NamedValueList list_);
  bool saveAutoAnswerers(AppTypes::NamedValueList list_);

  AppTypes::NamedValueList readGeneral();
  AppTypes::NamedValueList readDirAndFiles();
  AppTypes::NamedValueList readAutoAnswerers();

  struct ScriptDefs
  {
    QString scrName_;
    int mode_;
    QString destRepo_;
    QString cmOptions_;
    AppTypes::InitRepositoryMap initDefs_;
  };
  bool saveInitScriptDefs(ScriptDefs defs_);
  AppTypes::NamedValueList readInitScriptDefs(const QString script_name_);

  bool testPermissions(QString& errorText_);

private:
  template<typename TList = AppTypes::NamedValueList,
           typename T2 = QMap<EnvVars_GBL, QString>>
  TList readSettings(const QString section_, T2 t_);

  template<typename TList = AppTypes::NamedValueList, typename TGroup = QString>
  bool saveSettings(TList list_, TGroup group_);

  // -------------------------------------------------------------------------
  static constexpr const char* grp_name_ = "INITREPO_%0";
  static constexpr const char* name_ = "A_NAME";

  //! \brief mode_  true == localrepo | false == remoterepo
  static constexpr const char* mode_ = "A_MODE";
  static constexpr const char* destrepo_ = "A_DESTREPO";
  static constexpr const char* cmoptions_ = "A_CMOPTIONS";

  static constexpr const char* reponame_ = "B_REPONAME_%0";
  static constexpr const char* archive_ = "B_ARCHIVE_%0";
  static constexpr const char* encmode_ = "B_ENCMODE_%0";
  static constexpr const char* append_ = "B_APPEND_%0";
  static constexpr const char* mkparentdir_ = "B_MKPARENTDIR_%0";
  static constexpr const char* stoquota_ = "B_STOQUOTA_%0";
};

// ===========================================================================

/*!
 * \brief The SettingsHandlerCreate class
 */
class SettingsHandlerCreate : public QObject
{
  Q_OBJECT
public:
  explicit SettingsHandlerCreate(QObject* parent = nullptr) {};

  bool save(const QString& scriptName,
            const QMultiHash<QString, QPair<QList<QString>, QList<QString>>>&
              scriptData);

  AppTypes::RepoistoryCreateDataMap read(const QString& scriptName);
};

#endif // SETTINGSHANDLER_H
