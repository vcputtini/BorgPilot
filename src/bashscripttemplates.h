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
#ifndef BASHSCRIPTTEMPLATES_H
#define BASHSCRIPTTEMPLATES_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <qstringliteral.h>

#include <algorithm>
#include <array>
#include <initializer_list>

class BashScriptTemplates /*: public QObject*/
{
  /*Q_OBJECT*/
public:
  enum class ScriptTpl
  {
    TPL_BASH_HEADER,
    TPL_BASH_VARS,
    TPL_BASH_LOG,
    TPL_BASH_CHECK_COMMANDS,
    TPL_BASH_CHECK_PRIVILEGES,
    TPL_BASH_ENSURE_DIRS,
    TPL_BASH_PID_TREATMENT,
    TPL_BASH_CLEANUP,
    TPL_BASH_BORG_ENV,
    TPL_BASH_BORG_CREATE,
    TPL_BASH_BORG_INIT_REPO,
    TPL_BASH_MAIN,
    TPL_NONE
  };

  explicit BashScriptTemplates(ScriptTpl tpl_ = ScriptTpl::TPL_NONE
                               /*QObject* parent = nullptr*/);

  QString getScriptTemplate(ScriptTpl tpl_ = ScriptTpl::TPL_NONE) const;

  //
  // BASH SCRIPT TEMPLATES ===================================================
  //
  static constexpr const char* bash_header_ = R"(#!/usr/bin/env bash
# ==============================================================================
# BorgPilot: Backup Script
# Copyright(C) 2026 by Volnei Cervi Puttini <vputtini@vcplinux.com.br>
# ==============================================================================

set -euEo pipefail

# Trap for debugging and cleanup in case of error.
readonly SCRIPT_NAME="${0##*/}"
trap '__log_error "line ${BASH_LINENO[0]}: $BASH_COMMAND"; __cleanup; exit 1' ERR
trap '__cleanup' EXIT

)";

  static constexpr const char* bash_vars_ = R"RAW(
readonly PID_FILE="/var/run/borgpilot.pid"
readonly LOG_PATH="/var/log/borgpilot"
readonly REQUIRED_CMDS=("borg" "date" "hostname" "id" "whoami")
readonly SCRIPT_PID=$$

)RAW";

  static constexpr const char* bash_log_ =
    R"RAW(
__log() {
    local level="$1"
    shift
    local message="$*"
    local timestamp="$(date '+%Y-%m-%d_%H:%M:%S')"
    local LOG_FILE="${LOG_PATH}/borgpilot_${timestamp}.log"

    {
        printf '=%.0s' {1..79}
        echo ""
        printf "LOG: [%s] [%s] %s\n" "$timestamp" "$level" "$message"
        echo ""
    } >> "$LOG_FILE" 2>&1 || {
        printf '[FATAL] Failed to write log: %s\n' "$LOG_FILE" >&2
        exit 1
    }
}

__log_info() { __log "INFO" "$@"; }
__log_error() { __log "ERROR" "$@" >&2; }
__log_warn() { __log "WARN" "$@"; }

)RAW";

  static constexpr const char* bash_check_commands_ =
    R"RAW(
__check_commands() {
    local cmd
    for cmd in "${REQUIRED_CMDS[@]}"; do
        if ! command -v "$cmd" >/dev/null 2>&1; then
            __log_error "Required command not found: $cmd"
            exit 1
        fi
    done
}

)RAW";

  static constexpr const char* bash_check_privileges_ =
    R"RAW(
__check_root() {
    if [[ "$(id -u)" != 0 ]]; then
        __log_error "This script must be run as root"
        printf '[ERROR] This script must be run as root\n' >&2
        exit 1
    fi
}
)RAW";

  static constexpr const char* bash_ensure_dirs_ =
    R"RAW(
__ensure_dirs() {
    local dir
    for dir in "${REQUIRED_DIRS[@]}"; do
        if [[ ! -d "$dir" ]]; then
            __log_info "Creating directory: $dir"
            mkdir -p "$dir" || {
                __log_error "Failed to create directory: $dir"
                exit 1
            }
        fi
    done
}

)RAW";

  static constexpr const char* bash_pid_treatment_ =
    R"RAW(
# Secure PID file management
__pid_running() {
    [[ -f "$PID_FILE" ]] && kill -0 "$(cat "$PID_FILE" 2>/dev/null)" 2>/dev/null
}

__create_pid_file() {
    # Checks if an active PID already exists.
    if [[ -f "$PID_FILE" ]]; then
        local existing_pid
        existing_pid="$(cat "$PID_FILE" 2>/dev/null)" || existing_pid="unknown"

        if __pid_running; then
            __log_error "The script is already running. (PID: $existing_pid)"
            printf 'The script is already running. PID: %s\n' "$existing_pid" >&2
            exit 1
        else
            __log_warn "Old PID found ($existing_pid), removing..."
            rm -f "$PID_FILE"
        fi
    fi

    # Creates a new PID file atomically
    printf '%d' "$SCRIPT_PID" > "$PID_FILE" || {
        __log_error "Failed to create PID file: $PID_FILE"
        exit 1
    }
    chmod 644 "$PID_FILE"
}

)RAW";

  static constexpr const char* bash_cleanup_ =
    R"RAW(
__cleanup() {
    [[ -f "$PID_FILE" && "$(cat "$PID_FILE" 2>&1)" == "$SCRIPT_PID" ]] && rm -f "$PID_FILE"
}

)RAW";

  static constexpr const char* bash_borg_env_ =
    R"RAW(
# Borg environment configuration (default directories only)
__setup_borg_env() {
    readonly __borg_base_dir="%0"
    readonly __borg_cache_dir="%1"
    readonly __borg_config_dir="%2"
    readonly __borg_keys_dir="%5"
    readonly __borg_key_file="%6"
    readonly __borg_security_dir="%4"
    readonly __tmpdir="%7"

    readonly REQUIRED_DIRS=(
        "${__borg_base_dir}",
        "${__borg_cache_dir}",
        "${__borg_config_dir}",
        "${__borg_keys_dir}",
        "${__borg_key_file}",
        "${__borg_security_dir}",
        "${__tmpdir}"
    )

    export BORG_BASE_DIR=${__borg_base_dir}
    export BORG_CACHE_DIR=${__borg_cache_dir}
    export BORG_CONFIG_DIR=${__borg_config_dir}
    export BORG_KEYS_DIR=${__borg_keys_dir}
    export BORG_KEY_FILE=${__borg_key_file}
    export BORG_SECURITY_DIR=${__borg_security_dir}
    export TMPDIR=${__tmpdir}

}

)RAW";

  /*!
   * \brief bash_borg_create_
   * \note This function has been intentionally left incomplete.
   * The missing parts will be included in the code generation  by the
   * BashScriptGenerator::commandForBackup() function.
   */
  static constexpr const char* bash_borg_create_ =
    R"RAW(
# Main backup function
__borg_create() {
    __log_info "Starting BorgBackup backup process"

)RAW";

  /*!
   * \brief bash_borg_init_repo_
   * \note This function has been intentionally left incomplete.
   * The missing parts will be included in the code generation  by the
   * BashScriptGenerator::commandForInitialization() function.
   */
  static constexpr const char* bash_borg_init_repo_ =
    R"RAW(
__borg_init_repository() {
    __log_info "Starting BorgBackup repository intialization process"

)RAW";

  static constexpr const char* bash_main_ =
    R"RAW(
main() {
    __log_info "Starting BorgPilot Backup Script (PID: $SCRIPT_PID)"

    __setup_borg_env
    __check_root
    __ensure_dirs
    __check_commands
    __create_pid_file
    __log_info "Environment successfully configured."

    __borg_create

    __log_info "Script completed successfully."
}

# Execute main only if the script is called directly.
[[ "${BASH_SOURCE[0]}" == "${0}" ]] && main "$@"

# EOF

)RAW";

  static constexpr std::string_view bash_main_def0 =
    R"RAW(
main() {
    __log_info "Starting BorgPilot Backup Script (PID: $SCRIPT_PID)"

    __setup_borg_env
    __check_root
    __ensure_dirs
    __check_commands
    __create_pid_file
    __log_info "Environment successfully configured."

    __borg_create

)RAW";

  static constexpr std::string_view bash_main_def1 =
    R"RAW(
main() {
    __log_info "Starting BorgPilot Backup Script (PID: $SCRIPT_PID)"

    __setup_borg_env
    __check_root
    __ensure_dirs
    __check_commands
    __create_pid_file
    __log_info "Environment successfully configured."

    __borg_init_repository

)RAW";

  static constexpr std::string_view bash_main_def2 =
    R"RAW(
    __log_info "Script completed successfully."
}

# Execute main only if the script is called directly.
[[ "${BASH_SOURCE[0]}" == "${0}" ]] && main "$@"

# EOF

)RAW";

private:
  ScriptTpl tmpTpl_;

  static constexpr std::initializer_list<ScriptTpl> ScriptTpl_List_ = {
    ScriptTpl::TPL_BASH_HEADER,
    ScriptTpl::TPL_BASH_VARS,
    ScriptTpl::TPL_BASH_LOG,
    ScriptTpl::TPL_BASH_CHECK_COMMANDS,
    ScriptTpl::TPL_BASH_CHECK_PRIVILEGES,
    ScriptTpl::TPL_BASH_ENSURE_DIRS,
    ScriptTpl::TPL_BASH_PID_TREATMENT,
    ScriptTpl::TPL_BASH_CLEANUP,
    ScriptTpl::TPL_BASH_BORG_ENV,
    ScriptTpl::TPL_BASH_BORG_CREATE,
    ScriptTpl::TPL_BASH_BORG_INIT_REPO,
    ScriptTpl::TPL_BASH_MAIN
  };

  /*!
   * \brief ScriptTpl_m_1
   * \note Best performance for O(1) search
   */
  const QHash<ScriptTpl, QString> ScriptTpl_m_ = {
    { ScriptTpl::TPL_BASH_HEADER, bash_header_ },
    { ScriptTpl::TPL_BASH_VARS, bash_vars_ },
    { ScriptTpl::TPL_BASH_LOG, bash_log_ },
    { ScriptTpl::TPL_BASH_CHECK_COMMANDS, bash_check_commands_ },
    { ScriptTpl::TPL_BASH_CHECK_PRIVILEGES, bash_check_privileges_ },
    { ScriptTpl::TPL_BASH_ENSURE_DIRS, bash_ensure_dirs_ },
    { ScriptTpl::TPL_BASH_PID_TREATMENT, bash_pid_treatment_ },
    { ScriptTpl::TPL_BASH_CLEANUP, bash_cleanup_ },
    { ScriptTpl::TPL_BASH_BORG_ENV, bash_borg_env_ },
    { ScriptTpl::TPL_BASH_BORG_CREATE, bash_borg_create_ },
    { ScriptTpl::TPL_BASH_BORG_INIT_REPO, bash_borg_init_repo_ },
    { ScriptTpl::TPL_BASH_MAIN, bash_main_ }
  };

  friend QTextStream& operator<<(QTextStream& out_,
                                 const BashScriptTemplates& obj_)
  {
    out_ << obj_.getScriptTemplate().toUtf8();
    return out_;
  }
};

#endif // BASHSCRIPTTEMPLATES_H
