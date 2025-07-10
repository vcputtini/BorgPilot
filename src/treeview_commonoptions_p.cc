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
#include "treeview_commonoptions_p.h"

/*!
 * \brief TreeView_CommonOptions_P::TreeView_CommonOptions_P
 * \param parent_
 */
TreeView_CommonOptions_P::TreeView_CommonOptions_P(QWidget* parent_)
  : QTreeView(parent_)
  , siModel_(new QStandardItemModel)
{
  InitView();
}

/*!
 * \brief TreeView_CommonOptions_P::~TreeView_CommonOptions_P
 */
TreeView_CommonOptions_P::~TreeView_CommonOptions_P()
{
  delete siModel_;
}

#if 1
/*!
 * \public
 * \brief TreeView_CommonOptions_P::getCheckedItems
 * \return
 */
QByteArray
TreeView_CommonOptions_P::getCheckedItems()
{
  QByteArray checkedItems_ba_{};

  for (int i = 0; i < siModel_->rowCount(); ++i) {
    QStandardItem* item = siModel_->item(i);
    if (item->isCheckable() && item->checkState() == Qt::Checked) {
      checkedItems_ba_.append(i);
    }
  }

  return checkedItems_ba_;
}
#endif

/*!
 * \brief TreeView_CommonOptions_P::updateCheckableItem
 * \param row_
 */
void
TreeView_CommonOptions_P::updateCheckableItem(const QStringList tokens_)
{
  for (int row_ = 0; const auto& it_ : tokens_) {
  GlobalOptions:
    GlobalOptions_e opt_ = getEnum(it_);
    QModelIndex index_ =
      siModel_->index(static_cast<int>(opt_), 0, QModelIndex());
    if (index_.isValid()) {
      QStandardItem* item_ = siModel_->itemFromIndex(index_);
      item_->setCheckState(Qt::Checked);
    }
  }
}

/*!
 * \private
 * \brief TreeView_CommonOptions_P::InitView
 */
void
TreeView_CommonOptions_P::InitView()
{
  siModel_->clear();
  siModel_->setHorizontalHeaderItem(0, new QStandardItem(tr("Choose")));
  siModel_->setHorizontalHeaderItem(1, new QStandardItem(tr("Option")));
  siModel_->setHorizontalHeaderItem(2, new QStandardItem(tr("Value")));
  siModel_->setHorizontalHeaderItem(3, new QStandardItem(tr("Description")));

  int row{ 0 };

  auto commonBeginIt_ = GlobalOptions::cbeginMap();
  auto commonEndIt_ = GlobalOptions::cendMap();

  for (auto&& it_ = commonBeginIt_; it_ != commonEndIt_; ++it_) {
    if (it_->first == GlobalOptions_e::None) {
      continue;
    }
    QStandardItem* blank_ = new QStandardItem("");
    if (it_->first == GlobalOptions_e::Info ||
        it_->first == GlobalOptions_e::Iec ||
        it_->first == GlobalOptions_e::Show_rc) {
      blank_->setData(Qt::Checked,
                      Qt::CheckStateRole); // makes the checkbox appear
    } else {
      blank_->setData(Qt::Unchecked,
                      Qt::CheckStateRole); // makes the checkbox appear
    }
    blank_->setFlags(blank_->flags() | Qt::ItemIsUserCheckable);
    blank_->setEditable(false);
    QStandardItem* option_ =
      new QStandardItem(std::move(globalOpts_.getOptions(it_->first)));
    option_->setEditable(false);
    QStandardItem* value_ = new QStandardItem(std::move(""));
    switch (it_->first) {
      case GlobalOptions_e::Debug_topic:
      case GlobalOptions_e::Lock_wait:
      case GlobalOptions_e::Umask:
      case GlobalOptions_e::Remote_path:
      case GlobalOptions_e::Upload_ratelimit:
      case GlobalOptions_e::Upload_buffer:
      case GlobalOptions_e::Debug_profile: {
        value_->setEditable(true);
        break;
      }
      default: {
        value_->setEditable(false);
      }
    }

    QStandardItem* help_ = new QStandardItem(std::move(it_->second.second));
    help_->setEditable(false);
    siModel_->appendRow(QList<QStandardItem*>()
                        << blank_ << option_ << value_ << help_);
    ++row;
  }

  setModel(siModel_);
}
