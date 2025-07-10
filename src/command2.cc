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
#include "command2.h"

/*!
 * \brief Command::initializeCommandMap
 * \return
 */
const std::unordered_map<Command::Command_EC, std::string>
Command::initializeCommandMap()
{
  return { { Command_EC::Init, "init" },
           { Command_EC::Create, "create" },
           { Command_EC::Extract, "extract" },
           { Command_EC::Check, "check" },
           { Command_EC::Rename, "rename" },
           { Command_EC::List, "list" },
           { Command_EC::Diff, "diff" },
           { Command_EC::Delete, "delete" },
           { Command_EC::Prune, "prune" },
           { Command_EC::Compact, "compact" },
           { Command_EC::Info, "info" },
           { Command_EC::Mount, "mount" },
           { Command_EC::Umount, "umount" },
           { Command_EC::Key_Change_Passphrase, "key change-passphrase" },
           { Command_EC::Key_Export, "key export" },
           { Command_EC::Key_Import, "key import" },
           { Command_EC::Upgrade, "upgrade" },
           { Command_EC::Recreate, "recreate" },
           { Command_EC::Import_tar, "import-tar" },
           { Command_EC::Export_tar, "export-tar" },
           { Command_EC::Serve, "serve" },
           { Command_EC::Config, "config" },
           { Command_EC::With_lock, "with-lock" },
           { Command_EC::Break_lock, "break-lock" },
           { Command_EC::Benchmark_crud, "benchmark crud" },
           { Command_EC::None, "" } };
}

/*!
 * \brief Command::initializeEncryptMap
 * \return
 */
const std::map<Command::EncryptModes, std::string>
Command::initializeEncryptMap()
{
  return { { EncryptModes::None, "none" },
           { EncryptModes::Repokey_blake2, "repokey_blake2" },
           { EncryptModes::Keyfile, "keyfile" } };
}

/*!
 * \brief Command::initializeArgumentsMap
 * \return
 */
const std::map<Command::InitArguments, std::string>
Command::initializeArgumentsMap()
{
  return { { InitArguments::Append_only, "--append-only" },
           { InitArguments::Make_parents_dir, "--make-parents-dir" },
           { InitArguments::Storage_quata, "--storage-quota" } };
}

/*!
 * \brief Command::initializePlaceHoldersMap
 * \return
 */
const std::map<Command::BorgPlaceHolders, std::string>
Command::initializePlaceHoldersMap()
{
  return { { BorgPlaceHolders::Hostname, "{hostname}" },
           { BorgPlaceHolders::Fqdn, "{fqdn}" },
           { BorgPlaceHolders::Reverse_fqdn, "{reverse-fqdn}" },
           { BorgPlaceHolders::Now, "{now}" },
           { BorgPlaceHolders::Now, "{utcnow}" },
           { BorgPlaceHolders::User, "{user}" },
           { BorgPlaceHolders::Pid, "{pid}" },
           { BorgPlaceHolders::Borgversion, "{borgversion}" },
           { BorgPlaceHolders::Borgmajor, "{borgmajor}" },
           { BorgPlaceHolders::Borgminor, "{borgminor}" },
           { BorgPlaceHolders::Borgpatch, "{borgpatch}" } };
}

/*!
 * \brief Static members initialization
 */
const std::unordered_map<Command::Command_EC, std::string> Command::Command_m_ =
  Command::initializeCommandMap();
const std::map<Command::EncryptModes, std::string> Command::EncryptModes_m_ =
  Command::initializeEncryptMap();
const std::map<Command::InitArguments, std::string> Command::InitArguments_m_ =
  Command::initializeArgumentsMap();
const std::map<Command::BorgPlaceHolders, std::string>
  Command::BorgPlaceholders_m_ = Command::initializePlaceHoldersMap();

/*!
 * \note Constructor
 * \brief Command::Command
 */
Command::Command() = default;

/*!
 * \brief Command::get
 * \param args_
 * \return
 */
const std::string&
Command::get(Command_EC args_) const
{
  static const std::string unknown = "unknown";
  auto it = Command_m_.find(args_);
  return it != Command_m_.end() ? it->second : unknown;
}

/*!
 * \brief Command::get
 * \param args_
 * \return
 */
const Command::Command_EC
Command::getCommandStr(const std::string args_)
{
  for (auto it = Command_m_.cbegin(); it != Command_m_.cend(); ++it) {
    if (it->second == args_) {
      return it->first;
    }
  }

  // key not found
  return Command::Command_EC::None;
}

/*!
 * \brief Command::get
 * \param args_
 * \return
 */
const std::string&
Command::get(EncryptModes args_) const
{
  static const std::string unknown = "unknown";
  auto it = EncryptModes_m_.find(args_);
  return it != EncryptModes_m_.end() ? it->second : unknown;
}

/*!
 * \brief Command::get
 * \param args_
 * \return
 */
const std::string&
Command::get(InitArguments args_) const
{
  static const std::string unknown = "unknown";
  auto it = InitArguments_m_.find(args_);
  return it != InitArguments_m_.end() ? it->second : unknown;
}

/*!
 * \brief Command::get
 * \param args_
 * \return
 */
const std::string&
Command::get(BorgPlaceHolders args_) const
{
  static const std::string unknown = "unknown";
  auto it = BorgPlaceholders_m_.find(args_);
  return it != BorgPlaceholders_m_.end() ? it->second : unknown;
}
