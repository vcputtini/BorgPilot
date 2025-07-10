/***************************************************************************
 * Copyright (c) 2023-2024                                                 *
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
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <initializer_list>

#include <QMap>
#include <QString>

/*!
 * \brief The Arguments class
 */
class Arguments
{
public:
  static constexpr const char* BORG_ = "borg";

  enum class Arguments_EC
  {
    Init = 0x00,
    Create,
    Extract,
    Check,
    Rename,
    List,
    Diff,
    Delete,
    Prune,
    Compact,
    Info,
    Mount,
    Umount,
    Key_Change_Passphrase,
    Key_Export,
    Key_Import,
    Upgrade,
    Recreate,
    Import_tar,
    Export_tar,
    Serve,
    Config,
    With_lock,
    Break_lock,
    Benchmark_crud
  };

  static constexpr std::initializer_list<Arguments_EC> Arguments_List_ = {
    Arguments_EC::Init,          Arguments_EC::Create,
    Arguments_EC::Extract,       Arguments_EC::Check,
    Arguments_EC::Rename,        Arguments_EC::List,
    Arguments_EC::Diff,          Arguments_EC::Delete,
    Arguments_EC::Prune,         Arguments_EC::Compact,
    Arguments_EC::Info,          Arguments_EC::Mount,
    Arguments_EC::Umount,        Arguments_EC::Key_Change_Passphrase,
    Arguments_EC::Key_Export,    Arguments_EC::Key_Import,
    Arguments_EC::Upgrade,       Arguments_EC::Recreate,
    Arguments_EC::Import_tar,    Arguments_EC::Export_tar,
    Arguments_EC::Serve,         Arguments_EC::Config,
    Arguments_EC::With_lock,     Arguments_EC::Break_lock,
    Arguments_EC::Benchmark_crud
  };

  static constexpr const char* init_ = "init";
  static constexpr const char* create_ = "create";
  static constexpr const char* extract_ = "extract";
  static constexpr const char* check_ = "check";
  static constexpr const char* rename_ = "rename";
  static constexpr const char* list_ = "list";
  static constexpr const char* diff_ = "diff";
  static constexpr const char* delete_ = "delete";
  static constexpr const char* prune_ = "prune";
  static constexpr const char* compact_ = "compact";
  static constexpr const char* info_ = "info";
  static constexpr const char* mount_ = "mount";
  static constexpr const char* umount_ = "umount";
  static constexpr const char* key_change_passphrase_ = "key change-passphrase";
  static constexpr const char* key_export_ = "key export";
  static constexpr const char* key_import_ = "key import";
  static constexpr const char* upgrade_ = "upgrade";
  static constexpr const char* recreate_ = "recreate";
  static constexpr const char* import_tar_ = "import-tar";
  static constexpr const char* export_tar_ = "export-tar";
  static constexpr const char* serve_ = "serve";
  static constexpr const char* config_ = "config";
  static constexpr const char* with_lock_ = "with-lock";
  static constexpr const char* break_lock_ = "break-lock";
  static constexpr const char* benchmark_crud_ = "benchmark crud";

  const QMap<Arguments_EC, QString> Arguments_m_ = {
    { Arguments_EC::Init, init_ },
    { Arguments_EC::Create, create_ },
    { Arguments_EC::Extract, extract_ },
    { Arguments_EC::Check, check_ },
    { Arguments_EC::Rename, rename_ },
    { Arguments_EC::List, list_ },
    { Arguments_EC::Diff, diff_ },
    { Arguments_EC::Diff, diff_ },
    { Arguments_EC::Delete, delete_ },
    { Arguments_EC::Prune, prune_ },
    { Arguments_EC::Compact, compact_ },
    { Arguments_EC::Info, info_ },
    { Arguments_EC::Mount, mount_ },
    { Arguments_EC::Umount, umount_ },
    { Arguments_EC::Key_Change_Passphrase, key_change_passphrase_ },
    { Arguments_EC::Key_Export, key_export_ },
    { Arguments_EC::Key_Import, key_import_ },
    { Arguments_EC::Serve, serve_ },
    { Arguments_EC::Config, config_ },
    { Arguments_EC::With_lock, with_lock_ },
    { Arguments_EC::Break_lock, break_lock_ },
    { Arguments_EC::Benchmark_crud, benchmark_crud_ }
  };

  enum class EncryptModes
  {
    None = 0x00,
    Repokey_blake2,
    Keyfile
  };

  static constexpr const char* init_em_none_ = "none";
  static constexpr const char* init_em_repokey_blake2_ = "repokey_blake2";
  static constexpr const char* init_em_keyfile_ = "keyfile";

  const QMap<EncryptModes, QString> EncryptModes_m_ = {
    { EncryptModes::None, init_em_none_ },
    { EncryptModes::Repokey_blake2, init_em_repokey_blake2_ },
    { EncryptModes::Keyfile, init_em_keyfile_ }
  };

  static constexpr const char* append_only_ = "--append-only";
  static constexpr const char* make_parents_dir_ = "--make-parents-dir";
  static constexpr const char* sto_quota_ = "--storage_quota";

public:
  explicit Arguments();

  const QString get(Arguments_EC args_) const;
  const QString get(EncryptModes args_) const;
};

#endif // ARGUMENTS_H
