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
#include "dialogfindreqexternaltools.h"
#include "ui_dialogfindreqexternaltools.h"

#include "findreqexternaltools.h"
#include "types_global.h"

/*!
 * \brief DialogFindReqExternalTools::DialogFindReqExternalTools
 * \param parent
 */
DialogFindReqExternalTools::DialogFindReqExternalTools(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::DialogFindReqExternalTools)
{
  ui->setupUi(this);

  connect(ui->pushButton_Ok,
          &QPushButton::clicked,
          this,
          [this](bool checked_) { close(); });

  FindReqExternalTools find_;
  AppTypes::ResultMap res_ = find_.exec();
  if (!find_.isValid()) {
    ui->label_Msg->setText(tr("There are command tools not found. Please "
                              "install it before proceeding"));
  } else {
    ui->label_Msg->setText(tr("Conditions satisfied for using the program"));
  }

  abstractTableModel_Settings_P =
    new AbstractTableModel_Settings_P(res_.size(), 3, this);
  ui->tableView_Results->setModel(abstractTableModel_Settings_P);

  int row{ 0 };
  for (auto it_ = res_.cbegin(); it_ != res_.cend(); ++it_) {
    QModelIndex index_varname_ =
      abstractTableModel_Settings_P->index(row, 0, QModelIndex());
    abstractTableModel_Settings_P->setData(index_varname_,
                                           std::move(QVariant((it_.key()))));
    QModelIndex index_data_ =
      abstractTableModel_Settings_P->index(row, 1, QModelIndex());
    abstractTableModel_Settings_P->setData(
      index_data_,
      QVariant((it_.value().first ? tr("Installed") : tr("Not Installed"))));
    QModelIndex index_path_ =
      abstractTableModel_Settings_P->index(row, 2, QModelIndex());
    abstractTableModel_Settings_P->setData(
      index_path_, std::move(QVariant(it_.value().second)));
    ++row;
  }
}

/*!
 * \brief DialogFindReqExternalTools::~DialogFindReqExternalTools
 */
DialogFindReqExternalTools::~DialogFindReqExternalTools()
{
  delete abstractTableModel_Settings_P;
  delete ui;
}
