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
#ifndef TYPES_GLOBAL_H
#define TYPES_GLOBAL_H

#include <QDebug>
#include <QList>
#include <QMap>
#include <QMultiHash>
#include <QPair>
#include <QString>

#include <tuple>

// Global Macros
inline bool DebugEnabled = true;
#define DEBUG_MSG_EXTENDED(enabled, msg)                                       \
  if (enabled)                                                                 \
    qDebug().nospace() << "[" << __FILE__ << ":" << __LINE__ << " | "          \
                       << Q_FUNC_INFO << "] " << msg;

#define DEBUG_MSG(enabled, msg)                                                \
  if (enabled)                                                                 \
    qDebug().nospace() << "[" << __LINE__ << " | " << Q_FUNC_INFO << "] "      \
                       << msg;

namespace AppTypes {

// Basic types ----------------------------------------------------------------
constexpr static const int MaxRepositoriesRows = 20;

using VariableName = QString;
using VariableData = QString;

// Complex Types --------------------------------------------------------------

// ---------------------------------------------------------------------------
enum class InitRepositoryFields
{
  Reponame = 0x00,
  Archivename,
  EncryptMode,
  Append,
  MakeParentDir,
  StorageQuota
};
using InitRepositoryTuple =
  std::tuple<QString, QString, int, int, int, QString>;
using InitRepositoryMap = QMultiHash<int, InitRepositoryTuple>;
// ---------------------------------------------------------------------------

using NamedValuePair = QPair<VariableName, VariableData>;
using NamedValueList = QList<NamedValuePair>;

using ResultMap = QMap<const QString, QPair<bool, QString>>;
using RepoistoryCreateDataMap =
  QMultiHash<QString, QPair<QList<QString>, QList<QString>>>;

// Used in: listwidget_delete_p
using SelectedPairs = QSet<QPair<QString, QString>>;

}

#endif // TYPES_GLOBAL_H
