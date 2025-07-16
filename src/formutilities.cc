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
#include "formutilities.h"
#include "src/ui_formutilities.h"

#include <QAbstractListModel>
#include <QBrush>
#include <QFileDialog>
#include <QHash>
#include <QList>
#include <QListView>
#include <QMessageBox>
#include <QOverload>
#include <QPair>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "borgrunnerthread.h"
#include "listwidget_delete_p.h"
#include "listwidget_prune_p.h"
#include "progid.h"
#include "settingshandler.h"

#include "types_global.h"

/*!
 * \brief FormUtilities::FormUtilities
 * \param parent
 */
FormUtilities::FormUtilities(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormUtilities)
{
  ui->setupUi(this);

  configBehaviorRadioButtons();
  configBehaviorToolButtons();
  configBehaviorComboBoxes();
  configBehaviorListWidget();

  /* -------------------------------------------------------------------- */
  connect(
    ui->toolButton_Exec, SIGNAL(clicked(bool)), this, SLOT(execCommand()));
}

/*!
 * \brief FormUtilities::~FormUtilities
 */
FormUtilities::~FormUtilities()
{
  delete ui;
}

/*!
 * \private slot
 * \brief FormUtilities::execCommand
 */
void
FormUtilities::execCommand()
{
  ui->textEdit_Results->clear();

  if (commandSelected_ == "None" || commandSelected_.isEmpty()) {
    ui->textEdit_Results->append(tr("Command canceled by user"));
    commandSelected_.clear();
    return;
  }

  const QString input_ = std::move(ui->lineEdit_Command->text().simplified());
  if (input_.isEmpty()) {
    ui->textEdit_Results->append(tr("Archive/Placeholder must be informed!"));
    ui->lineEdit_Command->setFocus();
    return;
  }

  auto warning_ = [this]() -> bool {
    QMessageBox msgBox;
    msgBox.setText(tr("<strong>Warning!</strong>").toUtf8());
    msgBox.setInformativeText(
      tr("The <b>delete and prune</b> commands are <b>dangerous and will "
         "cause data "
         "loss</b> if you use them incorrectly. You should strongly select "
         "the "
         "<b>--dry-run</b> option to ensure that the result is as expected. "
         "\n\nDo "
         "you "
         "really want to run the command?")
        .toUtf8());
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    const int ret_ = msgBox.exec();
    return (ret_ == QMessageBox::Yes ? true : false);
  };

  auto MessageBox = [](const QString& text_) {
    QMessageBox mbox_;
    mbox_.setIcon(QMessageBox::Information);
    mbox_.setText(tr(text_.toStdString().c_str()));
    mbox_.exec();
  };

  BorgThreadRunner::BorgCommands borgCommand_ =
    BorgThreadRunner::fromStrToCommand(commandSelected_);

  if (borgCommand_ == BorgThreadRunner::BorgCommands::Delete ||
      borgCommand_ == BorgThreadRunner::BorgCommands::Prune) {
    if (!warning_()) {
      return;
    }
  }

  QStringList borgComm_{};
  QString repoSource_{};
  /* checks ------------------------------------------------------------ */
  switch (borgCommand_) {
    case BorgThreadRunner::BorgCommands::Info:
    case BorgThreadRunner::BorgCommands::List:
    case BorgThreadRunner::BorgCommands::Check: {
      if (const QString repoName_ = nameRepoSelected_.simplified();
          !repoName_.isEmpty()) {
        const QString command_ = ui->lineEdit_Command->text().simplified();
        const QString parameters_ =
          ui->lineEdit_Parameters->text().simplified();
        repoSource_ =
          std::move(QString("%1 %2::%3").arg(parameters_, repoName_, command_));
      }
      break;
    }
    case BorgThreadRunner::BorgCommands::Diff: {
      if (const QString otherRepo_ =
            ui->lineEdit_Parameters->text().simplified();
          !otherRepo_.isEmpty()) {
        const QString repoName_ = nameRepoSelected_.simplified();
        const QString command_ = ui->lineEdit_Command->text().simplified();
        repoSource_ =
          std::move(QString("%1::%2 %3").arg(repoName_, command_, otherRepo_));
      }
      break;
    }
    case BorgThreadRunner::BorgCommands::Rename: {
      if (const QString newName_ = ui->lineEdit_Parameters->text().simplified();
          !newName_.isEmpty()) {
        const QString repoName_ = nameRepoSelected_.simplified();
        const QString command_ = ui->lineEdit_Command->text().simplified();
        repoSource_ =
          std::move(QString("%1::%2 %3").arg(repoName_, command_, newName_));
      }
      break;
    }
    case BorgThreadRunner::BorgCommands::Delete: {
      if (const QString command_ = ui->lineEdit_Command->text().simplified();
          !command_.isEmpty()) {
        const QString repoName_ = nameRepoSelected_.simplified();
        const QString parameters_ =
          ui->lineEdit_Parameters->text().simplified();
        repoSource_ =
          std::move(QString("%1 %2::%3").arg(parameters_, repoName_, command_));
      }
      break;
    }
    case BorgThreadRunner::BorgCommands::Prune: {
      if (const QString command_ = ui->lineEdit_Command->text().simplified();
          !command_.isEmpty()) {
        const QString repoName_ = nameRepoSelected_.simplified();
        const QString parameters_ =
          ui->lineEdit_Parameters->text().simplified();
        repoSource_ =
          std::move(QString("%1 %2::%3").arg(parameters_, repoName_, command_));
      }
    }
    default: {
      return;
    }
  } // end-switch
  /* ------------------------------------------------------------------ */
  auto extractPatterns = [](const QString& input_) {
    QStringList result_;
    // Capture --pattern '...'
    static QRegularExpression re(R"(--pattern\s+(['"])(.*?)\1)");
    QRegularExpressionMatchIterator it = re.globalMatch(std::move(input_));

    while (it.hasNext()) {
      QRegularExpressionMatch match_ = it.next();
      QString patternValue_ = match_.captured(2).trimmed(); // e.g: "+ re:.png$"

      // Here the integer must be passed: "+ re:.png$"
      if (!patternValue_.isEmpty()) {
        result_ << std::move("--pattern") << std::move(patternValue_);
      } else {
        return QStringList();
      }
    }
    return result_;
  };
  /* ------------------------------------------------------------------ */
  borgComm_ = { repoSource_.split(" ") };
  borgComm_.removeIf([](const QString& s_) { return s_.isEmpty(); });

  static QRegularExpression re_(R"(^(.+?)\s+(--format=.*)$)");
  QRegularExpressionMatch match_ = re_.match(std::move(repoSource_));
  QStringList args_{};
  if (match_.hasMatch()) {
    args_.append(std::move(match_.captured(1)));
    args_.append(std::move(match_.captured(2)));
  } else {
    args_.append(std::move(input_.section(' ', 0, 0)));
    args_.append(std::move(extractPatterns(repoSource_)));
  }

#if 0
  for (auto& a : args_) {
    qDebug() << "> " << a;
  }
#endif

  ui->textEdit_Results->append(
    QString("Command: borg %0 %1").arg(commandSelected_, borgComm_.at(0)));

  auto* thread_ =
    new BorgThreadRunner(borgCommand_, std::move(borgComm_), this);
  Q_ASSERT(thread_);

  connect(
    thread_,
    &BorgThreadRunner::borgOutput,
    this,
    [=, this](const QString& line) { ui->textEdit_Results->append(line); });
  connect(
    thread_, &BorgThreadRunner::borgError, this, [=, this](const QString& err) {
      ui->textEdit_Results->append("[ERROR] " + err);
    });
  connect(thread_,
          &BorgThreadRunner::borgFinished,
          this,
          [=, this](int code, QProcess::ExitStatus status) {
            ui->textEdit_Results->append("Completed (code " +
                                         QString::number(code) + ")");
            thread_->deleteLater();
          });

  ui->textEdit_Results->append("Executing...");

  if (!thread_->isRunning()) {
    thread_->start();
  }
}

/*!
 * \private
 * \brief FormUtilities::findRepoAndArchive
 * \param dataList_
 * \return
 */
FormUtilities::RepoArchiveInfo
FormUtilities::findRepoAndArchive(
  const AppTypes::NamedValueList& dataList_) const
{
  QHash<QString, QString> repoMap_;
  QHash<QString, QString> archiveMap_;
  QString destRepo_;

  static const QRegularExpression re_repo_(R"(^B_REPONAME_(\d+)$)");
  static const QRegularExpression re_arch_(R"(^B_ARCHIVE_(\d+)$)");
  static const QRegularExpression re_destrepo_(R"(^A_DESTREPO$)");

  for (const auto& pair_ : dataList_) {
    const QString& key_ = pair_.first.trimmed();
    const QString value_ = pair_.second.trimmed();

    if (value_.isEmpty()) {
      continue;
    }

    if (auto match_ = re_repo_.match(key_); match_.hasMatch()) {
      const QString id_ = match_.captured(1);
      repoMap_[id_] = value_;
    } else if (auto match_ = re_arch_.match(key_); match_.hasMatch()) {
      const QString id_ = match_.captured(1);
      archiveMap_[id_] = value_;
    } else if (re_destrepo_.match(key_).hasMatch()) {
      destRepo_ = value_;
    }
  }

  AppTypes::NamedValueList result_;
  for (QString id_ : repoMap_.keys()) {
    if (archiveMap_.contains(id_)) {
      result_.append(qMakePair(repoMap_[id_], archiveMap_[id_]));
    }
  }
  return { destRepo_, result_ };
}

/*!
 * \private slot
 * \brief FormUtilities::acaoRadioSelecionado
 * \param botao
 */
void
FormUtilities::actionSelectedRadio(QRadioButton* button_)
{
  for (const QString objName_ = button_->objectName();
       auto& a_ : Command::getCommandsList()) {
    if (objName_.contains(QString::fromStdString(a_), Qt::CaseInsensitive)) {
      if (Command cmd_; button_->isChecked()) {
        switch (cmd_.getCommandStr(a_)) {
          case Command::Command_EC::Delete: {
            ui->lineEdit_Parameters->clear();
            ui->lineEdit_Parameters->setFocus();
            ui->lineEdit_Parameters->setPlaceholderText("");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::Prune: {
            ui->lineEdit_Parameters->clear();
            ui->lineEdit_Parameters->setFocus();
            ui->lineEdit_Parameters->setPlaceholderText("");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::Info: {
            ui->lineEdit_Parameters->setPlaceholderText(
              "Repository or archive to display information about");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::Check: {
            ui->lineEdit_Parameters->setPlaceholderText(
              "Repository or archive to check consistency of");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::List: {
            ui->lineEdit_Parameters->setPlaceholderText(
              "Repository or archive to list contents of");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::Diff: {
            ui->lineEdit_Parameters->clear();
            ui->lineEdit_Parameters->setFocus();
            ui->lineEdit_Parameters->setPlaceholderText(
              "Name of the archive to compare");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          case Command::Command_EC::Rename: {
            ui->lineEdit_Parameters->clear();
            ui->lineEdit_Parameters->setFocus();
            ui->lineEdit_Parameters->setPlaceholderText(
              "The new archive name to use");
            commandSelected_ = std::move(a_.c_str());
            break;
          }
          default: {
            ui->lineEdit_Parameters->setPlaceholderText("");
            commandSelected_ = std::move("None");
          }
        } // end-switch
      }
    }
  }
}

// Setups =================================================================
/*!
 * \private
 * \brief FormUtilities::configBehaviorRadioButtons
 */
void
FormUtilities::configBehaviorRadioButtons()
{
  commandSelected_ = (ui->radioButton_Info->isCheckable() ? "info" : "none");

  // connect all RadioButtons to slot using lambda
  listObjects<QRadioButton>(ui->groupBox_Commands, [this](QRadioButton* rb_) {
    connect(rb_, &QRadioButton::clicked, this, [this, rb_]() {
      this->actionSelectedRadio(rb_);
    });
  });

  connect(
    ui->radioButton_Rename, &QRadioButton::toggled, this, [this](bool checked) {
      if (!checked) {
        ui->lineEdit_Parameters->clear();
        ui->lineEdit_Parameters->setFocus();
      }
    });
  connect(
    ui->radioButton_Diff, &QRadioButton::toggled, this, [this](bool checked) {
      if (!checked) {
        ui->lineEdit_Parameters->clear();
        ui->lineEdit_Parameters->setFocus();
      }
    });

  // ---
  connect(ui->radioButton_Cmd_Delete,
          &QRadioButton::toggled,
          this,
          [this](bool checked) {
            if (!checked) {
              ui->listWidget_Delete->unCheckAllOptions();
            }
            ui->listWidget_Delete->setEnabled(checked);
          });
  connect(ui->radioButton_Cmd_Prune,
          &QRadioButton::toggled,
          this,
          [this](bool checked) {
            if (!checked) {
              ui->listWidget_Prune->unCheckAllOptions();
            }

            ui->listWidget_Prune->setEnabled(checked);
          });
}

/*!
 * \private
 * \brief FormUtilities::configBehaviorToolButtons
 */
void
FormUtilities::configBehaviorToolButtons()
{
  /* Clear text */
  connect(ui->toolButton_ClearResults, &QToolButton::clicked, this, [this]() {
    ui->textEdit_Results->clear();
  });

  /* Save results */
  connect(ui->toolButton_SaveAsResults, &QToolButton::clicked, this, [this]() {
    if (ui->textEdit_Results->toPlainText().isEmpty()) {
      QMessageBox::information(this, tr("Information"), tr("Text empty."));
      return;
    }

    QFileDialog dlg_;
    dlg_.setFileMode(QFileDialog::ExistingFiles);
    dlg_.setViewMode(QFileDialog::Detail);
    dlg_.setNameFilter(R"(*.txt)");
    dlg_.setDirectory(QDir::homePath());

    QString fn_ = dlg_.getSaveFileName(
      this, tr("Save File As"), QDir::homePath(), tr("Results (*.txt)"));
    if (fn_.isEmpty()) {
      const int ret_ = QMessageBox::warning(this,
                                            ProgId::Name,
                                            tr("File name was not provided!"),
                                            QMessageBox::Cancel);
      dlg_.close();
      return;
    }
    QFile file_((!fn_.contains(R"(.txt)") ? QString("%0.txt").arg(fn_) : fn_));
    QStringList lines_ = ui->textEdit_Results->toPlainText().split('\n');
    if (file_.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out_(&file_);
      for (const QString& line_ : std::as_const(lines_)) {
        out_ << line_ << '\n';
      }
      file_.close();
    } else {
      const int ret_ =
        QMessageBox::warning(this,
                             tr("ProgId::Name"),
                             tr("File Error: %0 ").arg(file_.errorString()),
                             QMessageBox::Cancel);
      dlg_.close();
    }
  });
}

/*!
 * \private
 * \brief FormUtilities::configBehaviorListWidget
 */
void
FormUtilities::configBehaviorListWidget()
{
  ui->listWidget_Delete->setEnabled(false);
  ui->listWidget_Prune->setEnabled(false);

  // Get the pointer to the promoted QListWidget
  listWidget_Delete_P =
    qobject_cast<ListWidget_Delete_P*>(ui->listWidget_Delete);
  Q_ASSERT(listWidget_Delete_P);

  listWidget_Prune_P = qobject_cast<ListWidget_Prune_P*>(ui->listWidget_Prune);
  Q_ASSERT(listWidget_Prune_P);

  auto mountArgs_ = [this](auto& data_) -> QString {
    QString args_{};
    args_.reserve(1024);
    for (const auto& [key_, value_] : data_) {
      args_ += " ";
      args_ +=
        (value_ == "" ? std::move(key_) : std::move(key_ + "=" + value_));
    }
    return args_;
  };

  connect(ui->listWidget_Delete,
          &ListWidget_Delete_P::itemsSelectedChanged,
          this,
          [this, mountArgs_](const AppTypes::SelectedPairs& data_) {
            ui->lineEdit_Parameters->setText(std::move(mountArgs_(data_)));
          });

  connect(listWidget_Prune_P,
          &ListWidget_Prune_P::itemsSelectedChanged,
          this,
          [this, mountArgs_](const AppTypes::SelectedPairs& data_) {
            ui->lineEdit_Parameters->setText(std::move(mountArgs_(data_)));
          });
}

/*!
 * \private
 * \brief FormUtilities::configBehaviorComboBoxes
 */
void
FormUtilities::configBehaviorComboBoxes()
{
  listViewRepos_P = qobject_cast<ListViewRepos_P*>(ui->listView_Repos);
  Q_ASSERT(listViewRepos_P);
  connect(listViewRepos_P,
          &ListViewRepos_P::lineSelected,
          this,
          [this](const QString& text_) {
            nameRepoSelected_ =
              std::move(text_.section("::", 0, 0).simplified());
            archiveSelected_ = std::move(text_.section("::", 1).simplified());
            ui->lineEdit_Command->setText(std::move(archiveSelected_));
          });

  connect(ui->comboBox_ScriptName,
          &QComboBox::currentTextChanged,
          this,
          [this](const QString& text_) {
            auto* model_ = new QStandardItemModel(this);
            listViewRepos_P->setModel(model_);

            SettingsHandler sHandler_;
            AppTypes::NamedValueList list_ =
              sHandler_.readInitScriptDefs(text_);
            auto pairs_ = findRepoAndArchive(list_);

            for (const auto& it_ : std::as_const(pairs_.repoAndArchives)) {
              const QString repoPath_ =
                QString("%1/%2").arg(pairs_.destRepo, it_.first);

              QStringList args_;
              args_ << repoPath_ << R"(--format={archive}{NL})";
              auto* thread_ = new BorgThreadRunner(
                BorgThreadRunner::BorgCommands::List, args_, this);
              connect(thread_,
                      &BorgThreadRunner::borgOutput,
                      this,
                      [model_, repoPath_](const QString& outputBlock) {
                        const QStringList lines_ =
                          outputBlock.split('\n', Qt::SkipEmptyParts);
                        for (const QString& line_ : lines_) {
                          const QString trimmed_ = line_.trimmed();
                          if (!trimmed_.isEmpty()) {
                            auto* item_ = new QStandardItem(
                              QString("%1::%2").arg(repoPath_, trimmed_));
                            item_->setEditable(false);
                            model_->appendRow(item_);
                          }
                        }
                      });
              connect(thread_,
                      &BorgThreadRunner::borgError,
                      this,
                      [model_, repoPath_](const QString& errorLine) {
                        auto* errorItem_ = new QStandardItem(
                          QString("Error while listing %1: %2")
                            .arg(repoPath_, errorLine.trimmed()));
                        errorItem_->setEditable(false);
                        errorItem_->setForeground(QBrush(Qt::red));
                        model_->appendRow(errorItem_);
                      });
              connect(thread_, &QObject::destroyed, this, [thread_]() {
                thread_->deleteLater();
              });
              if (!thread_->isRunning()) {
                thread_->start();
              }
            }
          });
}
