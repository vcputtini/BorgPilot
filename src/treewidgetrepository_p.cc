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
#include "treewidgetrepository_p.h"

#include <QFont>
#include <QHeaderView>
#include <QLabel>

/*!
 * \brief TreeWidgetRepository_P::TreeWidgetRepository_P
 * \param parent
 */
TreeWidgetRepository_P::TreeWidgetRepository_P(QWidget* parent)
  : QTreeWidget(parent)
  , buttonGroup_(new QButtonGroup(this))
{
  buttonGroup_->setExclusive(true); // apenas um arquivo selecionado por vez
  setupHeader();
}

/*!
 * \public
 * Cria um item de nível superior com:
 *   col Repository -> repoPath  (negrito)
 *   col Archive    -> ::archive
 *   col Filters    -> Filters: [] First [0  ] [] Last [0  ]
 */
void
TreeWidgetRepository_P::addRepository(const QString& repoPath)
{
  currentRoot_ = new QTreeWidgetItem(this);

  // Coluna 0 — caminho do repositório em negrito para destaque visual
  currentRoot_->setText(static_cast<int>(Column::Repository), repoPath);
  QFont boldFont = currentRoot_->font(static_cast<int>(Column::Repository));
  boldFont.setBold(true);
  currentRoot_->setFont(static_cast<int>(Column::Repository), boldFont);

  // Coluna 1 — vazia no nível raiz
  currentRoot_->setText(static_cast<int>(Column::Archive), QString{});

  addTopLevelItem(currentRoot_);
  currentRoot_->setExpanded(true); // expande por padrão
}

/*!
 *
 * \public
 * Cria um item filho sob currentRoot_ com:
 *   col Repository → vazio (indentação indica hierarquia)
 *   col Archive    → QRadioButton com archiveName
 *
 * Quando o radio é clicado:
 *   • o item filho fica selecionado
 *   • setPattern() é emitido com o padrão Borg montado
 */
void
TreeWidgetRepository_P::addArchive(const QString& archiveName)
{
  // Garantia de segurança: cria raiz default se necessário
  if (!currentRoot_) {
    addRepository(tr("Default"));
  }

  auto* childItem = new QTreeWidgetItem();
  currentRoot_->addChild(childItem);

  // Colunas 0 fica vazia - a hierarquia visual vem da indentação
  childItem->setText(static_cast<int>(Column::Repository), QString{});

  // Coluna 1 - radio button com o nome do arquivo
  auto* radio = new QRadioButton(archiveName);
  setItemWidget(childItem, static_cast<int>(Column::Archive), radio);
  buttonGroup_->addButton(radio);

  // Captura o repoPath do pai no momento da conexão (snapshot seguro)
  const QString repoPath =
    currentRoot_->text(static_cast<int>(Column::Repository));

  connect(radio,
          &QRadioButton::clicked,
          this,
          [this, childItem, repoPath, archiveName]() {
            // Seleciona o item filho na árvore
            setCurrentItem(childItem);
            childItem->setSelected(true);
            emit sig_setPattern(buildPattern(repoPath, archiveName));
          });
}

/*!
 * \public
 * Limpa a árvore e reseta todo o estado interno.
 * Os QRadioButton são removidos do buttonGroup antes de clear() para evitar
 * double-free (o Qt já cuida dos filhos via parent/child, mas o buttonGroup
 * mantém ponteiros extras).
 */
void
TreeWidgetRepository_P::clearTree()
{
  // Remove todos os botões do grupo antes do clear() da árvore
  const QList<QAbstractButton*> buttons = buttonGroup_->buttons();
  for (QAbstractButton* btn : buttons) {
    buttonGroup_->removeButton(btn);
    // NÃO chamamos delete nem deleteLater aqui:
    // os widgets foram passados via setItemWidget() e o Qt os gerencia
  }

  clear(); // remove e deleta todos os QTreeWidgetItems
  currentRoot_ = nullptr;
}

// Helpers

/*!
 * \private
 * Monta o padrão Borg baseado no tipo de repositório:
 *   • Contém '@'   → protocolo SSH
 *   • Caso contrário → repositório local
 */
QString
TreeWidgetRepository_P::buildPattern(const QString& repoPath,
                                     const QString& archiveName)
{
  // qDebug() << repoPath << " -- " << archiveName;
  if (repoPath.contains(u'@')) {
    return QStringLiteral("ssh://%1::%2").arg(repoPath, archiveName);
  }
  return QStringLiteral("%1").arg(archiveName);
}

/*!
 * \private
 * Configura cabeçalhos e políticas visuais da árvore.
 * Centraliza toda a lógica de aparência em um único lugar.
 * \note O comportamento do objeto foi definido na definicao
 *       da UI.
 */
void
TreeWidgetRepository_P::setupHeader()
{
  setColumnCount(static_cast<int>(Column::COUNT));
  setHeaderLabels({ tr("Repostory"), tr("::archive"), tr("Filters") });
  header()->setSectionResizeMode(static_cast<int>(Column::Archive),
                                 QHeaderView::ResizeToContents);
}
