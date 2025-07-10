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
#ifndef GLOBAL_OPTIONS_H
#define GLOBAL_OPTIONS_H

#include <QList>
#include <QPair>
#include <QString>

#include <map> // std::map
/*!
 * \brief The GlobalOptions class
 */

// SUGESTAO DE NOVO NOME: BorgGlobalOptions
class GlobalOptions
{
public:
  /*!
   * \brief The GlobalOptions_e enum
   * \note Enumerator representing common options
   */
  enum class GlobalOptions_e
  {
    Critical,
    Error,
    Warning,
    Info,
    Debug,
    Debug_topic,
    Progress,
    Iec,
    Log_json,
    Lock_wait,
    Bypass_lock,
    Show_version,
    Show_rc,
    Umask,
    Remote_path,
    Upload_ratelimit,
    Upload_buffer,
    Consider_part_files,
    Debug_profile,
    Rsh,
    None
  };

  /*!
   * \brief The Optionals_e enum
   * \note Enumerator representing optional options
   */
  enum class Optionals_e
  {
    Dryrun = 0x00,
    Stats,
    List,
    Filter,
    Json,
    NoCacheSync,
    StdinName,
    Stdinuser,
    StdinGroup,
    StdinMode,
    ContentFromCommand,
    PathsFromStdin,
    PathsFromCommand,
    PathsDelimiter,
    Exclude,
    ExcludeFrom,
    Pattern,
    PatternsFrom,
    ExcludeCaches,
    ExcludeIfPresent,
    KeepExcludeTags,
    ExcludeNoDump,
    OneFileSystem,
    NumericIds,
    NoAtime,
    Atime,
    NocTime,
    NoBirthtime,
    NoFlags,
    NoAcls,
    NoXAttres,
    Sparse,
    FilesCache,
    ReadSpecial,
    Comment,
    Timestamp,
    CheckpointInterval,
    ChunkerParams,
    Compression
  };

  enum class eGlobalOptions
  {
    /* Commons */
    Critical = 0x00,
    Error,
    Warning,
    Info,
    Debug,
    Debug_topic,
    Progress,
    Iec,
    Log_json,
    Lock_wait,
    Bypass_lock,
    Show_version,
    Show_rc,
    Umask,
    Remote_path,
    Upload_ratelimit,
    Upload_buffer,
    Consider_part_files,
    Debug_profile,
    Rsh,

    /* optionals */
    Dryrun,
    Stats,
    List,
    Filter,
    Json,
    NoCacheSync,
    StdinName,
    Stdinuser,
    StdinGroup,
    StdinMode,
    ContentFromCommand,
    PathsFromStdin,
    PathsFromCommand,
    PathsDelimiter,
    Exclude,
    ExcludeFrom,
    Pattern,
    PatternsFrom,
    ExcludeCaches,
    ExcludeIfPresent,
    KeepExcludeTags,
    ExcludeNoDump,
    OneFileSystem,
    NumericIds,
    NoAtime,
    Atime,
    NocTime,
    NoBirthtime,
    NoFlags,
    NoAcls,
    NoXAttres,
    Sparse,
    FilesCache,
    ReadSpecial,
    Comment,
    Timestamp,
    CheckpointInterval,
    ChunkerParams,
    Compression,
    None
  };

  explicit GlobalOptions();

  static const QString& getOptions(GlobalOptions_e opts_);
  static const QString& description(GlobalOptions_e opts_);
  static const GlobalOptions_e getEnum(const QString& strOpts_);

  static QString getOptionals(Optionals_e opts_);
  static QString optionalDescription(Optionals_e opts_);

  static std::map<GlobalOptions_e, QPair<QString, QString>>::const_iterator
  cbeginMap();
  static std::map<GlobalOptions_e, QPair<QString, QString>>::const_iterator
  cendMap();

  static std::map<Optionals_e, QPair<QString, QString>>::const_iterator
  cbeginMapOptional();
  static std::map<Optionals_e, QPair<QString, QString>>::const_iterator
  cendMapOptional();

  // teste ====

  static std::map<eGlobalOptions, QPair<QString, QString>>::const_iterator
  cbeginMapAll();
  static std::map<eGlobalOptions, QPair<QString, QString>>::const_iterator
  cendMapAll();

  static const QString get(const eGlobalOptions opts_)
  {
    static const QString unknown_ = "unknown";
    auto it_ = GlobalAll_m_.find(opts_);
    return it_ != GlobalAll_m_.end() ? it_->second.first : unknown_;
  }

  // ==========

  /**
   * \brief Checks if an argument is in the global options list.
   * \tparam T Argument type (QString or Optionals_e).
   * \tparam param Argument to check.
   * \return true if the argument is in the list, false otherwise.
   */
  template<typename T>
  bool haveArgs(const T& param) const
  {
    static const QList<QString> args_ = {
      "--filter",       "--stdin-name",         "--stdin-user",
      "--stdin-group",  "--stdin-mode",         "--paths-from-delimiter",
      "--exclude",      "--exclude-from",       "--pattern",
      "--pattern-from", "--exclude-if-present", "--file-cache",
      "--comment",      "--checkpoint-params",  "--compression"
    };

    QString value;
    if constexpr (std::is_same_v<T, Optionals_e>) {
      GlobalOptions glb_;
      value = glb_.getOptionals(param);
    } else {
      value = param;
    }

    return std::find(args_.constBegin(), args_.constEnd(), value) !=
           args_.constEnd();
  }

private:
  static const std::map<GlobalOptions_e, QPair<QString, QString>>
  initializeGlobalOptionsMap();
  static const std::map<Optionals_e, QPair<QString, QString>>
  initializeOptionalsMap();

  static const std::map<GlobalOptions_e, QPair<QString, QString>>
    GlobalOptions_m_;
  static const std::map<Optionals_e, QPair<QString, QString>> Optional_m_;

  // test ======
  static const std::map<eGlobalOptions, QPair<QString, QString>>
  initializeAll();

  static const std::map<eGlobalOptions, QPair<QString, QString>> GlobalAll_m_;
  // ============
};

#endif // GLOBAL_OPTIONS_H
