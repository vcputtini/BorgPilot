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
#ifndef TABLEWIDGET_REPONAMESITEMDELEGATE_H
#define TABLEWIDGET_REPONAMESITEMDELEGATE_H

#include <QAbstractItemModel>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QIntValidator>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QVariant>
#include <QWidget>

/*!
 * \brief The TableWidget_RepoNamesItemDelegate class
 */
class TableWidget_RepoNamesItemDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  TableWidget_RepoNamesItemDelegate(QObject* parent = nullptr);
  ~TableWidget_RepoNamesItemDelegate();

  QWidget* createEditor(QWidget*,
                        const QStyleOptionViewItem&,
                        const QModelIndex&) const;
  void setEditorData(QWidget*, const QModelIndex&) const;
  void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
  void updateEditorGeometry(QWidget*,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const;

  /*bool editorEvent(QEvent* event,
                   QAbstractItemModel* model,
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index);*/

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const;

private:
  enum class Columns
  {
    Name,
    Archive,
    EncMode,
    Append,
    MakeDirPath,
    StoQuota
  };
};

#endif // TABLEWIDGET_REPONAMESITEMDELEGATE_H
