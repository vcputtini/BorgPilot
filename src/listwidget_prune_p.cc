#include "listwidget_prune_p.h"

/*!
 * \private
 * \brief ListWidget_Prune_P::Prune_m_
 */
using Opt = ListWidget_Prune_P::PruneOptions;
const std::unordered_map<Opt, std::pair<std::string, std::string>>
  ListWidget_Prune_P::Prune_m_ = {
    { ListWidget_Prune_P::PruneOptions::DryRun, { "--dry-run", "" } },
    { ListWidget_Prune_P::PruneOptions::List, { "--list", "" } },
    { ListWidget_Prune_P::PruneOptions::Stats, { "--stats", "" } },
    { ListWidget_Prune_P::PruneOptions::SaveSpace, { "--save-space", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepWithin, { "--keep-within", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepLast, { "--keep-last", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepSecondly,
      { "--keep-secondly", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepMinutely,
      { "--keep-minutely", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepHourly, { "--keep-hourly", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepDaily, { "--keep-daily", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepWeekly, { "--keep-weekly", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepMonthly, { "--keep-monthly", "" } },
    { ListWidget_Prune_P::PruneOptions::Keep13Weekly,
      { "--keep-13weekly", "" } },
    { ListWidget_Prune_P::PruneOptions::Keep3Monthly,
      { "--keep-3monthly", "" } },
    { ListWidget_Prune_P::PruneOptions::KeepYearly, { "--keep-yearly", "" } },
  };

/*!
 * \brief ListWidget_Prune_P::ListWidget_Prune_P
 * \param parent
 */
ListWidget_Prune_P::ListWidget_Prune_P(QWidget* parent)
  : QListWidget(parent)
{
  setSelectionMode(QAbstractItemView::NoSelection);
  populate();
}

/*!
 * \private
 * \brief ListWidget_Prune_P::getSelectedPairs
 * \return
 */
AppTypes::SelectedPairs
ListWidget_Prune_P::getSelectedPairs() const
{
  AppTypes::SelectedPairs result_;

  for (int i = 0; i < count(); ++i) {
    QWidget* container_ = itemWidget(item(i));
    if (!container_)
      continue;

    auto* checkBox_ = container_->findChild<QCheckBox*>();
    auto* lineEdit_ = container_->findChild<QLineEdit*>();

    if (checkBox_ && lineEdit_ && checkBox_->isChecked()) {
      result_.insert(qMakePair(checkBox_->text(), lineEdit_->text()));
    }
  }

  return result_;
}

/*!
 * \public
 * \brief ListWidget_Prune_P::unCheckAllOptions
 */
void
ListWidget_Prune_P::unCheckAllOptions()
{
  for (auto& item_ : findItems("", Qt::MatchContains)) {
    // Retrieve the widget associated with the item (the container)
    QWidget* container_ = itemWidget(item_);

    QCheckBox* checkBox_ = container_->findChild<QCheckBox*>();
    QLineEdit* lineEdit_ = container_->findChild<QLineEdit*>();

    // If both QCheckBox and QLineEdit exist, uncheck QCheckBox and clear
    // QLineEdit
    if (checkBox_) {
      checkBox_->setChecked(false);
    }
    if (lineEdit_) {
      lineEdit_->setText("0");
    }
  }
}

/*!
 * \private slot
 * \brief ListWidget_Prune_P::onCheckboxStateChanged
 */
void
ListWidget_Prune_P::onCheckboxStateChanged()
{
  emit itemsSelectedChanged(getSelectedPairs());
}

/*!
 * \public
 * \brief ListWidget_Prune_P::populate
 */
void
ListWidget_Prune_P::populate()
{
  clear();

  // [_,value_], key isn't used
  for (const auto& [_, value_] : Prune_m_) {
    const QString checkboxText_ = QString::fromStdString(value_.first);
    const QString defaultValue_ = "0";

    auto* item_ = new QListWidgetItem(this);
    item_->setSizeHint(QSize(300, 30));

    auto* container_ = new QWidget(this);
    auto* layout_ = new QHBoxLayout(container_);
    layout_->setContentsMargins(5, 0, 5, 0);

    auto* checkBox_ = new QCheckBox(checkboxText_, container_);
    auto* lineEdit_ = new QLineEdit(defaultValue_, container_);
    if (checkboxText_.contains("--keep")) {
      if (checkboxText_ == Prune_m_.at(PruneOptions::KeepWithin).first) {
        lineEdit_->setInputMask("NNN");
      } else {
        lineEdit_->setInputMask("999");
      }
    } else {
      lineEdit_->setVisible(false);
      lineEdit_->setText("");
    }

    layout_->addWidget(checkBox_);
    layout_->addWidget(lineEdit_);
    container_->setLayout(layout_);

    setItemWidget(item_, container_);

    connect(checkBox_,
            &QCheckBox::checkStateChanged,
            this,
            &ListWidget_Prune_P::onCheckboxStateChanged);
  }
}
