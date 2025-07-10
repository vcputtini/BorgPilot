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
#ifndef ENVIRONMENTVARS_H
#define ENVIRONMENTVARS_H

#include <initializer_list>

#include <QHash>
#include <QString>

/*!
 * \brief The EnvironmentVars class
 */
class EnvironmentVars
{
public:
  explicit EnvironmentVars();

  enum class EnvVars_DF
  {
    BORG_BASE_DIR = 0x00,
    BORG_CACHE_DIR,
    BORG_CONFIG_DIR,
    BORG_SECURITY_DIR,
    BORG_KEYS_DIR,
    BORG_KEY_FILE,
    TMPDIR
  };

  static constexpr std::initializer_list<EnvVars_DF> DF_List_ = {
    EnvVars_DF::BORG_BASE_DIR,   EnvVars_DF::BORG_CACHE_DIR,
    EnvVars_DF::BORG_CONFIG_DIR, EnvVars_DF::BORG_SECURITY_DIR,
    EnvVars_DF::BORG_KEYS_DIR,   EnvVars_DF::BORG_KEY_FILE,
    EnvVars_DF::TMPDIR
  };

  static constexpr const char* borg_base_dir_ = "BORG_BASE_DIR";
  static constexpr const char* borg_cache_dir_ = "BORG_CACHE_DIR";
  static constexpr const char* borg_config_dir_ = "BORG_CONFIG_DIR";
  static constexpr const char* borg_security_dir_ = "BORG_SECURITY_DIR";
  static constexpr const char* borg_keys_dir_ = "BORG_KEYS_DIR";
  static constexpr const char* borg_key_file_ = "BORG_KEY_FILE";
  static constexpr const char* borg_tmpdir_ = "TMPDIR";

  /* ---------------------------------------------------------------------- */
  enum class EnvVars_GBL
  {
    BORG_REPO,
    BORG_PASSPHRASE,
    BORG_PASSCOMMAND,
    BORG_PASSPHRASE_FD,
    BORG_NEW_PASSPHRASE,
    BORG_DISPLAY_PASSPHRASE,
    BORG_HOST_ID,
    BORG_LOGGING_CONF,
    BORG_RSH,
    BORG_REMOTE_PATH,
    BORG_FILES_CACHE_SUFFIX,
    BORG_FILES_CACHE_TTL,
    BORG_SHOW_SYSINFO,
    BORG_FUSE_IMPL,
    BORG_SELFTEST,
    BORG_WORKAROUNDS
  };

  static constexpr std::initializer_list<EnvVars_GBL> GBL_List_ = {
    EnvVars_GBL::BORG_REPO,
    EnvVars_GBL::BORG_PASSPHRASE,
    EnvVars_GBL::BORG_PASSCOMMAND,
    EnvVars_GBL::BORG_PASSPHRASE_FD,
    EnvVars_GBL::BORG_NEW_PASSPHRASE,
    EnvVars_GBL::BORG_DISPLAY_PASSPHRASE,
    EnvVars_GBL::BORG_HOST_ID,
    EnvVars_GBL::BORG_LOGGING_CONF,
    EnvVars_GBL::BORG_RSH,
    EnvVars_GBL::BORG_REMOTE_PATH,
    EnvVars_GBL::BORG_FILES_CACHE_SUFFIX,
    EnvVars_GBL::BORG_FILES_CACHE_TTL,
    EnvVars_GBL::BORG_SHOW_SYSINFO,
    EnvVars_GBL::BORG_FUSE_IMPL,
    EnvVars_GBL::BORG_SELFTEST,
    EnvVars_GBL::BORG_WORKAROUNDS
  };

  static constexpr const char* borg_repo_ = "BORG_REPO";
  static constexpr const char* borg_passphrase_ = "BORG_PASSPHRASE";
  static constexpr const char* borg_passcommand_ = "BORG_PASSCOMMAND";
  static constexpr const char* borg_passphrase_fd_ = "BORG_PASSPHRASE_FD";
  static constexpr const char* borg_new_passphrase_ = "BORG_NEW_PASSPHRASE";
  static constexpr const char* borg_display_passphrase_ =
    "BORG_DISPLAY_PASSPHRASE";
  static constexpr const char* borg_host_id_ = "BORG_HOST_ID";
  static constexpr const char* borg_logging_conf_ = "BORG_LOGGING_CONF";
  static constexpr const char* borg_rsh_ = "BORG_RSH";
  static constexpr const char* borg_remote_path_ = "BORG_REMOTE_PATH";
  static constexpr const char* borg_files_cache_suffix_ =
    "BORG_FILES_CACHE_SUFFIX";
  static constexpr const char* borg_files_cache_ttl_ = "BORG_FILES_CACHE_TTL";
  static constexpr const char* borg_show_sysinfo_ = "BORG_SHOW_SYSINFO";
  static constexpr const char* borg_fuse_impl_ = "BORG_FUSE_IMPL";
  static constexpr const char* borg_selftest_ = "BORG_SELFTEST";
  static constexpr const char* borg_workarounds_ = "BORG_WORKAROUNDS";

  /*!
   * \brief  A list of comma separated strings that trigger workarounds in borg,
   * e.g. to work around bugs in other software.
   * from:
   * https://borgbackup.readthedocs.io/en/stable/usage/general.html#environment-variables
   */
  enum class BorgWorkarounds
  {
    Basesyncfile,
    Retry_erofs,
    Authenticated_no_key,
    Ignore_invalid_archive_tam
  };

  static constexpr std::initializer_list<BorgWorkarounds> WArounds_List_ = {
    BorgWorkarounds::Basesyncfile,
    BorgWorkarounds::Retry_erofs,
    BorgWorkarounds::Authenticated_no_key,
    BorgWorkarounds::Ignore_invalid_archive_tam
  };

  static constexpr const char* wa_basesyncfile = "basesyncfile";
  static constexpr const char* wa_retry_erofs = "retry_erofs";
  static constexpr const char* wa_authenticated_no_key = "authenticated_no_key";
  static constexpr const char* wa_ignore_invalid_archive_tam =
    "ignore_invalid_archive_tam";

  /* ---------------------------------------------------------------------- */

  /*!
   * \brief Some automatic “answerers” (if set, they automatically answer
   * confirmation questions):
   */
  enum class EnvVars_AutoAnswerers
  {
    BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK,
    BORG_RELOCATED_REPO_ACCESS_IS_OK,
    BORG_CHECK_I_KNOW_WHAT_I_AM_DOING,
    BORG_DELETE_I_KNOW_WHAT_I_AM_DOING
  };

  static constexpr std::initializer_list<EnvVars_AutoAnswerers>
    EnvVars_Auto_List_ = {
      EnvVars_AutoAnswerers::BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK,
      EnvVars_AutoAnswerers::BORG_RELOCATED_REPO_ACCESS_IS_OK,
      EnvVars_AutoAnswerers::BORG_CHECK_I_KNOW_WHAT_I_AM_DOING,
      EnvVars_AutoAnswerers::BORG_DELETE_I_KNOW_WHAT_I_AM_DOING
    };

  static constexpr const char* borg_unknown_unencrypted_repo_access_is_ok =
    "BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK";
  static constexpr const char* borg_relocated_repo_access_is_ok =
    "BORG_RELOCATED_REPO_ACCESS_IS_OK";
  static constexpr const char* borg_check_i_know_what_i_am_doing =
    "BORG_CHECK_I_KNOW_WHAT_I_AM_DOING";
  static constexpr const char* borg_delete_i_know_waht_i_am_doing =
    "BORG_DELETE_I_KNOW_WHAT_I_AM_DOING";

  enum EnvVars
  {
    GLOBAL = 0x00,
    DIR_AND_FILES
  };

  const QHash<EnvVars_DF, QString> EnvVars_DF_m_ = {
    { EnvVars_DF::BORG_BASE_DIR, borg_base_dir_ },
    { EnvVars_DF::BORG_CACHE_DIR, borg_cache_dir_ },
    { EnvVars_DF::BORG_CONFIG_DIR, borg_config_dir_ },
    { EnvVars_DF::BORG_SECURITY_DIR, borg_security_dir_ },
    { EnvVars_DF::BORG_KEYS_DIR, borg_keys_dir_ },
    { EnvVars_DF::BORG_KEY_FILE, borg_key_file_ },
    { EnvVars_DF::TMPDIR, borg_tmpdir_ }
  };

  const QHash<EnvVars_GBL, QString> EnvVars_GBL_m_ = {
    { EnvVars_GBL::BORG_REPO, borg_repo_ },
    { EnvVars_GBL::BORG_PASSPHRASE, borg_passphrase_ },
    { EnvVars_GBL::BORG_PASSCOMMAND, borg_passcommand_ },
    { EnvVars_GBL::BORG_PASSPHRASE_FD, borg_passphrase_fd_ },
    { EnvVars_GBL::BORG_NEW_PASSPHRASE, borg_new_passphrase_ },
    { EnvVars_GBL::BORG_DISPLAY_PASSPHRASE, borg_display_passphrase_ },
    { EnvVars_GBL::BORG_HOST_ID, borg_host_id_ },
    { EnvVars_GBL::BORG_LOGGING_CONF, borg_logging_conf_ },
    { EnvVars_GBL::BORG_RSH, borg_rsh_ },
    { EnvVars_GBL::BORG_REMOTE_PATH, borg_remote_path_ },
    { EnvVars_GBL::BORG_FILES_CACHE_SUFFIX, borg_files_cache_ttl_ },
    { EnvVars_GBL::BORG_FILES_CACHE_TTL, borg_files_cache_ttl_ },
    { EnvVars_GBL::BORG_SHOW_SYSINFO, borg_show_sysinfo_ },
    { EnvVars_GBL::BORG_FUSE_IMPL, borg_fuse_impl_ },
    { EnvVars_GBL::BORG_SELFTEST, borg_selftest_ },
    { EnvVars_GBL::BORG_WORKAROUNDS, borg_workarounds_ }
  };

  const QHash<BorgWorkarounds, QString> BorgWorkarounds_m_{
    { BorgWorkarounds::Basesyncfile, wa_basesyncfile },
    { BorgWorkarounds::Retry_erofs, wa_retry_erofs },
    { BorgWorkarounds::Authenticated_no_key, wa_authenticated_no_key },
    { BorgWorkarounds::Ignore_invalid_archive_tam,
      wa_ignore_invalid_archive_tam }
  };

  const QHash<EnvVars_AutoAnswerers, QString> AutoAnswerers_m_{
    { EnvVars_AutoAnswerers::BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK,
      borg_unknown_unencrypted_repo_access_is_ok },
    { EnvVars_AutoAnswerers::BORG_RELOCATED_REPO_ACCESS_IS_OK,
      borg_relocated_repo_access_is_ok },
    { EnvVars_AutoAnswerers::BORG_CHECK_I_KNOW_WHAT_I_AM_DOING,
      borg_check_i_know_what_i_am_doing },
    { EnvVars_AutoAnswerers::BORG_DELETE_I_KNOW_WHAT_I_AM_DOING,
      borg_delete_i_know_waht_i_am_doing }
  };
};

#endif // ENVIRONMENTVARS_H
