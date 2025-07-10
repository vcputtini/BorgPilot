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
#include "abstracttablemodel_settings_p.h"

/*!
 * \brief AbstractTableModel_Settings_P::AbstractTableModel_Settings_P
 * \param maxRows_
 * \param maxCols_
 * \param parent
 */
AbstractTableModel_Settings_P::AbstractTableModel_Settings_P(const int maxRows_,
                                                             const int maxCols_,
                                                             QObject* parent)
  : QAbstractTableModel(parent)
  , mRows_(maxRows_)
  , mCols_(maxCols_)
  , rowList_{}
  , bkpObject(parent)
{
  QStringList newList_{};
  for (int column = 0; column < qMax(1, maxCols_); ++column) {
    newList_.append("");
  }
  for (int row = 0; row < qMax(1, maxRows_); ++row) {
    rowList_.append(newList_);
  }
}

/*!
 * \brief AbstractTableModel_Settings_P::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant
AbstractTableModel_Settings_P::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (bkpObject->objectName() == "DialogFindReqExternalTools") {
    if (orientation == Qt::Horizontal) {
      switch (section) {
        case 0: {
          return tr("Command");
          break;
        }
        case 1: {
          return tr("Status");
          break;
        }
        case 2: {
          return tr("Path");
          break;
        }
      }
    }
  } else {
    if (orientation == Qt::Horizontal) {
      switch (section) {
        case 0: {
          return tr("Variável");
          break;
        }
        case 1: {
          return tr("Valor");
          break;
        }
      }
    } else {
      return QString("%0").arg(section);
    }
  }

  return QAbstractItemModel::headerData(section, orientation, role);
}

/*!
 * \brief AbstractTableModel_Settings_P::setHeaderData
 * \param section
 * \param orientation
 * \param value
 * \param role
 * \return
 */
bool
AbstractTableModel_Settings_P::setHeaderData(int section,
                                             Qt::Orientation orientation,
                                             const QVariant& value,
                                             int role)
{
  if (value != headerData(section, orientation, role)) {
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

/*!
 * \brief AbstractTableModel_Settings_P::rowCount
 * \param parent
 * \return
 */
int
AbstractTableModel_Settings_P::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    return 0;
  }
  return rowList_.size();
}

/*!
 * \brief AbstractTableModel_Settings_P::columnCount
 * \param parent
 * \return
 */
int
AbstractTableModel_Settings_P::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return rowList_[0].size();
}

/*!
 * \brief AbstractTableModel_Settings_P::data
 * \param index
 * \param role
 * \return
 */
QVariant
AbstractTableModel_Settings_P::data(const QModelIndex& index, int role) const
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

/*!
 * \brief AbstractTableModel_Settings_P::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool
AbstractTableModel_Settings_P::setData(const QModelIndex& index,
                                       const QVariant& value,
                                       int role)
{
  if (!index.isValid() || role != Qt::EditRole) {
    return false;
  }
  rowList_[index.row()][index.column()] = value.toString();
  emit dataChanged(index, index);
  return true;
}

/*!
 * \brief AbstractTableModel_Settings_P::flags
 * \param index
 * \return
 */
Qt::ItemFlags
AbstractTableModel_Settings_P::flags(const QModelIndex& index) const
{
  if (!index.isValid()) {
    return Qt::ItemIsEnabled;
  }
  if (index.column() == 0) {
    return Qt::ItemIsEnabled; // not editable
  }
  if ((bkpObject->objectName() == "DialogFindReqExternalTools") &&
      (index.column() >= 1)) {
    return Qt::ItemIsEnabled; // not editable
  }

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
