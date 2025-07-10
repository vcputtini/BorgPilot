/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
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
#include "abstracttableview_settings.h"

AbstractTableView_Settings::AbstractTableView_Settings(const int maxRows_,
                                                       const int maxCols_,
                                                       QObject* parent_)
  : QAbstractItemModel(parent_)
  , rows_(maxRows_)
  , cols_(maxCols_)
{
}

QVariant
AbstractTableView_Settings::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    return QString("Column %1").arg(section);
  } else {
    return QString("Row %1").arg(section);
  }
}

QModelIndex
AbstractTableView_Settings::index(int row,
                                  int column,
                                  const QModelIndex& parent) const
{
  // FIXME: Implement me!
  return parent;
}

QModelIndex
AbstractTableView_Settings::parent(const QModelIndex& index) const
{
  // FIXME: Implement me!
  return index;
}

int
AbstractTableView_Settings::rowCount(const QModelIndex& parent) const
{
  if (!parent.isValid()) {
    return 0;
  }

  return rowList_.size();
}

int
AbstractTableView_Settings::columnCount(const QModelIndex& parent) const
{
  if (!parent.isValid()) {
    return 0;
  }

  return rowList_[0].size();
}

QVariant
AbstractTableView_Settings::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  if (role == Qt::DisplayRole) {
    return rowList_[index.row()][index.column()];
  } else {
    return QVariant();
  }
}

bool
AbstractTableView_Settings::setData(const QModelIndex& index,
                                    const QVariant& value,
                                    int role)
{
  if (!index.isValid() || role != Qt::EditRole) {
    qDebug() << __FUNCTION__ << " : "
             << "FALSE";
    return false;
  }
  rowList_[index.row()][index.column()] = value.toString();
  emit dataChanged(index, index);
  return true;
}

bool
AbstractTableView_Settings::insertRows(int row,
                                       int count,
                                       const QModelIndex& parent)
{

  int columns = columnCount();
  beginInsertRows(parent, row, row + rows_ - 1);
  for (int row = 0; row < rows_; ++row) {
    QStringList items;
    for (int column = 0; column < columns; ++column)
      items.append("");
    rowList_.insert(row, items);
  }
  endInsertRows();
  return true;
}

bool
AbstractTableView_Settings::insertColumns(int column,
                                          int count,
                                          const QModelIndex& parent)
{
  int rows = rowCount();
  beginInsertColumns(parent, column, column + cols_ - 1);
  for (int row = 0; row < rows; ++row) {
    for (int c_ = column; column < cols_; ++column) {
      rowList_[row].insert(c_, "");
    }
  }
  endInsertColumns();
  return true;
}
