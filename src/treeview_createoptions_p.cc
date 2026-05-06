/***************************************************************************
 * Copyright (c) 2023-2024                                                 *
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
#include "treeview_createoptions_p.h"

/*!
 * \brief TreeView_CreateOptions_P::TreeView_CreateOptions_P
 * \param parent_
 */
TreeView_CreateOptions_P::TreeView_CreateOptions_P(QWidget* parent_)
  : QTreeView(parent_)
  , siModel_(new QStandardItemModel)
{
  InitView();
}

/*!
 * \brief TreeView_CreateOptions_P::~TreeView_CreateOptions_P
 */
TreeView_CreateOptions_P::~TreeView_CreateOptions_P()
{
  delete siModel_;
}

void
TreeView_CreateOptions_P::InitView()
{
  return;

  siModel_->clear();
  siModel_->setHorizontalHeaderItem(0, new QStandardItem(tr("Choose")));
  siModel_->setHorizontalHeaderItem(1, new QStandardItem(tr("Option")));
  siModel_->setHorizontalHeaderItem(2, new QStandardItem(tr("Value")));
  siModel_->setHorizontalHeaderItem(3, new QStandardItem(tr("Description")));

  int row_{ 0 };
  for (auto&& it_ = cbeginMapOptional(); it_ != cendMapOptional(); ++it_) {
    qDebug() << it_->second.first;

    QStandardItem* blank_ = new QStandardItem("");
    blank_->setData(Qt::Unchecked,
                    Qt::CheckStateRole); // makes the checkbox appear
    blank_->setFlags(blank_->flags() | Qt::ItemIsUserCheckable);
    blank_->setEditable(false);

    QStandardItem* option_ =
      new QStandardItem(std::move(createOpts_.getOptionals(it_->first)));
    option_->setEditable(false);
    QStandardItem* value_ = new QStandardItem(std::move(""));

    QStandardItem* help_ = new QStandardItem(std::move(it_->second.first));
    help_->setEditable(false);
    siModel_->appendRow(QList<QStandardItem*>()
                        << std::move(blank_) << std::move(option_)
                        << std::move(value_) << std::move(help_));

    ++row_;
  }
  setModel(siModel_);
}

#if 0
/*!
 * \brief TreeView_CreateOptions_P::InitView
 */
void
TreeView_CreateOptions_P::InitView()
{
  siModel_->clear();
  siModel_->setHorizontalHeaderItem(0, new QStandardItem(tr("Choose")));
  siModel_->setHorizontalHeaderItem(1, new QStandardItem(tr("Option")));
  siModel_->setHorizontalHeaderItem(2, new QStandardItem(tr("Value")));
  siModel_->setHorizontalHeaderItem(3, new QStandardItem(tr("Description")));

  int row{ 0 };
  for (auto&& it_ = CreateOptions_m_.cbegin(); it_ != CreateOptions_m_.cend();
       ++it_) {
    QStandardItem* blank_ = new QStandardItem("");
    blank_->setData(Qt::Unchecked,
                    Qt::CheckStateRole); // makes the checkbox appear
    blank_->setFlags(blank_->flags() | Qt::ItemIsUserCheckable);
    blank_->setEditable(false);
    QStandardItem* option_ =
      new QStandardItem(std::move(createOpts_.get(it_.key())));
    option_->setEditable(false);
    QStandardItem* value_ = new QStandardItem(std::move(""));
    switch (it_.key()) {
      case CreateOptions_e::Filter:
      case CreateOptions_e::Stdin_name:
      case CreateOptions_e::Stdin_user:
      case CreateOptions_e::Stdin_group:
      case CreateOptions_e::Stdin_mode:
      case CreateOptions_e::Paths_delimiter:
      case CreateOptions_e::Compression: {
        value_->setEditable(true);
        break;
      }
      default: {
        value_->setEditable(false);
      }
    }

    QStandardItem* help_ = new QStandardItem(std::move(it_.value().second));
    help_->setEditable(false);
    siModel_->appendRow(QList<QStandardItem*>()
                        << std::move(blank_) << std::move(option_)
                        << std::move(value_) << std::move(help_));
    ++row;
  }

  setModel(siModel_);
}

#endif
