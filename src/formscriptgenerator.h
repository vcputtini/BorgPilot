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
#ifndef FORMSCRIPTGENERATOR_H
#define FORMSCRIPTGENERATOR_H

#include "QStandardItem"
#include "QStandardItemModel"
#include "QString"
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMessageBox>
#include <QMultiHash>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QTableWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include "command2.h"
#include "global_options.h"
#include "scriptgen.h"

#include "dialogshowlogfile.h"

namespace Ui {
class FormScriptGenerator;
}

/*!
 * \brief The NameRepoLineEdit class
 */
class NameRepoLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit NameRepoLineEdit(bool isReadOnly = false, QWidget* parent = nullptr)
    : QLineEdit(parent)
  {
    setMaxLength(254);
    if (isReadOnly) {
      setReadOnly(true);
    } else {
      setToolTip(
        tr("<html><head/><body><p>Max. length: 254 bytes</p><p>Valid chars: "
           "Aa-Zz 0-9 _ /</p></body></html>"));
      QRegularExpression regExpName_("[0-9A-Za-z_]*"); // valid chars
      setValidator(new QRegularExpressionValidator(regExpName_, this));
    }
  }
};

/*!
 * \brief The ArchiveLineEdit class
 */
class ArchiveLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit ArchiveLineEdit(bool isReadOnly = false, QWidget* parent = nullptr)
    : QLineEdit(parent)
  {
    setMaxLength(254);
    if (isReadOnly) {
      setReadOnly(true);
    } else {
      setToolTip(
        tr("<html><head/><body><p>Max. length: 254 bytes</p><p>Valid chars: "
           "Aa-Zz 0-9 _ {} %/</p></body></html>"));
    }
  }
};

/*!
 * \brief The StoQuotaLineEdit class
 */
class StoQuotaLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit StoQuotaLineEdit(QWidget* parent = nullptr)
    : QLineEdit(parent)
  {
    setMaxLength(10);
    setText("0");
    setToolTip("Max. length: <b>10 bytes</b><br><br>"
               "Note: Allowed suffix: <b>B|M|G|T</b><br><br>"
               "Usage: <b>0</b> no limit, or: 1024B, 100M, "
               "1.5G, 1G,, 1T and so on.");
    QRegularExpression regExp_("[0-9. B|M|G|T]*"); // valid chars
    setValidator(new QRegularExpressionValidator(regExp_, this));
  }
};

/*!
 * \brief The EncModeComboBox class
 */
class EncModeComboBox : public QComboBox
{
  Q_OBJECT
public:
  explicit EncModeComboBox(QWidget* parent = nullptr)
    : QComboBox(parent)
  {
    addItems(QStringList() << "none"
                           << "repokey"
                           << "repokey-blake2");
  }
};

/*!
 * \brief The CheckBox class
 */
class CheckBox : public QCheckBox
{
  Q_OBJECT
public:
  explicit CheckBox(QWidget* parent = nullptr)
    : QCheckBox(parent)
  {
    setTristate(false);
    setChecked(false);
  }

protected:
  // void paintEvent(QPaintEvent* event) override { QPainter painter_(this); }
};

/*!
 * \brief The CheckableComboBox class
 */
class CheckableComboBox : public QComboBox
{
  Q_OBJECT
public:
  explicit CheckableComboBox(QWidget* parent = nullptr)
    : QComboBox(parent)
  {
    model = new QStandardItemModel(this);
    setModel(model);
    setView(new QListView());
  }

  void addItem(const QString& text)
  {
    QStandardItem* item = new QStandardItem(text);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    model->appendRow(item);
  }

  QList<QString> checkedItems() const
  {
    QList<QString> checkedList;
    for (int i = 0; i < model->rowCount(); i++) {
      if (model->item(i)->checkState() == Qt::Checked) {
        checkedList.append(model->item(i)->text());
      }
    }
    return checkedList;
  }

private:
  QStandardItemModel* model;

#if 0
protected:
  void showPopup() override
  {
    QComboBox::showPopup();
    // Customize the popup if needed
  }

  void paintEvent(QPaintEvent* event) override
  {
    QComboBox::paintEvent(event);
    // Custom painting for the combo box can be added here
  }
#endif
};

/*!
 * \brief The CheckableComboBox2 class
 */
class CheckableComboBox2 : public QComboBox
{
  Q_OBJECT

public:
  CheckableComboBox2(QWidget* parent = nullptr)
    : QComboBox(parent)
  {
    model = new QStandardItemModel(this);
    setModel(model);
    setView(new QListView());
  }

  void addItem(const QString& text)
  {
    QStandardItem* item = new QStandardItem(text);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    model->appendRow(item);
  }

  QList<QString> checkedItems() const
  {
    QList<QString> checkedList;
    for (int i = 0; i < model->rowCount(); ++i) {
      if (model->item(i)->checkState() == Qt::Checked) {
        checkedList.append(model->item(i)->text());
      }
    }
    return checkedList;
  }

protected:
  void showPopup() override
  {
    QComboBox::showPopup();
    QListView* listView = qobject_cast<QListView*>(view());
    if (listView) {
      listView->setGeometry(view()->geometry());
      listView->scrollToBottom();
    }
  }

private:
  QStandardItemModel* model;
  QVBoxLayout* VBoxLayout;
};

// =========================================================================

/*!
 * \brief Main Class: The FormScriptGenerator class
 */
class FormScriptGenerator
  : public QWidget
  , public GlobalOptions
{
  Q_OBJECT

public:
  explicit FormScriptGenerator(QWidget* parent = nullptr);
  ~FormScriptGenerator();

private:
  Ui::FormScriptGenerator* ui;

private:
  Command Args_;
  QSettings settings_;

  QMainWindow* mainWindowToolBar;
  QToolBar* toolBarSettings;
  QToolButton* toolButtonClear;

  DialogShowLogFile* dialogShowFile;

  QStandardItem* newItem;
  QStandardItemModel* siModel;

  ScriptGen* shgen_;

  enum class Pages : int
  {
    InitRepos,
    CreateRepos
  };

  enum class Columns
  {
    Name = 0x00,
    Archive,
    EncMode,
    Append,
    MakeDirPath,
    StoQuota
  };

  static constexpr std::initializer_list<Columns> Columns_List_ = {
    Columns::Name,   Columns::Archive,     Columns::EncMode,
    Columns::Append, Columns::MakeDirPath, Columns::StoQuota
  };

  void setupInitRepoTable();
  void createToolBar();

  class TestFileNames
  {
  public:
    enum class Reasons
    {
      SaveWithNewname,
      NameProvidedByUser,
      NotExists
    };
    Reasons test(const QString& fn_);
    QString newFilename() { return newName_; }

  private:
    QString newName_;
  };

  NameRepoLineEdit* name_value_;
  ArchiveLineEdit* archive_value_;
  EncModeComboBox* encmode_value_;
  CheckBox* append_value_;
  CheckBox* mkparentdir_value_;
  StoQuotaLineEdit* stoquota_value_;

  void setupButtons();

private slots:
  void saveInitTable();
  void loadInitTable();
  void genInitScript();

  void setupCreateBackup();
  void genCreateBackupReload();
  void genCreateBackup();
  void genCreateSaveSettings();

  void fileError(const QString& text_);

  void dialogShowLogFile_triggered();
};

#endif // FORMSCRIPTGENERATOR_H
