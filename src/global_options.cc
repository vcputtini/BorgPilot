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
#include "global_options.h"

/*!
 * \brief GlobalOptions::initializeGlobalOptionsMap
 * \return
 */
const std::map<GlobalOptions::GlobalOptions_e, QPair<QString, QString>>
GlobalOptions::initializeGlobalOptionsMap()
{
  return {
    { GlobalOptions_e::Critical,
      { "--critical", QT_TR_NOOP("work on log level CRITICAL") } },
    { GlobalOptions_e::Error,
      { "--error", QT_TR_NOOP("work on log level ERROR") } },
    { GlobalOptions_e::Warning,
      { "--warning", QT_TR_NOOP("work on log level WARNING (default)") } },
    { GlobalOptions_e::Info,
      { "--info", QT_TR_NOOP("work on log level INFO") } },
    { GlobalOptions_e::Debug,
      { "--debug",
        QT_TR_NOOP("enable debug output, work on log level DEBUG") } },
    { GlobalOptions_e::Debug_topic,
      { "--debug-topic",
        QT_TR_NOOP("enable TOPIC debugging (multiple times allowed)") } },
    { GlobalOptions_e::Progress,
      { "--progress", QT_TR_NOOP("show progress information") } },
    { GlobalOptions_e::Iec,
      { "--iec", QT_TR_NOOP("format using IEC units(1KiB = 1024B)") } },
    { GlobalOptions_e::Log_json,
      { "--log-json",
        QT_TR_NOOP("Output one JSON object per log lineinstead of "
                   "formatted text") } },
    { GlobalOptions_e::Lock_wait,
      { "--lock-wait",
        QT_TR_NOOP("wait at most SECONDS for acquiring a repository/cache lock "
                   "(default: 1)") } },

    { GlobalOptions_e::Bypass_lock,
      { "--bypass-lock", QT_TR_NOOP("Bypass locking mechanism") } },
    { GlobalOptions_e::Show_version,
      { "--show-version", QT_TR_NOOP("show/log the borg version") } },
    { GlobalOptions_e::Show_rc,
      { "--show-rc", QT_TR_NOOP("show/log the return code (rc)") } },
    { GlobalOptions_e::Umask,
      { "--umask", QT_TR_NOOP("set umask to M (local only,default: 0077)") } },
    { GlobalOptions_e::Remote_path,
      { "--remote-path",
        QT_TR_NOOP(
          "use PATH as borg executable on theremote (default: \"borg\")") } },

    { GlobalOptions_e::Upload_ratelimit,
      { "--upload-ratelimit",
        QT_TR_NOOP("set network upload rate limit in kiByte/s (default: "
                   "0=unlimited)") } },
    { GlobalOptions_e::Upload_buffer,
      { "--upload-buffer", QT_TR_NOOP("set network upload buffer size") } },
    { GlobalOptions_e::Consider_part_files,
      { "--consider-part-files",
        QT_TR_NOOP("treat part files like normal files (e.g.to "
                   "list/extract them)") } },

    { GlobalOptions_e::Debug_profile,
      { "--debug-profile",
        QT_TR_NOOP("Write execution profile in Borg format into FILE.") } },
    { GlobalOptions_e::Rsh,
      { "--rsh",
        QT_TR_NOOP("Use this command to connect to the ‘borg serve’ process "
                   "(default: ‘ssh’)") } },
    { GlobalOptions_e::None, { "", "" } }
  };
}

/*!
 * \brief GlobalOptions::initializeOptionalsMap
 * \return
 */
const std::map<GlobalOptions::Optionals_e, QPair<QString, QString>>
GlobalOptions::initializeOptionalsMap()
{
  return {
    { Optionals_e::Dryrun,
      { qMakePair("--dry-run",
                  QT_TR_NOOP("Do not create a backup archive")) } },
    { Optionals_e::Stats,
      { qMakePair("--stats",
                  QT_TR_NOOP("Print statistics for the created archive")) } },
    { Optionals_e::List,
      { qMakePair("--list",
                  QT_TR_NOOP("Output verbose list of items (files, "
                             "dirs, …)")) } },
    { Optionals_e::Filter,
      { qMakePair("--filter",
                  QT_TR_NOOP("Only display items with the given "
                             "status characters (see "
                             "description)")) } },
    { Optionals_e::Json,
      { qMakePair("--json",
                  QT_TR_NOOP("Output stats as JSON. Implies --stats.")) } },
    { Optionals_e::NoCacheSync,
      { qMakePair("--no-cache-sync",
                  QT_TR_NOOP("Experimental: do not synchronize the "
                             "cache. Implies not "
                             "using the files cache.")) } },
    { Optionals_e::StdinName,
      { qMakePair("--stdin-name",
                  QT_TR_NOOP("Use NAME in archive for stdin data "
                             "(default: ‘stdin’)")) } },
    { Optionals_e::Stdinuser,
      { qMakePair("--stdin-user",
                  QT_TR_NOOP("Set user USER in archive for stdin "
                             "data (default: ‘root’)")) } },
    { Optionals_e::StdinGroup,
      { qMakePair("--stdin-group",
                  QT_TR_NOOP("Set group GROUP in archive for stdin "
                             "data (default: ‘wheel’)")) } },
    { Optionals_e::StdinMode,
      { qMakePair("--stdin-mode",
                  QT_TR_NOOP("Set mode to M in archive for stdin "
                             "data (default: 0660)")) } },
    { Optionals_e::ContentFromCommand,
      { qMakePair("--content-from-command",
                  QT_TR_NOOP("Interpret PATH as command and store "
                             "its stdout.")) } },
    { Optionals_e::PathsFromStdin,
      { qMakePair("--paths-from-stdin",
                  QT_TR_NOOP("Read DELIM-separated list of paths "
                             "to backup from stdin. "
                             "All control is external: it will "
                             "back up all files given - "
                             "no more, no less.")) } },
    { Optionals_e::PathsFromCommand,
      { qMakePair("--paths-from-command",
                  QT_TR_NOOP("Interpret PATH as command and treat "
                             "its output as "
                             "--paths-from-stdin")) } },
    { Optionals_e::PathsDelimiter,
      { qMakePair("--paths-from-delimiter",
                  QT_TR_NOOP("Set path delimiter for --paths-from-stdin and "
                             "--paths-from-command (default: \n)")) } },
    { Optionals_e::Exclude,
      { qMakePair("--exclude",
                  QT_TR_NOOP("Exclude paths matching PATTERN")) } },
    { Optionals_e::ExcludeFrom,
      { qMakePair(
        "--exclude-from",
        QT_TR_NOOP("Read exclude patterns from EXCLUDEFILE, one per line")) } },
    { Optionals_e::Pattern,
      { qMakePair("--pattern",
                  QT_TR_NOOP("Include/exclude paths matching PATTERN")) } },
    { Optionals_e::PatternsFrom,
      { qMakePair(
        "--patterns-from",
        QT_TR_NOOP(
          "Read include/exclude patterns from PATTERNFILE, one per line")) } },
    { Optionals_e::ExcludeCaches,
      { qMakePair(
        "--exclude-caches",
        QT_TR_NOOP("Exclude directories that contain a CACHEDIR.TAG file "
                   "(http://www.bford.info/cachedir/spec.html)")) } },
    { Optionals_e::ExcludeIfPresent,
      { qMakePair(
        "--exclude-if-present",
        QT_TR_NOOP("Exclude directories that are tagged by containing a "
                   "filesystem object with the given NAME")) } },
    { Optionals_e::KeepExcludeTags,
      { qMakePair(
        "--keep-exclude-tags",
        QT_TR_NOOP(
          "If tag objects are specified with --exclude-if-present, don’t omit "
          "the tag objects themselves from the backup archive")) } },
    { Optionals_e::ExcludeNoDump,
      { qMakePair("--exclude-nodump",
                  QT_TR_NOOP("Rxclude files flagged NODUMP")) } },
    { Optionals_e::OneFileSystem,
      qMakePair(
        "--one-file-system",
        QT_TR_NOOP("Stay in the same file system and do not store mount points "
                   "of other file systems - this might behave different from "
                   "your expectations, see the description below.")) },
    { Optionals_e::NumericIds,
      qMakePair("--numeric-ids",
                QT_TR_NOOP("Only store numeric user and group identifiers")) },
    { Optionals_e::NoAtime,
      qMakePair("--noatime", QT_TR_NOOP("Do not store atime into archive")) },
    { Optionals_e::Atime,
      qMakePair("--atime", QT_TR_NOOP("Do store atime into archive")) },
    { Optionals_e::NocTime,
      qMakePair("--noctime", QT_TR_NOOP("Do not store ctime into archive")) },
    { Optionals_e::NoBirthtime,
      qMakePair(
        "--nobirthtime",
        QT_TR_NOOP("Do not store birthtime (creation date) into archive")) },
    { Optionals_e::NoFlags,
      qMakePair("--noflags",
                QT_TR_NOOP("Do not read and store flags (e.g. NODUMP, "
                           "IMMUTABLE) into archive")) },
    { Optionals_e::NoAcls,
      qMakePair("--noacls",
                QT_TR_NOOP("Do not read and store ACLs into archive")) },
    { Optionals_e::NoXAttres,
      qMakePair("--noxattrs",
                QT_TR_NOOP("Do not read and store xattrs into archive")) },
    { Optionals_e::Sparse,
      qMakePair(
        "--spase",
        QT_TR_NOOP(
          "Detect sparse holes in input (supported only by fixed chunker)")) },
    { Optionals_e::FilesCache,
      qMakePair(
        "--files-cache",
        QT_TR_NOOP("Dperate files cache in MODE. default: ctime,size,inode")) },
    { Optionals_e::ReadSpecial,
      qMakePair(
        "--read-special",
        QT_TR_NOOP("Open and read block and char device files as well as FIFOs "
                   "as if they were regular files. Also follows symlinks "
                   "pointing to these kinds of files.")) },
    { Optionals_e::Comment,
      qMakePair("--comment", QT_TR_NOOP("Add a comment text to the archive")) },
    { Optionals_e::Timestamp,
      qMakePair("--timestamp",
                QT_TR_NOOP("Manually specify the archive creation date/time "
                           "(UTC, yyyy-mm-ddThh:mm:ss format). Alternatively, "
                           "give a reference file/directory.")) },
    { Optionals_e::CheckpointInterval,
      qMakePair(
        "--checkpoint-interval",
        QT_TR_NOOP("Write checkpoint every SECONDS seconds (Default: 1800)")) },
    { Optionals_e::ChunkerParams,
      qMakePair(
        "--chunk-params",
        QT_TR_NOOP("Specify the chunker parameters (ALGO, CHUNK_MIN_EXP, "
                   "CHUNK_MAX_EXP, HASH_MASK_BITS, HASH_WINDOW_SIZE). default: "
                   "buzhash,19,23,21,4095")) },
    { Optionals_e::Compression,
      { qMakePair(
        "--compression",
        QT_TR_NOOP(
          "Select compression algorithm, see the output of the “borg help "
          "compression” command for details.")) } }
  };
}

/*!
 * \brief Static members initialization
 */
const std::map<GlobalOptions::GlobalOptions_e, QPair<QString, QString>>
  GlobalOptions::GlobalOptions_m_ = GlobalOptions::initializeGlobalOptionsMap();

const std::map<GlobalOptions::Optionals_e, QPair<QString, QString>>
  GlobalOptions::Optional_m_ = GlobalOptions::initializeOptionalsMap();

/*!
 * \public
 * \brief cbeginItMap
 * \return
 */
std::map<GlobalOptions::GlobalOptions_e,
         QPair<QString, QString>>::const_iterator
GlobalOptions::cbeginMap()
{
  return GlobalOptions_m_.cbegin();
}

/*!
 * \public
 * \brief cendItMap
 * \return
 */
std::map<GlobalOptions::GlobalOptions_e,
         QPair<QString, QString>>::const_iterator
GlobalOptions::cendMap()
{
  return GlobalOptions_m_.cend();
}

/*!
 * \private
 * \brief GlobalOptions::cbeginMapOptional
 * \return
 */
std::map<GlobalOptions::Optionals_e, QPair<QString, QString>>::const_iterator
GlobalOptions::cbeginMapOptional()
{
  return Optional_m_.cbegin();
}

/*!
 * \private
 * \brief GlobalOptions::cendMapOptional
 * \return
 */
std::map<GlobalOptions::Optionals_e, QPair<QString, QString>>::const_iterator
GlobalOptions::cendMapOptional()
{
  return Optional_m_.cend();
}

/*!
 * \brief GlobalOptions::GlobalOptions
 */
GlobalOptions::GlobalOptions() = default;

/*!
 * \public
 * \brief GlobalOptions::getOptions
 * \param opts_
 * \return QString&
 */
const QString&
GlobalOptions::getOptions(GlobalOptions_e opts_)
{
  static const QString unknown_ = "unknown";
  auto it_ = GlobalOptions_m_.find(opts_);
  return it_ != GlobalOptions_m_.end() ? it_->second.first : unknown_;
}

/*!
 * \public
 * \brief GlobalOptions::description
 * \param opts_
 * \return QString&
 */
const QString&
GlobalOptions::description(GlobalOptions_e opts_)
{
  static const QString unknown_ = "unknown";
  auto it_ = GlobalOptions_m_.find(opts_);
  return it_ != GlobalOptions_m_.end() ? it_->second.second : unknown_;
}

/*!
 * \public
 * \brief GlobalOptions::getEnum
 * \param strOpts_ ex: --list, --stats ...
 * \return GlobalOptions::GlobalOptions_e
 */
const GlobalOptions::GlobalOptions_e
GlobalOptions::getEnum(const QString& strOpts_)
{
  for (auto&& it_ = GlobalOptions_m_.cbegin(); it_ != GlobalOptions_m_.cend();
       ++it_) {
    if (it_->second.first == strOpts_) {
      return it_->first;
    }
  }

  return GlobalOptions_e::None;
}

/*!
 * \public
 * \brief GlobalOptions::getOptionals
 * \param opts_
 * \return QString
 */
QString
GlobalOptions::getOptionals(Optionals_e opts_)
{
  static const QString unknown_ = "unknown";
  auto it_ = Optional_m_.find(opts_);
  return it_ != Optional_m_.end() ? it_->second.first : unknown_;
}

/*!
 * \public
 * \brief GlobalOptions::optionalDescription
 * \param opts_
 * \return QString
 */
QString
GlobalOptions::optionalDescription(Optionals_e opts_)
{
  static const QString unknown_ = "unknown";
  auto it_ = Optional_m_.find(opts_);
  return it_ != Optional_m_.end() ? it_->second.second : unknown_;
}

// teste ====
std::map<GlobalOptions::eGlobalOptions, QPair<QString, QString>>::const_iterator
GlobalOptions::cbeginMapAll()
{
  return GlobalAll_m_.cbegin();
}

/*!
 * \public
 * \brief cendItMap
 * \return
 */
std::map<GlobalOptions::eGlobalOptions, QPair<QString, QString>>::const_iterator
GlobalOptions::cendMapAll()
{
  return GlobalAll_m_.cend();
}

const std::map<GlobalOptions::eGlobalOptions, QPair<QString, QString>>
  GlobalOptions::GlobalAll_m_ = GlobalOptions::initializeAll();

const std::map<GlobalOptions::eGlobalOptions, QPair<QString, QString>>
GlobalOptions::initializeAll()
{
  return {
    { eGlobalOptions::Critical,
      { "--critical", QT_TR_NOOP("work on log level CRITICAL") } },
    { eGlobalOptions::Error,
      { "--error", QT_TR_NOOP("work on log level ERROR") } },
    { eGlobalOptions::Warning,
      { "--warning", QT_TR_NOOP("work on log level WARNING (default)") } },
    { eGlobalOptions::Info,
      { "--info", QT_TR_NOOP("work on log level INFO") } },
    { eGlobalOptions::Debug,
      { "--debug",
        QT_TR_NOOP("enable debug output, work on log level DEBUG") } },
    { eGlobalOptions::Debug_topic,
      { "--debug-topic",
        QT_TR_NOOP("enable TOPIC debugging (multiple times allowed)") } },
    { eGlobalOptions::Progress,
      { "--progress", QT_TR_NOOP("show progress information") } },
    { eGlobalOptions::Iec,
      { "--iec", QT_TR_NOOP("format using IEC units(1KiB = 1024B)") } },
    { eGlobalOptions::Log_json,
      { "--log-json",
        QT_TR_NOOP("Output one JSON object per log lineinstead of "
                   "formatted text") } },
    { eGlobalOptions::Lock_wait,
      { "--lock-wait",
        QT_TR_NOOP("wait at most SECONDS for acquiring a repository/cache lock "
                   "(default: 1)") } },

    { eGlobalOptions::Bypass_lock,
      { "--bypass-lock", QT_TR_NOOP("Bypass locking mechanism") } },
    { eGlobalOptions::Show_version,
      { "--show-version", QT_TR_NOOP("show/log the borg version") } },
    { eGlobalOptions::Show_rc,
      { "--show-rc", QT_TR_NOOP("show/log the return code (rc)") } },
    { eGlobalOptions::Umask,
      { "--umask", QT_TR_NOOP("set umask to M (local only,default: 0077)") } },
    { eGlobalOptions::Remote_path,
      { "--remote-path",
        QT_TR_NOOP(
          "use PATH as borg executable on theremote (default: \"borg\")") } },

    { eGlobalOptions::Upload_ratelimit,
      { "--upload-ratelimit",
        QT_TR_NOOP("set network upload rate limit in kiByte/s (default: "
                   "0=unlimited)") } },
    { eGlobalOptions::Upload_buffer,
      { "--upload-buffer", QT_TR_NOOP("set network upload buffer size") } },
    { eGlobalOptions::Consider_part_files,
      { "--consider-part-files",
        QT_TR_NOOP("treat part files like normal files (e.g.to "
                   "list/extract them)") } },
    { eGlobalOptions::Debug_profile,
      { "--debug-profile",
        QT_TR_NOOP("Write execution profile in Borg format into FILE.") } },
    { eGlobalOptions::Rsh,
      { "--rsh",
        QT_TR_NOOP("Use this command to connect to the ‘borg serve’ process "
                   "(default: ‘ssh’)") } },
    { eGlobalOptions::None, { "", "" } },
    /* optionals */
    { eGlobalOptions::Dryrun,
      { qMakePair("--dry-run",
                  QT_TR_NOOP("Do not create a backup archive")) } },
    { eGlobalOptions::Stats,
      { qMakePair("--stats",
                  QT_TR_NOOP("Print statistics for the created archive")) } },
    { eGlobalOptions::List,
      { qMakePair("--list",
                  QT_TR_NOOP("Output verbose list of items (files, "
                             "dirs, …)")) } },
    { eGlobalOptions::Filter,
      { qMakePair("--filter",
                  QT_TR_NOOP("Only display items with the given "
                             "status characters (see "
                             "description)")) } },
    { eGlobalOptions::Json,
      { qMakePair("--json",
                  QT_TR_NOOP("Output stats as JSON. Implies --stats.")) } },
    { eGlobalOptions::NoCacheSync,
      { qMakePair("--no-cache-sync",
                  QT_TR_NOOP("Experimental: do not synchronize the "
                             "cache. Implies not "
                             "using the files cache.")) } },
    { eGlobalOptions::StdinName,
      { qMakePair("--stdin-name",
                  QT_TR_NOOP("Use NAME in archive for stdin data "
                             "(default: ‘stdin’)")) } },
    { eGlobalOptions::Stdinuser,
      { qMakePair("--stdin-user",
                  QT_TR_NOOP("Set user USER in archive for stdin "
                             "data (default: ‘root’)")) } },
    { eGlobalOptions::StdinGroup,
      { qMakePair("--stdin-group",
                  QT_TR_NOOP("Set group GROUP in archive for stdin "
                             "data (default: ‘wheel’)")) } },
    { eGlobalOptions::StdinMode,
      { qMakePair("--stdin-mode",
                  QT_TR_NOOP("Set mode to M in archive for stdin "
                             "data (default: 0660)")) } },
    { eGlobalOptions::ContentFromCommand,
      { qMakePair("--content-from-command",
                  QT_TR_NOOP("Interpret PATH as command and store "
                             "its stdout.")) } },
    { eGlobalOptions::PathsFromStdin,
      { qMakePair("--paths-from-stdin",
                  QT_TR_NOOP("Read DELIM-separated list of paths "
                             "to backup from stdin. "
                             "All control is external: it will "
                             "back up all files given - "
                             "no more, no less.")) } },
    { eGlobalOptions::PathsFromCommand,
      { qMakePair("--paths-from-command",
                  QT_TR_NOOP("Interpret PATH as command and treat "
                             "its output as "
                             "--paths-from-stdin")) } },
    { eGlobalOptions::PathsDelimiter,
      { qMakePair("--paths-from-delimiter",
                  QT_TR_NOOP("Set path delimiter for --paths-from-stdin and "
                             "--paths-from-command (default: \n)")) } },
    { eGlobalOptions::Exclude,
      { qMakePair("--exclude",
                  QT_TR_NOOP("Exclude paths matching PATTERN")) } },
    { eGlobalOptions::ExcludeFrom,
      { qMakePair(
        "--exclude-from",
        QT_TR_NOOP("Read exclude patterns from EXCLUDEFILE, one per line")) } },
    { eGlobalOptions::Pattern,
      { qMakePair("--pattern",
                  QT_TR_NOOP("Include/exclude paths matching PATTERN")) } },
    { eGlobalOptions::PatternsFrom,
      { qMakePair(
        "--patterns-from",
        QT_TR_NOOP(
          "Read include/exclude patterns from PATTERNFILE, one per line")) } },
    { eGlobalOptions::ExcludeCaches,
      { qMakePair(
        "--exclude-caches",
        QT_TR_NOOP("Exclude directories that contain a CACHEDIR.TAG file "
                   "(http://www.bford.info/cachedir/spec.html)")) } },
    { eGlobalOptions::ExcludeIfPresent,
      { qMakePair(
        "--exclude-if-present",
        QT_TR_NOOP("Exclude directories that are tagged by containing a "
                   "filesystem object with the given NAME")) } },
    { eGlobalOptions::KeepExcludeTags,
      { qMakePair(
        "--keep-exclude-tags",
        QT_TR_NOOP(
          "If tag objects are specified with --exclude-if-present, don’t omit "
          "the tag objects themselves from the backup archive")) } },
    { eGlobalOptions::ExcludeNoDump,
      { qMakePair("--exclude-nodump",
                  QT_TR_NOOP("Rxclude files flagged NODUMP")) } },
    { eGlobalOptions::OneFileSystem,
      qMakePair(
        "--one-file-system",
        QT_TR_NOOP("Stay in the same file system and do not store mount points "
                   "of other file systems - this might behave different from "
                   "your expectations, see the description below.")) },
    { eGlobalOptions::NumericIds,
      qMakePair("--numeric-ids",
                QT_TR_NOOP("Only store numeric user and group identifiers")) },
    { eGlobalOptions::NoAtime,
      qMakePair("--noatime", QT_TR_NOOP("Do not store atime into archive")) },
    { eGlobalOptions::Atime,
      qMakePair("--atime", QT_TR_NOOP("Do store atime into archive")) },
    { eGlobalOptions::NocTime,
      qMakePair("--noctime", QT_TR_NOOP("Do not store ctime into archive")) },
    { eGlobalOptions::NoBirthtime,
      qMakePair(
        "--nobirthtime",
        QT_TR_NOOP("Do not store birthtime (creation date) into archive")) },
    { eGlobalOptions::NoFlags,
      qMakePair("--noflags",
                QT_TR_NOOP("Do not read and store flags (e.g. NODUMP, "
                           "IMMUTABLE) into archive")) },
    { eGlobalOptions::NoAcls,
      qMakePair("--noacls",
                QT_TR_NOOP("Do not read and store ACLs into archive")) },
    { eGlobalOptions::NoXAttres,
      qMakePair("--noxattrs",
                QT_TR_NOOP("Do not read and store xattrs into archive")) },
    { eGlobalOptions::Sparse,
      qMakePair(
        "--spase",
        QT_TR_NOOP(
          "Detect sparse holes in input (supported only by fixed chunker)")) },
    { eGlobalOptions::FilesCache,
      qMakePair(
        "--files-cache",
        QT_TR_NOOP("Dperate files cache in MODE. default: ctime,size,inode")) },
    { eGlobalOptions::ReadSpecial,
      qMakePair(
        "--read-special",
        QT_TR_NOOP("Open and read block and char device files as well as FIFOs "
                   "as if they were regular files. Also follows symlinks "
                   "pointing to these kinds of files.")) },
    { eGlobalOptions::Comment,
      qMakePair("--comment", QT_TR_NOOP("Add a comment text to the archive")) },
    { eGlobalOptions::Timestamp,
      qMakePair("--timestamp",
                QT_TR_NOOP("Manually specify the archive creation date/time "
                           "(UTC, yyyy-mm-ddThh:mm:ss format). Alternatively, "
                           "give a reference file/directory.")) },
    { eGlobalOptions::CheckpointInterval,
      qMakePair(
        "--checkpoint-interval",
        QT_TR_NOOP("Write checkpoint every SECONDS seconds (Default: 1800)")) },
    { eGlobalOptions::ChunkerParams,
      qMakePair(
        "--chunk-params",
        QT_TR_NOOP("Specify the chunker parameters (ALGO, CHUNK_MIN_EXP, "
                   "CHUNK_MAX_EXP, HASH_MASK_BITS, HASH_WINDOW_SIZE). default: "
                   "buzhash,19,23,21,4095")) },
    { eGlobalOptions::Compression,
      { qMakePair(
        "--compression",
        QT_TR_NOOP(
          "Select compression algorithm, see the output of the “borg help "
          "compression” command for details.")) } }
  }; // return
}
