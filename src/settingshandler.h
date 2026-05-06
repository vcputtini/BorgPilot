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
#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QAnyStringView>
#include <QMap>
#include <QMultiHash>
#include <QObject>
#include <QPair>
#include <QSettings>
#include <QString>

#include "environmentvars.h"
#include "types_global.h"

/*!
 * \class SettingsHandler
 * \brief Manages application-wide configurations and BorgBackup script
 * definitions.
 * * This class handles the persistence of general settings, environment
 * variables, and repository initialization parameters using the underlying
 * QSettings framework.
 */
class SettingsHandler
  : public QObject
  , public EnvironmentVars
{
  Q_OBJECT

public:
  explicit SettingsHandler(QObject* parent = nullptr);

  /*!
   * \brief Verifies if a specific script configuration exists in the settings.
   * \param name_ The name of the script to check.
   * \return true if the script is found; otherwise false.
   */
  [[nodiscard]] bool isScriptExists(const QString& name_);

  /*! \brief Persists general application settings. */
  bool saveGeneral(AppTypes::NamedValueList list_);

  /*! \brief Persists directory and file inclusion/exclusion lists. */
  bool saveDirAndFiles(AppTypes::NamedValueList list_);

  /*! \brief Persists automated response configurations for Borg interactions.
   */
  bool saveAutoAnswerers(AppTypes::NamedValueList list_);

  /*!
   * \brief Data structure for local application preferences.
   */
  struct Preferences
  {
    QString localScriptPath; /*!< FS path where generated scripts are stored. */
    QString localLogPath;    /*!< FS path for application and backup logs. */
    QString localDataPath;   /*!< FS path for general data files */
  };

  /*! \brief Saves the user's local path preferences. */
  bool savePreferences(Preferences prefs_);

  /*! \brief Retrieves the current local path preferences. */
  Preferences relodPreferences();

  /*! \brief Reads the general application settings. */
  AppTypes::NamedValueList readGeneral();

  /*! \brief Reads the stored directory and file lists. */
  AppTypes::NamedValueList readDirAndFiles();

  /*! \brief Reads the stored automated response settings. */
  AppTypes::NamedValueList readAutoAnswerers();

  /*!
   * \brief Configuration set for initializing a new Borg repository.
   */
  struct ScriptDefs
  {
    QString scrName_;   /*!< The script identifier. */
    int mode_;          /*!< 1 for local repository, 0 for remote. */
    QString destRepo_;  /*!< Target repository path or URI. */
    QString cmOptions_; /*!< Additional command-line options for Borg. */
    AppTypes::InitRepositoryMap
      initDefs_; /*!< Initialization-specific flags and values. */
  };

  /*! \brief Saves the initialization definitions for a specific script. */
  bool saveInitScriptDefs(ScriptDefs defs_);

  /*!
   * \brief Reads the initialization definitions for a given script name.
   * \param script_name_ The target script identifier.
   */
  AppTypes::NamedValueList readInitScriptDefs(const QString script_name_);

  /*!
   * \brief Validates if the application has the required filesystem
   * permissions.
   * \param errorText_ Receives a descriptive message if the test fails.
   * \return true if permissions are valid.
   */
  bool testPermissions(QString& errorText_);

  /*!
   * \brief Remove um completamente uma definicao de repositorio
   * \param group_ Nome do grupo para remover
   * \note nao reversivel
   */
  void removeRepository(QAnyStringView group_);

private:
  /*!
   * \brief Internal template to retrieve settings from a specific section.
   */
  template<typename TList = AppTypes::NamedValueList,
           typename T2 = QMap<EnvVars_GBL, QString>>
  TList readSettings(const QString section_, T2 t_);

  /*!
   * \brief Internal template to persist settings to a specific group.
   */
  template<typename TList = AppTypes::NamedValueList, typename TGroup = QString>
  bool saveSettings(TList list_, TGroup group_);

  //! \brief --- Constants for Settings Keys ---
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

signals:
  /*! \brief Emitted to provide updates on current operations or errors. */
  /*!
   * \brief Emitted to provide updates on whether there were any errors while
   * trying to access the configuration file.
   * \param status A QString() type containing the error message to be issued.
   */
  void sig_status(const QString& status);
};

// ===========================================================================

/*!
 * \class SettingsHandlerCreate
 * \brief Specialized handler for persisting Borg repository creation metadata.
 * * This class focuses on the complex data structures required during the
 * initial creation and initialization phase of a Borg repository.
 */
class SettingsHandlerCreate : public QObject
{
  Q_OBJECT
public:
  explicit SettingsHandlerCreate(QObject* parent = nullptr) {};
  ~SettingsHandlerCreate() = default;

  /*!
   * \brief Persists repository creation parameters to the configuration store.
   * \param scriptName The unique identifier for the backup script.
   * \param scriptData A multi-hash mapping configuration keys to their specific
   * value pairs.
   * \return True if the data was written successfully.
   */
  [[nodiscard]] bool save(
    const QString& scriptName,
    const QMultiHash<QString, QPair<QList<QString>, QList<QString>>>&
      scriptData);

  /*!
   * \brief Retrieves the repository creation data for a specific script.
   * \param scriptName The identifier for the script to load.
   * \return A map containing the structured initialization data.
   */
  [[nodiscard]] AppTypes::RepoistoryCreateDataMap read(
    const QString& scriptName);
};

#endif // SETTINGSHANDLER_H
