/***************************************************************************
 * Copyright (c) 2025                                                *
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
#include "tablewidget_reponamesitemdelegate.h"

/*!
 * \brief TableWidget_RepoNamesItemDelegate::TableWidget_RepoNamesItemDelegate
 * \param parent
 */
TableWidget_RepoNamesItemDelegate::TableWidget_RepoNamesItemDelegate(
  QObject* parent)
  : QItemDelegate(parent)
{
}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::~TableWidget_RepoNamesItemDelegate
 */
TableWidget_RepoNamesItemDelegate::~TableWidget_RepoNamesItemDelegate() {}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::createEditor
 * \param parent
 * \param view
 * \param index
 * \return
 */
QWidget*
TableWidget_RepoNamesItemDelegate::createEditor(
  QWidget* parent,
  const QStyleOptionViewItem& view,
  const QModelIndex& index) const
{
  QWidget* w_;

  switch (index.column()) {
    case static_cast<int>(Columns::Name): {
      QLineEdit* editor_ = new QLineEdit(parent);
      editor_->setMaxLength(50);
      w_ = editor_;
      break;
    }
    case static_cast<int>(Columns::Archive): {
      QLineEdit* editor_ = new QLineEdit(parent);
      editor_->setMaxLength(254);
      w_ = editor_;
      break;
    }
    case static_cast<int>(Columns::EncMode): {
      QComboBox* editor_ = new QComboBox(parent);
      editor_->addItems(QStringList() << "none"
                                      << "repokey"
                                      << "repokey-blake2");
      w_ = editor_;
      break;
    }
    case static_cast<int>(Columns::Append): {
      QCheckBox* editor_ = new QCheckBox(parent);
      w_ = editor_;
      break;
    }
    case static_cast<int>(Columns::MakeDirPath): {
      QCheckBox* editor_ = new QCheckBox(parent);
      w_ = editor_;
      break;
    }
    case static_cast<int>(Columns::StoQuota): {
      QLineEdit* editor_ = new QLineEdit(parent);
      // editor_->setValidator(new QIntValidator(0, 32767));
      editor_->setMaxLength(10);
      w_ = editor_;
      break;
    }
    default: {
      w_ = new QLineEdit(parent);
    }
  }

  w_->installEventFilter(const_cast<TableWidget_RepoNamesItemDelegate*>(this));
  return w_;
}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::setEditorData
 * \param editorWidget
 * \param index
 */
void
TableWidget_RepoNamesItemDelegate::setEditorData(QWidget* editorWidget,
                                                 const QModelIndex& index) const
{
  const QVariant indexValue_ = index.model()->data(index, Qt::EditRole);

  switch (index.column()) {
    case static_cast<int>(Columns::Name): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      editor_->setAlignment(Qt::AlignRight);
      editor_->setMaxLength(50);
      editor_->setText(indexValue_.toString());
      break;
    }
    case static_cast<int>(Columns::Archive): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      editor_->setAlignment(Qt::AlignRight);
      editor_->setMaxLength(254);
      editor_->setText(indexValue_.toString());
      break;
    }
    case static_cast<int>(Columns::EncMode): {
      QComboBox* editor_ = dynamic_cast<QComboBox*>(editorWidget);
      editor_->setCurrentText(indexValue_.toString());
      break;
    }
    case static_cast<int>(Columns::Append): {
      QCheckBox* editor_ = dynamic_cast<QCheckBox*>(editorWidget);
      // qDebug() << __FUNCTION__ << " append " << indexValue_.toBool();
      editor_->setCheckState(indexValue_ == 1 ? Qt::Checked : Qt::Unchecked);
      break;
    }
    case static_cast<int>(Columns::MakeDirPath): {
      // qDebug() << __FUNCTION__ << " MDP " << indexValue_.toBool();
      QCheckBox* editor_ = dynamic_cast<QCheckBox*>(editorWidget);
      editor_->setCheckState(indexValue_ == 1 ? Qt::Checked : Qt::Unchecked);
      break;
    }
    case static_cast<int>(Columns::StoQuota): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      editor_->setAlignment(Qt::AlignRight);
      editor_->setValidator(new QDoubleValidator(0, 9999999999, 0, editor_));
      editor_->setMaxLength(10);
      editor_->setText(QString("%L0").arg(indexValue_.toString()));
      break;
    }
    default: {
    }
  }
}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::setModelData
 * \param editorWidget
 * \param model
 * \param index
 */
void
TableWidget_RepoNamesItemDelegate::setModelData(QWidget* editorWidget,
                                                QAbstractItemModel* model,
                                                const QModelIndex& index) const
{
  switch (index.column()) {
    case static_cast<int>(Columns::Name): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      const QString newData_ = editor_->text();
      model->setData(index, newData_);
      break;
    }
    case static_cast<int>(Columns::Archive): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      const QString newData_ = editor_->text();
      model->setData(index, newData_);
      break;
    }
    case static_cast<int>(Columns::EncMode): {
      QComboBox* box_ = dynamic_cast<QComboBox*>(editorWidget);
      const QString newData_ = box_->currentText();
      model->setData(index, newData_);
      break;
    }
    case static_cast<int>(Columns::Append): {
      QCheckBox* editor_ = dynamic_cast<QCheckBox*>(editorWidget);
#if 0
      qDebug() << __FUNCTION__ << " checkstate " << editor_->checkState() << " "
               << (editor_->checkState() == Qt::Checked ? true : false)
               << " idx= " << index;
#endif
      model->setData(index,
                     editor_->checkState() == Qt::Checked ? Qt::Checked
                                                          : Qt::Unchecked);
      break;
    }
    case static_cast<int>(Columns::MakeDirPath): {
      QCheckBox* editor_ = dynamic_cast<QCheckBox*>(editorWidget);
      model->setData(index,
                     editor_->checkState() == Qt::Checked ? true : false);
      break;
    }
    case static_cast<int>(Columns::StoQuota): {
      QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
      const QString newData_ = editor_->text();
      model->setData(index, (!newData_.isEmpty() ? newData_ : "0"));
      break;
    }
    default: {
      model->setData(index, model->data(index, Qt::EditRole).toString());
    }
  }
}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::updateEditorGeometry
 * \param editorWidget
 * \param option
 * \param index
 */
void
TableWidget_RepoNamesItemDelegate::updateEditorGeometry(
  QWidget* editorWidget,
  const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  editorWidget->setGeometry(option.rect);
}

/*!
 * \brief TableWidget_RepoNamesItemDelegate::paint
 * \param painter
 * \param option
 * \param index
 */
void
TableWidget_RepoNamesItemDelegate::paint(QPainter* painter,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
  switch (index.column()) {
    case static_cast<int>(Columns::Name): {
      const QString text_ =
        index.model()->data(index, Qt::DisplayRole).toString();
      QStyleOptionViewItem myOption = option;
      drawDisplay(painter, myOption, myOption.rect, text_);
      drawFocus(painter, myOption, myOption.rect);
      break;
    }
    case static_cast<int>(Columns::Archive): {
      const QString text_ =
        index.model()->data(index, Qt::DisplayRole).toString();
      QStyleOptionViewItem myOption = option;
      drawDisplay(painter, myOption, myOption.rect, text_);
      drawFocus(painter, myOption, myOption.rect);
      break;
    }
    case static_cast<int>(Columns::EncMode): {
      break;
    }
    case static_cast<int>(Columns::Append): {
      bool value_ = index.model()->data(index, Qt::DisplayRole).toBool();
      /*QStyleOptionViewItem myOption = option;
drawCheck(
  painter, myOption, myOption.rect, value_ ? Qt::Checked : Qt::Unchecked);
drawFocus(painter, myOption, myOption.rect);*/

      QStyleOptionButton checkboxstyle;
      /*QRect checkbox_rect = QApplication::style()->subElementRect(
    QStyle::SE_CheckBoxIndicator, &checkboxstyle);*/
      checkboxstyle.rect = option.rect;
      /*checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()  /  -
                           checkbox_rect.width() / 2);*/
      if (value_) {
        checkboxstyle.state = QStyle::State_On | QStyle::State_Enabled;
      } else {
        checkboxstyle.state = QStyle::State_Off | QStyle::State_Enabled;
      }

      QApplication::style()->drawControl(
        QStyle::CE_CheckBox, &checkboxstyle, painter);

      break;
    }
    case static_cast<int>(Columns::MakeDirPath): {
      bool value_ = index.model()->data(index, Qt::DisplayRole).toBool();
      /*QStyleOptionViewItem myOption = option;
drawCheck(
painter, myOption, myOption.rect, value_ ? Qt::Checked : Qt::Unchecked);
drawFocus(painter, myOption, myOption.rect);*/

      QStyleOptionButton checkboxstyle;
      /*QRect checkbox_rect = QApplication::style()->subElementRect(
    QStyle::SE_CheckBoxIndicator, &checkboxstyle);*/
      checkboxstyle.rect = option.rect;
      /*checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()  /  -
                         checkbox_rect.width() / 2);*/
      if (value_) {
        checkboxstyle.state = QStyle::State_On | QStyle::State_Enabled;
      } else {
        checkboxstyle.state = QStyle::State_Off | QStyle::State_Enabled;
      }

      QApplication::style()->drawControl(
        QStyle::CE_CheckBox, &checkboxstyle, painter);
      break;
    }
    case static_cast<int>(Columns::StoQuota): {
      const QString text_ =
        index.model()->data(index, Qt::DisplayRole).toString();
      QStyleOptionViewItem myOption = option;
      myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
      drawDisplay(painter, myOption, myOption.rect, text_);
      drawFocus(painter, myOption, myOption.rect);

      break;
    }
    default: {
      QItemDelegate::paint(painter, option, index);
    }
  }
}

#if 0
bool
TableWidget_RepoNamesItemDelegate::editorEvent(
  QEvent* event,
  QAbstractItemModel* model,
  const QStyleOptionViewItem& option,
  const QModelIndex& index)
{
  Q_ASSERT(event);
  Q_ASSERT(model);

  // make sure that the item is checkable
  Qt::ItemFlags flags = model->flags(index);
  if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
    return false;
  // make sure that we have a check state
  QVariant value = index.data(Qt::CheckStateRole);
  if (!value.isValid())
    return false;
  // make sure that we have the right event type
  /*
  if (event->type() == QEvent::MouseButtonRelease) {
    const int textMargin =
      QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect checkRect =
      QStyle::alignedRect(option.direction,
                          Qt::AlignCenter,
                          check(option, option.rect, Qt::Checked).size(),
                          QRect(option.rect.x() + textMargin,
                                option.rect.y(),
                                option.rect.width() - (2 * textMargin),
                                option.rect.height()));
    if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
      return false;
  } else if (event->type() == QEvent::KeyPress) {
    if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
        static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
      return false;
  } else {
    return false;
  }
*/
  Qt::CheckState state =
    (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked ? Qt::Unchecked
                                                               : Qt::Checked);

  QMessageBox::information(
    0,
    QString((state == Qt::Checked) ? "Qt::Checked" : "Qt::Unchecked"),
    QString("[%1/%2]").arg(index.row()).arg(index.column()));

  return model->setData(index, state, Qt::CheckStateRole);
}
#endif
