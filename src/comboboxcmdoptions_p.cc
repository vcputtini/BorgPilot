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
#include "comboboxcmdoptions_p.h"
#include <QDebug>
#include <qlineedit.h>

/*!
 * \brief ComboBoxCmdOptions_P::ComboBoxCmdOptions_P
 * \param parent
 */
ComboBoxCmdOptions_P::ComboBoxCmdOptions_P(QWidget* parent)
  : QComboBox(parent)
{
  connect(this,
          &QComboBox::currentIndexChanged,
          this,
          &ComboBoxCmdOptions_P::handleSelectionChanged);
}

// -----------------------------------------------------------------------

/*!
 * \brief ComboBoxCmdOptions_P::uncheckAllItems
 * \param cb_
 */
void
ComboBoxCmdOptions_P::uncheckAllItems(QComboBox* cb_)
{
  if (!cb_) {
    return;
  }

  auto* model = qobject_cast<QStandardItemModel*>(cb_->model());
  if (!model)
    return;

  for (int i = 0; i < model->rowCount(); ++i) {
    QStandardItem* item = model->item(i);
    if (item && item->checkState() == Qt::Checked) {
      item->setCheckState(Qt::Unchecked);
    }
  }
}
// -----------------------------------------------------------------------

/*!
 * \brief ComboBoxCmdOptions_P::populateComboBox
 * \param cb_
 * \param mode_
 */
template<typename CBoxType>
void
ComboBoxCmdOptions_P::populateComboBox(CBoxType& cb_, const QString& mode_)
{

  cb_->clear();

  auto* model_ = new QStandardItemModel(cb_);
  cb_->setModel(model_);

  if (mode_ == "delete") {
    for (const auto& pair : Delete_m_) {
      const std::string& optStd_ = std::move(pair.second.first);
      const QString opt_ = std::move(QString::fromStdString(optStd_));

      QStandardItem* item_ = new QStandardItem(std::move(opt_));
      item_->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      item_->setData(Qt::Unchecked, Qt::CheckStateRole);

      model_->appendRow(item_);
    }
  } else if (mode_ == "prune") {
    for (const auto& pair : Prune_m_) {
      const std::string& labelStd_ = std::move(pair.second.first);
      const QString label_ = std::move(QString::fromStdString(labelStd_));

      QStandardItem* item_ = new QStandardItem(std::move(label_));
      item_->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      item_->setData(Qt::Unchecked, Qt::CheckStateRole);

      model_->appendRow(item_);
    }
  }

  // Conecte sinal de mudança no modelo
  connect(
    model_, &QStandardItemModel::itemChanged, this, [=, this](QStandardItem*) {
      QStringList checkedItems;

      for (int i = 0; i < model_->rowCount(); ++i) {
        QStandardItem* item_ = model_->item(i);
        if (item_->checkState() == Qt::Checked) {
          checkedItems << std::move(item_->text());
        }
      }

      emit optionsChanged(checkedItems.join(" "));
    });
};
// -----------------------------------------------------------------------

/*!
 * \brief ComboBoxCmdOptions_P::setOption
 * \param option_
 */
void
ComboBoxCmdOptions_P::setOption(const QString& option_)
{
  clear();

  ComboBoxCmdOptions_P* combo = this;
  populateComboBox(combo, option_);
}

/*!
 * \brief ComboBoxCmdOptions_P::handleSelectionChanged
 * \param index
 */
void
ComboBoxCmdOptions_P::handleSelectionChanged(int index)
{
  if (index >= 0) {
    emit optionSelected(itemText(index));
  }
}

/*!
 * \brief ComboBoxCmdOptions_P::setModel
 * \param model_
 */
void
ComboBoxCmdOptions_P::setModel(QAbstractItemModel* model_)
{
  QComboBox::setModel(model_);
}

/* Definitions */
/*!
 * \group
 * \brief ComboBoxCmdOptions_P::initializeDeleteMap
 * \return
 */
const std::unordered_map<ComboBoxCmdOptions_P::DeleteOptions,
                         std::pair<std::string, std::string>>
ComboBoxCmdOptions_P::initializeDeleteMap()
{
  return {
    { ComboBoxCmdOptions_P::DeleteOptions::DryRun, { "--dry-run", "" } },
    { ComboBoxCmdOptions_P::DeleteOptions::List, { "--list", "" } },
    { ComboBoxCmdOptions_P::DeleteOptions::Stats, { "--stats", "" } },
    { ComboBoxCmdOptions_P::DeleteOptions::Force, { "--force", "" } },
    { ComboBoxCmdOptions_P::DeleteOptions::SaveSpace, { "--save-space", "" } },
    { ComboBoxCmdOptions_P::DeleteOptions::KeepSecurityInfo,
      { "--keep-security-info", "" } },
  };
}

const std::unordered_map<ComboBoxCmdOptions_P::PruneOptions,
                         std::pair<std::string, std::string>>
ComboBoxCmdOptions_P::initializePruneMap()
{
  return {
    { ComboBoxCmdOptions_P::PruneOptions::DryRun, { "--dry-run", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::List, { "--list", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::Stats, { "--stats", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::SaveSpace, { "--save-space", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepWithin, { "--keep-within", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepLast, { "--keep-last", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepSecondly,
      { "--keep-secondly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepMinutely,
      { "--keep-minutely", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepHourly, { "--keep-hourly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepDaily, { "--keep-daily", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepWeekly, { "--keep-weekly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepMonthly,
      { "--keep-monthly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::Keep13Weekly,
      { "--keep-13weekly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::Keep3Monthly,
      { "--keep-3monthly", "" } },
    { ComboBoxCmdOptions_P::PruneOptions::KeepYearly, { "--keep-yearly", "" } },
  };
}

/*!
 * \brief Static members initialization
 */
/*!
 * \group
 * \brief initializeDeleteMap
 * \return
 */
static const std::unordered_map<ComboBoxCmdOptions_P::DeleteOptions,
                                std::pair<std::string, std::string>>
initializeDeleteMap();
static const std::unordered_map<ComboBoxCmdOptions_P::PruneOptions,
                                std::pair<std::string, std::string>>
initializePruneMap();

const std::unordered_map<ComboBoxCmdOptions_P::DeleteOptions,
                         std::pair<std::string, std::string>>
  ComboBoxCmdOptions_P::Delete_m_ = ComboBoxCmdOptions_P::initializeDeleteMap();
const std::unordered_map<ComboBoxCmdOptions_P::PruneOptions,
                         std::pair<std::string, std::string>>
  ComboBoxCmdOptions_P::Prune_m_ = ComboBoxCmdOptions_P::initializePruneMap();
