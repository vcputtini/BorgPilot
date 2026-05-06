/***************************************************************************
 * Copyright (c) 2025-2026                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@vcplinux.com.br>                                        *
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
#include "listwidget_delete_p.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>

using Opt = ListWidget_Delete_P::DeleteOptions;

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

ListWidget_Delete_P::ListWidget_Delete_P(QWidget* parent)
  : QListWidget(parent)
{
  setSelectionMode(QAbstractItemView::NoSelection);
  populate();
}

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
            &ListWidget_Delete_P::sl_checkboxStateChanged);
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

void
ListWidget_Delete_P::sl_checkboxStateChanged()
{
  emit sig_itemsSelectedChanged(getSelectedPairs());
}

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
