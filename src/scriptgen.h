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
#ifndef SCRIPTGEN_H
#define SCRIPTGEN_H

#include <QFile>
#include <QFileDevice>
#include <QIODevice>
#include <QMessageBox>
#include <QMultiHash>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QTextStream>

#include "command2.h"
#include "types_global.h"

/*!
 * \brief The ScriptGen class
 */
class ScriptGen : public QObject
{
  Q_OBJECT
public:
  explicit ScriptGen(const QString& fileName_ = QString(),
                     QObject* parent = nullptr);

  ScriptGen& append(int index_, AppTypes::InitRepositoryTuple data_);

  void setRepositoryPath(const QString& path_) noexcept;

  void sg_skeleton() noexcept;
  void sg_fnc_globals() noexcept;
  void sg_fnc_exports() noexcept;
  void sg_fnc_log() noexcept;
  void sg_fnc_constrols() noexcept;
  void sg_fnc_modules() noexcept;
  void sg_fnc_init_repos(const QString commons_) noexcept;
  // void sg_fnc_create_repos() noexcept;

  // borg [common options] create [options] ARCHIVE [PATH...]
  void extracted(QStringList& parts_);
  void sg_fnc_create(
    AppTypes::RepoistoryCreateDataMap data_map_create_) noexcept;

private:
  QTextStream out_ts_{};
  QFile out_file_{};

  QSettings setting_;

  AppTypes::InitRepositoryMap data_map_;

  QString fname_{};
  QString repoPath_{};

  void fError(const QString& text_);

  Command command_;

signals:
  void fileError(const QString& text_);
};

#endif // SCRIPTGEN_H
