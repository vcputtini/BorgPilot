/***************************************************************************
 * Copyright (c) 2025                                                      *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@gmail.com> | <vcputtini@vcplinux.com.br>
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
#include "listviewrepos_p.h"

#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QString>
#include <QTimer>

// #define DEBUG__

/*!
 * \brief ListViewRepos_P::ListViewRepos_P
 * \param parent
 */
ListViewRepos_P::ListViewRepos_P(QWidget* parent)
  : QListView(parent)
{
}

/*!
 * \brief ListViewRepos_P::setModel
 * \param model_
 */
void
ListViewRepos_P::setModel(QAbstractItemModel* model_)
{
  QListView::setModel(model_);

  // Is selectionModel ready?
  if (selectionModel()) {
    connect(selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &ListViewRepos_P::handleSelectionChanged);
  } else {
    // Ensures that the connection will be made after the event loop
    QTimer::singleShot(0, this, [this]() {
      if (selectionModel()) {
        connect(selectionModel(),
                &QItemSelectionModel::selectionChanged,
                this,
                &ListViewRepos_P::handleSelectionChanged);
      }
    });
  }

  // If there is only one item, it is automatically selected.
  if (model()->rowCount() == 1) {
    QModelIndex index_ = model()->index(0, 0);
    QTimer::singleShot(0, this, [this, index_]() {
      if (!selectionModel()) {
        return;
      }

      selectionModel()->select(index_, QItemSelectionModel::Select);
      selectionModel()->setCurrentIndex(index_, QItemSelectionModel::Current);

      const QString text_ = model()->data(index_, Qt::DisplayRole).toString();
      emit lineSelected(text_);
    });
  }
}

/*!
 * \brief ListViewRepos_P::handleSelectionChanged
 * \param selected_
 * \param deselected_
 */
void
ListViewRepos_P::handleSelectionChanged(const QItemSelection& selected_,
                                        const QItemSelection& deselected_)
{
  if (const QModelIndex index_ = selected_.indexes().first();
      selected_.indexes().isEmpty() || !index_.isValid()) {
    return;
  } else {
    const QString text_ = model()->data(index_, Qt::DisplayRole).toString();
#ifdef DEBUG__
    qDebug() << __FUNCTION__ << " " << text_ << " - "
             << model()->data(index_, Qt::DisplayRole + 1).toString();
    ;
#endif
    emit lineSelected(text_);
  }
}
