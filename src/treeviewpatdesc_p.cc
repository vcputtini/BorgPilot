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
#include "treeviewpatdesc_p.h"

const QHash<TreeViewPatDesc_P::PLACEHOLDER, TreeViewPatDesc_P::DESCRIPTION>
  TreeViewPatDesc_P::data_ = {
    { QStringLiteral("type"),
      QStringLiteral("File type (file, dir, symlink, etc.)") },
    { QStringLiteral("mode"),
      QStringLiteral("File permissions (e.g., -rw-r--r--)") },
    { QStringLiteral("uid"), QStringLiteral("Owner user ID") },
    { QStringLiteral("gid"), QStringLiteral("Owner group ID") },
    { QStringLiteral("user"), QStringLiteral("Owner user name") },
    { QStringLiteral("group"), QStringLiteral("Owner group name") },
    { QStringLiteral("path"), QStringLiteral("Full file path") },
    { QStringLiteral("target"), QStringLiteral("Symlink target") },
    { QStringLiteral("hlid"), QStringLiteral("Hard link identity") },
    { QStringLiteral("flags"), QStringLiteral("File flags (e.g., sparse)") },
    { QStringLiteral("size"), QStringLiteral("File size in bytes") },
    { QStringLiteral("num_chunks"), QStringLiteral("Number of data chunks") },
    { QStringLiteral("mtime"), QStringLiteral("Last modification time") },
    { QStringLiteral("ctime"), QStringLiteral("Last metadata change time") },
    { QStringLiteral("atime"), QStringLiteral("Last access time") },
    { QStringLiteral("isomtime"),
      QStringLiteral("ISO 8601 formatted modification time") },
    { QStringLiteral("isoctime"),
      QStringLiteral("ISO 8601 formatted metadata change time") },
    { QStringLiteral("isoatime"),
      QStringLiteral("ISO 8601 formatted access time") },
    { QStringLiteral("blake2b"),
      QStringLiteral("BLAKE2b hash of the content") },
    { QStringLiteral("sha256"), QStringLiteral("SHA-256 hash of the content") },
    { QStringLiteral("md5"), QStringLiteral("MD5 hash of the content") },
    { QStringLiteral("xxh64"),
      QStringLiteral("XXH64 checksum of the content") },
    { QStringLiteral("archiveid"), QStringLiteral("Internal archive ID") },
    { QStringLiteral("archivename"), QStringLiteral("Name of the archive") },
    { QStringLiteral("extra"),
      QStringLiteral("Extra info (e.g., symlink arrow ->)") },
    { QStringLiteral("health"),
      QStringLiteral("Item status: 'healthy' or 'broken'") },
    { QStringLiteral("archive"), QStringLiteral("Archive name (textual)") },
    { QStringLiteral("barchive"),
      QStringLiteral("Raw archive name (may contain binary)") },
    { QStringLiteral("comment"), QStringLiteral("Textual archive comment") },
    { QStringLiteral("bcomment"), QStringLiteral("Raw archive comment") },
    { QStringLiteral("id"), QStringLiteral("Archive internal ID") },
    { QStringLiteral("tam"),
      QStringLiteral("Trusted Archive Mode (TAM) status") },
    { QStringLiteral("start"), QStringLiteral("Backup creation start time") },
    { QStringLiteral("end"), QStringLiteral("Backup creation end time") },
    { QStringLiteral("command_line"),
      QStringLiteral("Command used to create the backup") },
    { QStringLiteral("hostname"),
      QStringLiteral("Host where the backup was created") },
    { QStringLiteral("username"),
      QStringLiteral("User who created the backup") },
    { QStringLiteral("NEWLINE"),
      QStringLiteral("Newline character (OS-dependent)") },
    { QStringLiteral("TAB"), QStringLiteral("Tab character") },
    { QStringLiteral("SPACE"), QStringLiteral("Single space character") },
    { QStringLiteral("CR"), QStringLiteral("Carriage return character") },
    { QStringLiteral("LF"), QStringLiteral("Line feed character") },
    { QStringLiteral("NUL"), QStringLiteral("Null character (\\0)") },
  };

/*!
 * \brief TreeViewPatDesc_P::TreeViewPatDesc_P
 * \param parent
 */
TreeViewPatDesc_P::TreeViewPatDesc_P(QWidget* parent)
  : QTreeView(parent)
{
}

/*!
 * \brief TreeViewPatDesc_P::populate
 */
void
TreeViewPatDesc_P::populate()
{
  model_ = createModelFromPlaceholders();
  this->setModel(model_.get());
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  this->setUniformRowHeights(true);
  this->setSelectionMode(QAbstractItemView::SingleSelection);

  // Disables hierarchical view
  this->setHeaderHidden(false); // Show headers
  this->expandAll();
  this->setColumnWidth(180, 300);
}

/*!
 * \brief TreeViewPatDesc_P::createModelFromPlaceholders
 * \return
 */
std::unique_ptr<QStandardItemModel>
TreeViewPatDesc_P::createModelFromPlaceholders()
{
  auto model = std::make_unique<QStandardItemModel>();

  model->setHorizontalHeaderLabels({ QStringLiteral("--format={Placeholder}"),
                                     QStringLiteral("Description") });

  for (auto it = data_.constBegin(); it != data_.constEnd(); ++it) {
    auto* keyItem = new QStandardItem(it.key());
    auto* descItem = new QStandardItem(it.value());

    // Doesn't allow editing
    keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
    descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable);

    model->appendRow({ keyItem, descItem });
  }

  return model;
}
