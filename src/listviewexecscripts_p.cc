/***************************************************************************
 * Copyright (c) 2025-2026                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@vcplinux.com.br>                                        *
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
#include "listviewexecscripts_p.h"

#include "progid.h"
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QSettings>
#include <QTimer>
#include <utility>

ListviewExecScripts_P::ListviewExecScripts_P(QWidget* parent)
  : QListView(parent)
  , m_model(new QStringListModel(this))
{
  this->setModel(m_model);
  setupConnections();

  // Initial load
  QTimer::singleShot(0, this, &ListviewExecScripts_P::refresh);
}

void
ListviewExecScripts_P::setupConnections()
{
  if (selectionModel()) {
    connect(selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &ListviewExecScripts_P::sl_handleSelectionChanged);
  }
}

void
ListviewExecScripts_P::refresh()
{
  populate();
}

void
ListviewExecScripts_P::populate()
{
  QStringList scripts;
  const QSettings settings(ProgId::strOrganization(),
                           ProgId::strInternalName());

  const QStringList groups = settings.childGroups();

  for (const QString& group : std::as_const(groups)) {
    // Ensuring that the literal is treated as a QString
    if (group.contains(QStringLiteral("INITREPO_"))) {
      scripts << group.section(QStringLiteral("INITREPO_"), 1);
    }
  }

  m_model->setStringList(scripts);

  if (m_model->rowCount() == 1) {
    const QModelIndex index = m_model->index(0, 0);

    QTimer::singleShot(0, this, [this, index]() {
      if (selectionModel() && index.isValid()) {
        // Using the correct selection flags instead of QModelIndex()
        selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::Current);

        const QString text = m_model->data(index, Qt::DisplayRole).toString();
        emit sig_lineSelected(text);
      }
    });
  }
}

void
ListviewExecScripts_P::sl_handleSelectionChanged(
  const QItemSelection& selected,
  const QItemSelection& /*deselected*/)
{
  if (selected.indexes().isEmpty()) {
    return;
  }

  const QModelIndex index = selected.indexes().first();
  if (index.isValid()) {
    const QString text = m_model->data(index, Qt::DisplayRole).toString();
    emit sig_lineSelected(text);
  }
}
