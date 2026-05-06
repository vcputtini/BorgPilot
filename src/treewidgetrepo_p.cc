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
#include "treewidgetrepo_p.h"

#include <QHeaderView>
#include <QLabel>

/*!
 * \brief TreeWidgetRepo_P::TreeWidgetRepo_P
 * \param parent
 */
TreeWidgetRepo_P::TreeWidgetRepo_P(QWidget* parent)
  : QTreeWidget(parent)
  , buttonGroup_(new QButtonGroup(this))
{
  buttonGroup_->setExclusive(true);
}

/*!
 * \public
 * \brief TreeWidgetRepo_P::addRootItem
 * \param rootItemText
 */
void
TreeWidgetRepo_P::addRootItem(const QString& rootItemText)
{
  // Create a visible "root" item
  rootItem_ = new QTreeWidgetItem(this);
  rootItem_->setText(0, rootItemText);
  addTopLevelItem(rootItem_);
}

/*!
 * \brief TreeWidgetRepo_P::addRepoItem
 * \param radioText
 * \param description
 */
void
TreeWidgetRepo_P::addRepoItem(const QString& radioText,
                              const QString& description)
{
  if (!rootItem_) {
    // Extra security: creates default root if addRootItem has not been called
    rootItem_ = new QTreeWidgetItem();
    rootItem_->setText(0, tr("Default"));
    addTopLevelItem(rootItem_);
  }

  QTreeWidgetItem* childItem_ = new QTreeWidgetItem();
  rootItem_->addChild(childItem_);

  QRadioButton* radio_ = new QRadioButton(radioText);
  QLabel* descLabel_ = new QLabel(description);

  setItemWidget(childItem_, 0, radio_);
  setItemWidget(childItem_, 1, descLabel_);

  buttonGroup_->addButton(radio_);

  // Select the item when the radio is clicked
  connect(radio_,
          &QRadioButton::clicked,
          this,
          [this, childItem_, radio_, description]() {
            this->setCurrentItem(childItem_);
            childItem_->setSelected(true);
            if (radio_->text().contains("@")) {
              emit setPattern(QStringLiteral("ssh://%0::%1 --format=\"{NL}\"")
                                .arg(radio_->text(), description));
            } else {
              emit setPattern(QStringLiteral("%0::%1 --format=\"{NL}\"")
                                .arg(radio_->text(), description));
            }
          });
}

/*!
 * \public
 * \brief TreeWidgetRepo_P::clearTree Clean items safely
 */
void
TreeWidgetRepo_P::clearTree()
{
  if (rootItem_) {
    clear();
    // Remove and delete all buttons from the button group
    const auto buttons = buttonGroup_->buttons();
    for (QAbstractButton* btn : buttons) {
      buttonGroup_->removeButton(btn);
      btn->deleteLater(); // Mark for safe removal
    }
    // Reset the rootItem_ pointer (without deleting directly)
    rootItem_ = nullptr;
  }
}
