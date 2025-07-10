#include "listwidget_delete_p.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>

using Opt = ListWidget_Delete_P::DeleteOptions;

/*!
 * \brief ListWidget_Delete_P::Delete_m_
 */
const std::unordered_map<Opt, std::pair<std::string, std::string>>
  ListWidget_Delete_P::Delete_m_ = {
    { Opt::DryRun, { "--dry-run", "Simula exclusão dos arquivos" } },
    { Opt::List, { "--list", "Lista os arquivos excluídos" } },
    { Opt::Stats, { "--stats", "Exibe estatísticas após exclusão" } },
    { Opt::Force, { "--force", "Força a exclusão" } },
    { Opt::SaveSpace, { "--save-space", "Minimiza uso de espaço em disco" } },
    { Opt::KeepSecurityInfo,
      { "--keep-acls", "Preserva ACLs e atributos de segurança" } }
  };

/*!
 * \brief ListWidget_Delete_P::ListWidget_Delete_P
 * \param parent
 */
ListWidget_Delete_P::ListWidget_Delete_P(QWidget* parent)
  : QListWidget(parent)
{
  setSelectionMode(QAbstractItemView::NoSelection);
  populate();
}

/*!
 * \brief ListWidget_Delete_P::populate
 */
void
ListWidget_Delete_P::populate()
{
  clear();

  for (const auto& [key, value] : Delete_m_) {
    const QString checkboxText = QString::fromStdString(value.first);
    const QString defaultValue = "0";

    auto* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(300, 30));

    auto* container = new QWidget(this);
    auto* layout = new QHBoxLayout(container);
    layout->setContentsMargins(5, 0, 5, 0);

    auto* checkBox = new QCheckBox(checkboxText, container);
    auto* lineEdit = new QLineEdit(defaultValue, container);
    lineEdit->setVisible(false);

    layout->addWidget(checkBox);
    layout->addWidget(lineEdit);
    container->setLayout(layout);

    setItemWidget(item, container);

    connect(checkBox,
            &QCheckBox::checkStateChanged,
            this,
            &ListWidget_Delete_P::onCheckboxStateChanged);
  }
}

/*!
 * \brief ListWidget_Delete_P::unCheckAllOptions
 */
void
ListWidget_Delete_P::unCheckAllOptions()
{
  for (int i = 0; i < count(); ++i) {
    QWidget* container = itemWidget(item(i));
    if (!container)
      continue;

    auto* checkBox = container->findChild<QCheckBox*>();
    if (checkBox) {
      checkBox->setChecked(false);
    }
  }
}

/*!
 * \brief ListWidget_Delete_P::onCheckboxStateChanged
 */
void
ListWidget_Delete_P::onCheckboxStateChanged()
{
  emit itemsSelectedChanged(getSelectedPairs());
}

/*!
 * \brief ListWidget_Delete_P::getSelectedPairs
 * \return
 */
AppTypes::SelectedPairs
ListWidget_Delete_P::getSelectedPairs() const
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
