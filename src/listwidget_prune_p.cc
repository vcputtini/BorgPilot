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
    QWidget* container = itemWidget(item(i));
    if (!container)
      continue;

    auto* checkBox = container->findChild<QCheckBox*>();
    auto* lineEdit = container->findChild<QLineEdit*>();

    if (checkBox && lineEdit && checkBox->isChecked()) {
      result_.insert(qMakePair(checkBox->text(), lineEdit->text()));
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
  for (auto& item : findItems("", Qt::MatchContains)) {
    // Retrieve the widget associated with the item (the container)
    QWidget* container = itemWidget(item);

    QCheckBox* checkBox = container->findChild<QCheckBox*>();
    QLineEdit* lineEdit = container->findChild<QLineEdit*>();

    // If both QCheckBox and QLineEdit exist, uncheck QCheckBox and clear
    // QLineEdit
    if (checkBox) {
      checkBox->setChecked(false);
    }
    if (lineEdit) {
      lineEdit->setText("0");
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

  for (const auto& [key, value] : Prune_m_) {
    const QString checkboxText = QString::fromStdString(value.first);
    const QString defaultValue = "0";

    auto* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(300, 30));

    auto* container = new QWidget(this);
    auto* layout = new QHBoxLayout(container);
    layout->setContentsMargins(5, 0, 5, 0);

    auto* checkBox = new QCheckBox(checkboxText, container);
    auto* lineEdit = new QLineEdit(defaultValue, container);
    if (checkboxText.contains("--keep")) {
      lineEdit->setInputMask("999");
    } else {
      lineEdit->setVisible(false);
      lineEdit->setText("");
    }

    layout->addWidget(checkBox);
    layout->addWidget(lineEdit);
    container->setLayout(layout);

    setItemWidget(item, container);

    connect(checkBox,
            &QCheckBox::checkStateChanged,
            this,
            &ListWidget_Prune_P::onCheckboxStateChanged);
  }
}
