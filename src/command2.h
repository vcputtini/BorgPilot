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
#ifndef COMMAND2_H
#define COMMAND2_H

#include <QList>
#include <QVector>

#include <map>
#include <string>
#include <unordered_map>

/*!
 * \brief The Command class
 */
class Command
{
public:
  static constexpr const char* BORG_ = "/usr/bin/env borg";

  enum class Command_EC
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
    Benchmark_crud,
    None
  };

  enum class EncryptModes
  {
    None = 0x00,
    Repokey_blake2,
    Keyfile
  };

  enum class InitArguments
  {
    Append_only = 0x00,
    Make_parents_dir,
    Storage_quata
  };

  enum class BorgPlaceHolders
  {
    Hostname = 0x00,
    Fqdn,
    Reverse_fqdn,
    Now,
    Utcnow,
    User,
    Pid,
    Borgversion,
    Borgmajor,
    Borgminor,
    Borgpatch
  };

  explicit Command();

  const std::string& get(Command_EC args_) const;
  const Command_EC getCommandStr(const std::string args_);
  const std::string& get(EncryptModes args_) const;
  const std::string& get(InitArguments args_) const;
  const std::string& get(BorgPlaceHolders args_) const;

  static QVector<std::string> getCommandsList()
  {
    QVector<std::string> list_{};
    for (auto& it_ : Command_m_) {
      list_.append(it_.second);
    }
    return list_;
  }

private:
  static const std::unordered_map<Command_EC, std::string>
  initializeCommandMap();
  static const std::map<EncryptModes, std::string> initializeEncryptMap();
  static const std::map<InitArguments, std::string> initializeArgumentsMap();
  static const std::map<BorgPlaceHolders, std::string>
  initializePlaceHoldersMap();

  static const std::unordered_map<Command_EC, std::string> Command_m_;
  static const std::map<EncryptModes, std::string> EncryptModes_m_;
  static const std::map<InitArguments, std::string> InitArguments_m_;
  static const std::map<BorgPlaceHolders, std::string> BorgPlaceholders_m_;
};

#endif // COMMAND2_H
