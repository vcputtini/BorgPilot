/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
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
#include "tableviewsettings_itemdelegate.h"

/*!
 * \brief TableViewSettings_itemDelegate::TableViewSettings_itemDelegate
 * \param parent
 */
TableViewSettings_itemDelegate::TableViewSettings_itemDelegate(QObject* parent)
  : QItemDelegate(parent)
{
}

/*!
 * \brief TableViewSettings_itemDelegate::~TableViewSettings_itemDelegate
 */
TableViewSettings_itemDelegate::~TableViewSettings_itemDelegate() {}

/*!
 * \brief ImportBatchitemDelegate::createEditor
 * \param parent
 * \param view
 * \param index
 * \return
 */
QWidget*
TableViewSettings_itemDelegate::createEditor(QWidget* parent,
                                             const QStyleOptionViewItem& view,
                                             const QModelIndex& index) const
{
  QWidget* e_;

  if (index.column() == static_cast<int>(Columns::Number)) {
    QLineEdit* editor_ = new QLineEdit(parent);
    editor_->setValidator(new QIntValidator(0, 32767));
    editor_->setMaxLength(6);
    e_ = editor_;
  } else if (index.column() == static_cast<int>(Columns::IdDoc)) {
    QLineEdit* editor_ = new QLineEdit(parent);
    editor_->setMaxLength(30);
    e_ = editor_;
  } else if (index.column() == static_cast<int>(Columns::Date)) {
    QDateEdit* editor_ = new QDateEdit(parent);
    editor_->setCalendarPopup(true);
    editor_->setDate(QDate::currentDate());
    e_ = editor_;
  } else if (index.column() == static_cast<int>(Columns::Value)) {
    QLineEdit* editor_ = new QLineEdit(parent);
    editor_->setValidator(new QDoubleValidator(0.0, 999999999.99, 2, editor_));
    editor_->setMaxLength(13);
    e_ = editor_;
  } else {
    e_ = new QLineEdit(parent);
  }

  e_->installEventFilter(const_cast<TableViewSettings_itemDelegate*>(this));
  return (e_);
}

/*!
 * \brief TableViewSettings_itemDelegate::setEditorData
 * \param editorWidget
 * \param index
 */
void
TableViewSettings_itemDelegate::setEditorData(QWidget* editorWidget,
                                              const QModelIndex& index) const
{
  const QVariant indexValue = index.model()->data(index, Qt::EditRole);

  if (index.column() == static_cast<int>(Columns::Number)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    editor_->setAlignment(Qt::AlignRight);
    editor_->setValidator(new QIntValidator(0, 32762));
    editor_->setMaxLength(6);
    editor_->setText(QString("%L0").arg(indexValue.toString()));
  } else if (index.column() == static_cast<int>(Columns::IdDoc)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    editor_->setText(indexValue.toString());
  } else if (index.column() == static_cast<int>(Columns::Date)) {
    QDateEdit* editor_ = dynamic_cast<QDateEdit*>(editorWidget);
    editor_->setDate(QDate::fromString(indexValue.toString(), "dd/MM/yyyy"));
  } else if (index.column() == static_cast<int>(Columns::Value)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    editor_->setAlignment(Qt::AlignRight);
    editor_->setValidator(new QDoubleValidator(0.0, 999999999.99, 2, editor_));
    editor_->setMaxLength(13);
    editor_->setText(QString("%L0").arg(indexValue.toString()));
  }
}

/*!
 * \brief TableViewSettings_itemDelegate::setModelData
 * \param editorWidget
 * \param model
 * \param index
 */
void
TableViewSettings_itemDelegate::setModelData(QWidget* editorWidget,
                                             QAbstractItemModel* model,
                                             const QModelIndex& index) const
{
  QString oldData_ = model->data(index, Qt::EditRole).toString();
  QString newData_{};

  if (index.column() == static_cast<int>(Columns::Number)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    newData_ = editor_->text();
    if (oldData_ != newData_) {
      model->setData(index, newData_);
    }
  } else if (index.column() == static_cast<int>(Columns::IdDoc)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    newData_ = editor_->text();
    if (oldData_ != newData_) {
      model->setData(index, newData_);
    }
  } else if (index.column() == static_cast<int>(Columns::Date)) { // date
    QDateEdit* editor_ = dynamic_cast<QDateEdit*>(editorWidget);
    newData_ = editor_->text();
    if (oldData_ != newData_) {
      model->setData(index, newData_);
    }
  } else if (index.column() == static_cast<int>(Columns::Value)) {
    QLineEdit* editor_ = dynamic_cast<QLineEdit*>(editorWidget);
    newData_ = editor_->text();
    model->setData(index, (!newData_.isEmpty() ? newData_ : nullptr));
  } else
    model->setData(index, model->data(index, Qt::EditRole).toString());
}

/*!
 * \brief TableViewSettings_itemDelegate::updateEditorGeometry
 * \param editorWidget
 * \param option
 * \param index
 */
void
TableViewSettings_itemDelegate::updateEditorGeometry(
  QWidget* editorWidget,
  const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  editorWidget->setGeometry(option.rect);
}

/*!
 * \brief TableViewSettings_itemDelegate::paint
 * \param painter
 * \param option
 * \param index
 */
void
TableViewSettings_itemDelegate::paint(QPainter* painter,
                                      const QStyleOptionViewItem& option,
                                      const QModelIndex& index) const
{
  /*
  if (index.column() == static_cast<int>(Columns::Number)) {
    const QString text_ =
      index.model()->data(index, Qt::DisplayRole).toString();
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    drawDisplay(painter, myOption, myOption.rect, text_);
    drawFocus(painter, myOption, myOption.rect);
  } else if (index.column() == static_cast<int>(Columns::Value)) {
    const QString text_ =
      index.model()->data(index, Qt::DisplayRole).toString();
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    // if (int i_ = text.toInt(); text.toFloat() == i_) {
    //   text += ",00";
    // } else {
    // text = QString::number(text.toFloat(), 'f', 2);
    //}
    drawDisplay(painter, myOption, myOption.rect, text_);
    drawFocus(painter, myOption, myOption.rect);
  } else {
    QItemDelegate::paint(painter, option, index);
  }
  */
}
