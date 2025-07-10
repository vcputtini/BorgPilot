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
#ifndef MESSAGESNS_MESSAGES_H
#define MESSAGESNS_MESSAGES_H

#include <QMultiHash>
#include <QObject>
#include <QString>

/*!
 * \namespace MessagesNS
 * \brief General messages used in the program
 */
namespace MessagesNS {

constexpr const char* SYS_INTERNAL_ERROR =
  QT_TR_NOOP(R"(Funtion: %0 Line: %1\nError: %2)");

// -------------
constexpr const char* GEN_WARN_00001 = QT_TR_NOOP(
  "Address Number: Must be filled in the range between 0 and 999999");

// -------------
constexpr const char* HELP_000 = QT_TR_NOOP(
  "Defaults to $HOME or ~$USER or ~ (in that order). If you want to move all "
  "borg-specific folders to a custom path at once, all you need to do is to "
  "modify BORG_BASE_DIR: the other paths for cache, config etc. will adapt "
  "accordingly (assuming you didn’t set them to a different custom value).");
constexpr const char* HELP_001 = QT_TR_NOOP(
  "Defaults to $BORG_BASE_DIR/.cache/borg. If BORG_BASE_DIR is not explicitly "
  "set while XDG env var XDG_CACHE_HOME is set, then $XDG_CACHE_HOME/borg is "
  "being used instead. This directory contains the local cache and might need "
  "a lot of space for dealing with big repositories. Make sure you’re aware of "
  "the associated security aspects of the cache location: Do I need to take "
  "security precautions regarding the cache?");
constexpr const char* HELP_002 = QT_TR_NOOP(
  "Defaults to $BORG_BASE_DIR/.config/borg. If BORG_BASE_DIR is not explicitly "
  "set while XDG env var XDG_CONFIG_HOME is set, then $XDG_CONFIG_HOME/borg is "
  "being used instead. This directory contains all borg configuration "
  "directories, see the FAQ for a security advisory about the data in this "
  "directory: How important is the $HOME/.config/borg directory?");
constexpr const char* HELP_003 = QT_TR_NOOP(
  "Defaults to $BORG_CONFIG_DIR/security. This directory contains information "
  "borg uses to track its usage of NONCES (“numbers used once” - usually in "
  "encryption context) and other security relevant data.");
constexpr const char* HELP_004 =
  QT_TR_NOOP("Defaults to $BORG_CONFIG_DIR/keys. This directory contains keys "
             "for encrypted repositories.");
constexpr const char* HELP_005 = QT_TR_NOOP(
  "When set, use the given path as repository key file. Please note that this "
  "is only for rather "
  "special applications that externally fully manage the key files:"
  "\nthis setting only applies to the keyfile modes (not to the repokey "
  "modes)."
  "\n<br>using a full, absolute path to the key file is recommended."
  "\nall directories in the given path must exist."
  "\nthis setting forces borg to use the key file at the given location."
  "\nthe key file must either exist (for most commands) or will be created "
  "(borg "
  "init)."
  "\nyou need to give a different path for different repositories."
  "\nyou need to point to the correct key file matching the repository the "
  "command will operate "
  "on.");
constexpr const char* HELP_006 =
  QT_TR_NOOP("This is where temporary files are stored (might need a lot of "
             "temporary space for some "
             "operations), see tempfile for details.");

constexpr const char* HELP_GEN_000 = QT_TR_NOOP(
  "When set, use the value to give the default repository location. If a "
  "command needs an archive parameter, you can abbreviate as ::archive. If a "
  "command needs a repository parameter, you can either leave it away or "
  "abbreviate as ::, if a positional parameter is required.");
constexpr const char* HELP_GEN_001 = QT_TR_NOOP(
  "When set, use the value to answer the passphrase question for encrypted "
  "repositories. It is used when a passphrase is needed to access an encrypted "
  "repo as well as when a new passphrase should be initially set when "
  "initializing an encrypted repo. See also BORG_NEW_PASSPHRASE.");
constexpr const char* HELP_GEN_002 = QT_TR_NOOP(
  "When set, use the standard output of the command (trailing newlines are "
  "stripped) to answer the passphrase question for encrypted repositories. It "
  "is used when a passphrase is needed to access an encrypted repo as well as "
  "when a new passphrase should be initially set when initializing an "
  "encrypted repo. Note that the command is executed without a shell. So "
  "variables, like $HOME will work, but ~ won’t. If BORG_PASSPHRASE is also "
  "set, it takes precedence. See also BORG_NEW_PASSPHRASE.");
constexpr const char* HELP_GEN_003 = QT_TR_NOOP(
  "When set, specifies a file descriptor to read a passphrase from. Programs "
  "starting borg may choose to open an anonymous pipe and use it to pass a "
  "passphrase. This is safer than passing via BORG_PASSPHRASE, because on some "
  "systems (e.g. Linux) environment can be examined by other processes. If "
  "BORG_PASSPHRASE or BORG_PASSCOMMAND are also set, they take precedence.");
constexpr const char* HELP_GEN_004 = QT_TR_NOOP(
  "When set, use the value to answer the passphrase question when a new "
  "passphrase is asked for. This variable is checked first. If it is not set, "
  "BORG_PASSPHRASE and BORG_PASSCOMMAND will also be checked. Main usecase for "
  "this is to fully automate borg change-passphrase.");
constexpr const char* HELP_GEN_005 =
  QT_TR_NOOP("When set, use the value to answer the “display the passphrase "
             "for verification” question when defining a new passphrase for "
             "encrypted repositories.");
constexpr const char* HELP_GEN_006 = QT_TR_NOOP(
  "Borg usually computes a host id from the FQDN plus the results of "
  "uuid.getnode() (which usually returns a unique id based on the MAC address "
  "of the network interface. Except if that MAC happens to be all-zero - in "
  "that case it returns a random value, which is not what we want (because it "
  "kills automatic stale lock removal). So, if you have a all-zero MAC address "
  "or other reasons to better externally control the host id, just set this "
  "environment variable to a unique value. If all your FQDNs are unique, you "
  "can just use the FQDN. If not, use fqdn@uniqueid.");
constexpr const char* HELP_GEN_007 = QT_TR_NOOP(
  "When set, use the given filename as INI-style logging configuration. A "
  "basic example conf can be found at docs/misc/logging.conf.");
constexpr const char* HELP_GEN_008 = QT_TR_NOOP(
  "When set, use this command instead of ssh. This can be used to specify ssh "
  "options, such as a custom identity file ssh -i /path/to/private/key. See "
  "man ssh for other options. Using the --rsh CMD commandline option overrides "
  "the environment variable.");
constexpr const char* HELP_GEN_009 =
  QT_TR_NOOP("When set, use the given path as borg executable on the remote "
             "(defaults to “borg” if unset). Using --remote-path PATH "
             "commandline option overrides the environment variable.");
constexpr const char* HELP_GEN_010 =
  QT_TR_NOOP("When set to a value at least one character long, instructs borg "
             "to use a specifically named (based on the suffix) alternative "
             "files cache. This can be used to avoid loading and saving cache "
             "entries for backup sources other than the current sources.");
constexpr const char* HELP_GEN_011 = QT_TR_NOOP(
  "When set to a numeric value, this determines the maximum “time to live” for "
  "the files cache entries (default: 20). The files cache is used to quickly "
  "determine whether a file is unchanged. The FAQ explains this more detailed "
  "in: It always chunks all my files, even unchanged ones!");
constexpr const char* HELP_GEN_012 =
  QT_TR_NOOP("When set to no (default: yes), system information (like OS, "
             "Python version, …) in exceptions is not shown. Please only use "
             "for good reasons as it makes issues harder to analyze.");
constexpr const char* HELP_GEN_013 = QT_TR_NOOP(
  "Choose the lowlevel FUSE implementation borg shall use for borg mount. This "
  "is a comma-separated list of implementation names, they are tried in the "
  "given order, e.g.:"
  "<br>pyfuse3,llfuse: default, first try to load pyfuse3, then try to load "
  "llfuse."
  "<br>llfuse,pyfuse3: first try to load llfuse, then try to load pyfuse3."
  "<br>pyfuse3: only try to load pyfuse3"
  "<br>llfuse: only try to load llfuse"
  "<br>none: do not try to load an implementation");
constexpr const char* HELP_GEN_014 = QT_TR_NOOP(
  "This can be used to influence borg’s builtin self-tests. The default is to "
  "execute the tests at the beginning of each borg command invocation."
  "<br>"
  "BORG_SELFTEST=disabled can be used to switch off the tests and rather save "
  "some time. Disabling is not recommended for normal borg users, but large "
  "scale borg storage providers can use this to optimize production servers "
  "after at least doing a one-time test borg (with selftests not disabled) "
  "when installing or upgrading machines / OS / borg.");
constexpr const char* HELP_GEN_015 =
  QT_TR_NOOP("For “Warning: Attempting to access a previously unknown "
             "unencrypted repository”");
constexpr const char* HELP_GEN_016 = QT_TR_NOOP(
  "For “Warning: The repository at location … was previously located at …”");
constexpr const char* HELP_GEN_017 = QT_TR_NOOP(
  "For “This is a potentially dangerous function…” (check --repair)");
constexpr const char* HELP_GEN_018 =
  QT_TR_NOOP("For “You requested to completely DELETE the repository including "
             "all archives it contains:”");

/*!
 * \brief The Tokens enum
 */
enum class Tokens
{
  SYS_INTERNAL_ERROR = 0x00,
  HELP_000 = 0x64,
  HELP_001,
  HELP_002,
  HELP_003,
  HELP_004,
  HELP_005,
  HELP_006,
  HELP_GEN_000 = 0xc8,
  HELP_GEN_001,
  HELP_GEN_002,
  HELP_GEN_003,
  HELP_GEN_004,
  HELP_GEN_005,
  HELP_GEN_006,
  HELP_GEN_007,
  HELP_GEN_008,
  HELP_GEN_009,
  HELP_GEN_010,
  HELP_GEN_011,
  HELP_GEN_012,
  HELP_GEN_013,
  HELP_GEN_014,
  HELP_GEN_015,
  HELP_GEN_016,
  HELP_GEN_017,
  HELP_GEN_018,
  GEN_WARN_00001 = 0x12c,
  NONE = 0xf423f
};

static constexpr std::initializer_list<Tokens> MessagesList_ = {
  Tokens::SYS_INTERNAL_ERROR, Tokens::HELP_000,     Tokens::HELP_001,
  Tokens::HELP_002,           Tokens::HELP_003,     Tokens::HELP_004,
  Tokens::HELP_005,           Tokens::HELP_006,     Tokens::HELP_GEN_000,
  Tokens::HELP_GEN_001,       Tokens::HELP_GEN_002, Tokens::HELP_GEN_003,
  Tokens::HELP_GEN_004,       Tokens::HELP_GEN_005, Tokens::HELP_GEN_006,
  Tokens::HELP_GEN_007,       Tokens::HELP_GEN_008, Tokens::HELP_GEN_009,
  Tokens::HELP_GEN_010,       Tokens::HELP_GEN_011, Tokens::HELP_GEN_012,
  Tokens::HELP_GEN_013,       Tokens::HELP_GEN_014, Tokens::HELP_GEN_015,
  Tokens::HELP_GEN_016,       Tokens::HELP_GEN_017, Tokens::HELP_GEN_018,
  Tokens::GEN_WARN_00001,     Tokens::NONE
};

/*!
 * \brief The CATEGORY enum
 */
enum class CATEGORY
{
  GENERAL = 0x00,
  HELP = 0x01,
  NONE = 0xff
};

/*!
 * \class
 * \brief The Messages class
 * \note Singleton
 */
class Messages
{
public:
  static Messages& instance();
  Messages(const Messages&) = delete;
  Messages& operator=(const Messages&) = delete;

  Messages& set(Tokens&& token_);
  const QString text() noexcept;

private:
  Messages();
  ~Messages();

  CATEGORY categ_;
  QString msg_;
  QMultiHash<const Tokens, const QString> messages_hash_;

  inline void fillMap();
};

} // namespace MessagesNS

#endif // MESSAGESNS_MESSAGES_H
