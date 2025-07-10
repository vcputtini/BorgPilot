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
#include "tableview_dirandfiles_p.h"

#include "settingshandler.h"
#include "types_global.h"

/*!
 * \brief TableView_DirAndFiles_P::TableView_DirAndFiles_P
 * \param parent_
 */
TableView_DirAndFiles_P::TableView_DirAndFiles_P(QWidget* parent_)
  : QTableView(parent_)
{
  Messages_ = &Messages_->instance();
  setupTable();
}

/*!
 * \brief TableView_DirAndFiles_P::~TableView_DirAndFiles_P
 * \param parent_
 */
TableView_DirAndFiles_P::~TableView_DirAndFiles_P()
{
  delete abstractTableModel_Settings_P;
}

/*!
 * \brief TableView_DirAndFiles_P::setupTable
 */
void
TableView_DirAndFiles_P::setupTable()
{
  setSortingEnabled(false);
  SettingsHandler shandler_;

  abstractTableModel_Settings_P =
    new AbstractTableModel_Settings_P(varsList_.size(), 2, this);
  setModel(abstractTableModel_Settings_P);

  AppTypes::NamedValueList values_ = shandler_.readDirAndFiles();

  if (values_.empty()) {
    return;
  }

  const int rowCount_ =
    std::min(shandler_.EnvVars_DF_m_.size(), values_.size());

  for (int row_ = 0; row_ < rowCount_; ++row_) {
    const QString& varName_ = values_.at(row_).first;
    const QString& varData_ = values_.at(row_).second;

    QModelIndex index_varname_ = abstractTableModel_Settings_P->index(row_, 0);
    abstractTableModel_Settings_P->setData(index_varname_, varName_);

    QModelIndex index_data_ = abstractTableModel_Settings_P->index(row_, 1);
    abstractTableModel_Settings_P->setData(index_data_, varData_);

    setColumnWidth(row_, 250);
  }
}
