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
#ifndef CREATEDEFAULTBORGCONFFILE_H
#define CREATEDEFAULTBORGCONFFILE_H

#include <QObject>

/*!
 * \brief The CreateDefaultBorgConfFile class
 * * \note Creates a default qBorgPilot.conf file if it does not exist
 */
class CreateDefaultBorgConfFile : public QObject
{
  Q_OBJECT
public:
  explicit CreateDefaultBorgConfFile(QObject* parent = nullptr);

  void create();

private:
  static constexpr const char* defaultBorgConfig = R"([AutoAnswerers]
[AutoAnswerers]
1\key=BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK
1\value=0
2\key=BORG_RELOCATED_REPO_ACCESS_IS_OK
2\value=1
3\key=BORG_CHECK_I_KNOW_WHAT_I_AM_DOING
3\value=1
4\key=BORG_DELETE_I_KNOW_WHAT_I_AM_DOING
4\value=1
size=4

[DirAndFiles]
1\key=BORG_BASE_DIR
1\value=
2\key=BORG_CACHE_DIR
2\value=/var/cache/borg
3\key=BORG_CONFIG_DIR
3\value=/etc/borg
4\key=BORG_SECURITY_DIR
4\value=/etc/borg/security
5\key=BORG_KEYS_DIR
5\value=/etc/borg/keys
6\key=BORG_KEY_FILE
6\value=/etc/borg/keys
7\key=TMPDIR
7\value=/tmp
size=7

[%General]
1\key=BORG_REPO
1\value=
10\key=BORG_REMOTE_PATH
10\value=
11\key=BORG_FILE_CACHE_SUFFIX
11\value=
12\key=BORG_FILES_CACHE_TTL
12\value=
13\key=BORG_SHOW_SYSINFO
13\value=
14\key=BORG_FUSE_IMPL
14\value=
15\key=BORG_SELFTEST
15\value=
16\key=BORG_WORKAROUNDS
16\value=
2\key=BORG_PASSPHRASE
2\value=
3\key=BORG_PASSCOMMAND
3\value=
4\key=BORG_PASSPHRASE_FD
4\value=
5\key=BORG_NEW_PASSPHRASE
5\value=
6\key=BORG_DISPLAY_PASSPHRASE
6\value=
7\key=BORG_HOST_ID
7\value=
8\key=BORG_LOGGING_CONF
8\value=
9\key=BORG_RSH
9\value=
size=16
)";
};

#endif // CREATEDEFAULTBORGCONFFILE_H
