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
#ifndef SCRIPTTEMPLATES_H
#define SCRIPTTEMPLATES_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <initializer_list>

class ScriptTemplates : public QObject
{
  Q_OBJECT
public:
  enum class ScriptTpl
  {
    TPL_INIT_SCRIPT,
    TPL_COPYRIGHT,
    TPL_TODAY,
    TPL_PID_FILE_,
    TPL_PRUNE_PID_FILE_,
    TPL_LOG_PATH_,
    TPL_LOG_FILE_,
    TPL_VARS_EXPORT,
    TPL_FUNC_LOGGER,
    TPL_CHEK_PID,
    TPL_FUNC_HEADER,
    TPL_CALL_FUNC,
    TPL_EXEC_STATUS_TEST,
    TPL_EXTERNAL_COMMANDS_VALIDATIONS,
    TPL_NONE
  };

  explicit ScriptTemplates(ScriptTpl tpl_ = ScriptTpl::TPL_NONE,
                           QObject* parent = nullptr);

  void setToken(ScriptTpl tpl_ = ScriptTpl::TPL_NONE);
  ScriptTpl token() const;
  QString scrTemplate() const;
  QString getLogFileTemplate() const;

  static constexpr const char* NF_Pattern_ = "__BORGPILOT_%0";
  static constexpr const char* separator_ =
    "# "
    "--------------------------------------------------------------------------"
    "----";
  static constexpr const char* EOF_ = "\n# EOF\n";
  static constexpr const char* SPC_ = " ";
  static constexpr const char ENDL_ = '\n';

private:
  ScriptTpl tmpTpl_;

  static constexpr const char* func_header_ = "function __BORGPILOT_%0 ()";
  static constexpr const char* call_func_ = "__BORGPILOT_%0 ()";

  /*!
   * \warning Do not change copyright notices. If you make any useful
   * changes, simply add your details. Thank you very much!
   */
  static constexpr const char* copyright_ = R"(
#
# ------------------------------------------------------------------------------
# Script generated by BorgPilot User Interface
# Copyright(C)'2025 by Volnei Cervi Puttini <vputtini@gmail.com>
# ------------------------------------------------------------------------------
#
)";

  static constexpr const char* today_ = "__today=`date \"+%Y-%m-%d\"`\n";
  static constexpr const char* pid_file_ =
    "__pid_file=\"/var/run/borgpilot.pid\"\n";
  static constexpr const char* log_path_ =
    "__log_path=\"/var/log/borgpilot\"\n";
  static constexpr const char* log_file_ =
    "__log_file=\"${__log_path}/${__today}.log\"\n";

  static constexpr const char* script_header_ = R"(#!/usr/bin/env bash
set -euEo pipefail
trap '"${BASH_SOURCE:-unknown}:${LINENO:-unknow}: $BASH_COMMAND";' ERR
# ==============================================================================

if [ \"$(whoami)\" != \"root\" ]; then
    echo \"This script needs to be run as root.\"
    exit 1
fi
)";

  static constexpr const char* external_commands_validations_ =
    R"(__required_cmds=("borg" "mkdir" "date" "hostname" "id" "whoami")
for cmd in "${__required_cmds[@]}"; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        echo "[ERROR] Required command not found: $cmd" >&2
        exit 1
    fi
done

)";

  static constexpr const char* vars_export_ = R"(
# Borg environment variables

__borg_base_dir="%0"
__borg_cache_dir="%1"
__borg_config_dir="%2"
__borg_keys_dir="%5"
__borg_key_file="%6"
__borg_security_dir="%4"
__tmpdir="%7"

export BORG_BASE_DIR=${__borg_base_dir}
export BORG_CACHE_DIR=${__borg_cache_dir}
export BORG_CONFIG_DIR=${__borg_config_dir}
export BORG_KEYS_DIR=${__borg_keys_dir}
export BORG_KEY_FILE=${__borg_key_file}
export BORG_SECURITY_DIR=${__borg_security_dir}
export TMPDIR=${__tmpdir}

for dir in "${__borg_config_dir}" "${__borg_security_dir}" "${__borg_keys_dir}" "${__borg_cache_dir}" "${__tmpdir}" "${__log_path}"; do
    if [[ ! -d "$dir" ]]; then
        mkdir -p "$dir" || { echo "[ERROR] Failed to create directory: $dir" >&2; exit 1; }
    fi
done

)";

  static constexpr const char* fnc_logger_ = R"(function __logging ()
{
  local __msg=$1
  local __parm=$2
  local __date=`date "+%Y-%m-%d %H:%M"`
  local __log_f=""
  if [[ "$__parm" == "" ]]; then
    __log_f=${__log_file}
  else
    __log_f=${__parm}
  fi
  {
    printf '=%.0s' {1..79}
    echo ""
    echo "LOG: [$__date] $__msg"
    echo " "
  } >> ${__log_f} 2>&1
}

)";

  static constexpr const char* fnc_prune_pid_file_ = R"(
function __prune_pid_file ()
{
    rm -rf ${__pid_file}
}

)";

  static constexpr const char* check_pid_ = R"(
# If the PID file exists, so don't run the backup
# Shows messages in console and save in log file too
if [[ -e "${__pid_file}" ]]; then
    echo "PID $$ Script is running or has ended unexpectedly. Check [${__pid_file}]" ""
    __logging "PID $$ Script is running or has ended unexpectedly. Check [${__pid_file}]" ""
    exit 0
else
  touch ${__pid_file}
  echo $$ >> ${__pid_file}
fi

)";

  static constexpr const char* exec_status_test_ = R"(      __status=$?
        if [[ "$__status" > 0 ]]; then
            echo "[*] Execution Error: EXIT STATUS = $__status"
        fi
)";

  static constexpr const char* create_instruction_ = R"(

)";

  // ---------------------------------------------------------------------------

  static constexpr std::initializer_list<ScriptTpl> ScriptTpl_List_ = {
    ScriptTpl::TPL_INIT_SCRIPT,
    ScriptTpl::TPL_COPYRIGHT,
    ScriptTpl::TPL_TODAY,
    ScriptTpl::TPL_PID_FILE_,
    ScriptTpl::TPL_PRUNE_PID_FILE_,
    ScriptTpl::TPL_LOG_PATH_,
    ScriptTpl::TPL_LOG_FILE_,
    ScriptTpl::TPL_VARS_EXPORT,
    ScriptTpl::TPL_FUNC_LOGGER,
    ScriptTpl::TPL_CHEK_PID,
    ScriptTpl::TPL_FUNC_HEADER,
    ScriptTpl::TPL_CALL_FUNC,
    ScriptTpl::TPL_EXEC_STATUS_TEST,
    ScriptTpl::TPL_EXTERNAL_COMMANDS_VALIDATIONS
  };

  /*!
   * \brief ScriptTpl_m_1
   * \note Best performance for O(1) search
   */
  const QHash<ScriptTpl, QString> ScriptTpl_m_ = {
    { ScriptTpl::TPL_INIT_SCRIPT, script_header_ },
    { ScriptTpl::TPL_COPYRIGHT, copyright_ },
    { ScriptTpl::TPL_TODAY, today_ },
    { ScriptTpl::TPL_PID_FILE_, pid_file_ },
    { ScriptTpl::TPL_PRUNE_PID_FILE_, fnc_prune_pid_file_ },
    { ScriptTpl::TPL_LOG_FILE_, log_file_ },
    { ScriptTpl::TPL_LOG_PATH_, log_path_ },
    { ScriptTpl::TPL_VARS_EXPORT, vars_export_ },
    { ScriptTpl::TPL_FUNC_LOGGER, fnc_logger_ },
    { ScriptTpl::TPL_CHEK_PID, check_pid_ },
    { ScriptTpl::TPL_FUNC_HEADER, func_header_ },
    { ScriptTpl::TPL_CALL_FUNC, call_func_ },
    { ScriptTpl::TPL_EXEC_STATUS_TEST, exec_status_test_ },
    { ScriptTpl::TPL_EXTERNAL_COMMANDS_VALIDATIONS,
      external_commands_validations_ }
  };

  friend QTextStream& operator<<(QTextStream& out_, const ScriptTemplates& obj_)
  {
    out_ << obj_.scrTemplate().toUtf8();
    return out_;
  }
};
#endif // SCRIPTTEMPLATES_H
