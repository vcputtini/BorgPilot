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
#include "formscriptgenerator.h"
#include "src/ui_formscriptgenerator.h"

#include <QColor>
#include <QFont>
#include <QFontDatabase>
#include <QPointer>

#include "bashscriptrunner.h"
#include "combobox_scriptcreatename_p.h"
#include "dialogShowFilesModes.h"
#include "global_options.h"
#include "global_tools.h"
#include "listviewexecscripts_p.h"
#include "progid.h"
#include "settingshandler.h"
#include "types_global.h"

FormScriptGenerator::FormScriptGenerator(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormScriptGenerator)
  , mainWindowToolBar(new QMainWindow)
  , toolBarSettings(new QToolBar)
  , newItem(new QStandardItem)
{
  ui->setupUi(this);

  setupCharValidation();
  setupInitRepoTable();
  sl_setupCreateBackup();
  setupTextBrowser();
  setupButtons();
}

/* ======================================================================= */

FormScriptGenerator::~FormScriptGenerator()
{
  delete ui;
}

void
FormScriptGenerator::sl_fileError(const QString& text_)
{
  QMessageBox::warning(
    this, tr(ProgId::Name), tr(text_.toUtf8()), QMessageBox::Close);
}

void
FormScriptGenerator::sl_dialogShowLogFile_triggered()
{
  dialogShowFile = new DialogShowLogFile();
  dialogShowFile->setWindowFlags(Qt::Dialog);
  dialogShowFile->setModal(true);
  dialogShowFile->show();
}

void
FormScriptGenerator::sl_dialogViewInitBashScript_triggered()
{
  dialogShowFile =
    new DialogShowLogFile(DialogShowFilesModes::Modes::InitScript, this);
  dialogShowFile->setWindowTitle(tr("Show Initialization Bash Scripts"));
  dialogShowFile->setWindowFlags(Qt::Dialog);
  dialogShowFile->setModal(true);
  dialogShowFile->show();
}

void
FormScriptGenerator::sl_dialogViewCreateBashScript_triggered()
{
  dialogShowFile =
    new DialogShowLogFile(DialogShowFilesModes::Modes::CreateScript, this);
  dialogShowFile->setWindowTitle(tr("Show Create Bash Scripts"));
  dialogShowFile->setWindowFlags(Qt::Dialog);
  dialogShowFile->setModal(true);
  dialogShowFile->show();
}

void
FormScriptGenerator::sl_toolButton_ExecScript()
{
  scriptExecution(ExecTypes::Create);
}

void
FormScriptGenerator::sl_toolButton_ExecInitScripts()
{
  scriptExecution(ExecTypes::Init);
}

void
FormScriptGenerator::sl_toolButton_SaveResults()
{
  if (ui->textBrowser_Results->toPlainText().isEmpty()) {
    return;
  }

  SettingsHandler handler_;
  SettingsHandler::Preferences prefs_ = handler_.relodPreferences();
  QFileDialog* dialog_ =
    new QFileDialog(this, tr("Save as"), prefs_.localDataPath, "*.dat");

  QByteArray data_(ui->textBrowser_Results->toPlainText().toStdString().c_str(),
                   ui->textBrowser_Results->toPlainText().size());
  dialog_->saveFileContent(data_, std::move(QString("Untitled.dat")));
  delete dialog_;
}

void
FormScriptGenerator::sl_msgProcessOutput(const QString& line)
{
  ui->textBrowser_Results->append(line);
}

void
FormScriptGenerator::sl_msgProcessTerminate(int exitCode)
{
  if (exitCode == 0) {
    ui->textBrowser_Results->append("Script executed successfully!");
  } else {
    ui->textBrowser_Results->append("Error running script.");
  }
}

void
FormScriptGenerator::setupInitRepoTable()
{
  ui->tabWidget_ScriptGen->setTabText(static_cast<int>(Pages::InitRepos),
                                      tr("Initialization"));
  ui->tabWidget_ScriptGen->setTabText(static_cast<int>(Pages::CreateRepos),
                                      tr("Create"));
  ui->tabWidget_ScriptGen->setTabText(static_cast<int>(Pages::ExecScripts),
                                      tr("Run backup scripts"));

  ui->tabWidget_ScriptGen->setCurrentIndex(0);

  const std::array<int, 6> columnWidths = { 200, 300, 130, 160, 160, 200 };
  for (size_t i = 0; i < columnWidths.size(); ++i) {
    ui->tableWidget_RepoNames->setColumnWidth(static_cast<int>(i),
                                              columnWidths[i]);
  }

  ui->tableWidget_RepoNames->setHorizontalHeaderLabels({ tr("Repository Name"),
                                                         tr("::Archive"),
                                                         tr("Encryption Mode"),
                                                         tr("Append"),
                                                         tr("Make Parents Dir"),
                                                         tr("Storage Quota") });

  ui->tableWidget_RepoNames->setRowCount(0); // clear
  ui->tableWidget_RepoNames->setRowCount(AppTypes::MaxRepositoriesRows);

  for (int row = 0; row < AppTypes::MaxRepositoriesRows; ++row) {
    auto* nameWidget = new NameRepoLineEdit();
    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::Name), nameWidget);

    // ------------------------------------------------------------
    auto* archiveCombo = new QComboBox();
    archiveCombo->setEditable(true);
    archiveCombo->setInsertPolicy(
      QComboBox::NoInsert); // Avoid automatic duplicates.
    archiveCombo->addItems({ "",
                             R"({hostname}-{now:%Y-%m-%dT%H:%M:%S})",
                             R"({hostname}-daily-{now:%Y-%m-%d})",
                             R"({user}@{hostname}-{now:%Y-%m-%d})"
                             R"({hostname}-weekly-{now:%G-week%V})",
                             R"(archive-{now:%s})" });
    archiveCombo->lineEdit()->setPlaceholderText(
      tr("Select or type archive name..."));

    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::Archive), archiveCombo);
    // ------------------------------------------------------------

    auto* encMode = new EncModeComboBox();
    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::EncMode), encMode);

    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::Append), new CheckBox());

    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::MakeDirPath), new QCheckBox());

    ui->tableWidget_RepoNames->setCellWidget(
      row, static_cast<int>(Columns::StoQuota), new StoQuotaLineEdit());
  }
}

void
FormScriptGenerator::sl_setupCreateBackup()
{
  ui->treeWidget_CreateBackup->clear();

  ui->treeWidget_CreateBackup->setScriptName(
    ui->comboBox_ScriptCreateName->currentText().simplified());
  ui->treeWidget_CreateBackup->populate();
}

FormScriptGenerator::TestFileNames::Reasons
FormScriptGenerator::TestFileNames::test(const QString& fn_)
{
  const QString tempNewName_ = Globals::suggestNewFilename(fn_.simplified());
  if (fn_.compare(tempNewName_.simplified()) != 0) { // already exists
    QString info_ =
      QString("New name suggested: <b>%1</b><br><br>Do you agree with this?")
        .arg(Globals::fileName(tempNewName_));
    QMessageBox msgBox;
    msgBox.setText(tr("File name already exists!"));
    msgBox.setInformativeText(tr(info_.toUtf8()));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Save);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (msgBox.exec() == QMessageBox::Save) {
      newName_ = std::move(tempNewName_);
      return Reasons::SaveWithNewname;
    }
    return Reasons::NameProvidedByUser;
  }
  return Reasons::NotExists;
}

void
FormScriptGenerator::sl_genCreateBackupReload()
{
  SettingsHandlerCreate settings_;

  AppTypes::RepoistoryCreateDataMap undo_map_ =
    settings_.read(ui->comboBox_ScriptCreateName->currentText().simplified());

  ui->treeWidget_CreateBackup->clearTree(); // reset tree
  ui->treeWidget_CreateBackup->setScriptName(
    ui->comboBox_ScriptCreateName->currentText().simplified());
  ui->treeWidget_CreateBackup->reloadCreateBackupSettings(undo_map_);
}

void
FormScriptGenerator::sl_saveInitTable()
{
  SettingsHandler setHandler;

  const auto message_ = [](const QString& text_, const QString& info_) {
    QMessageBox msgBox;
    msgBox.setText(tr(text_.toUtf8()));
    msgBox.setInformativeText(tr(info_.toUtf8()));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Save);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    const int ret_ = msgBox.exec();
    return ret_;
  };

  if (ui->lineEdit_DestPath->text().endsWith('/')) {
    QMessageBox::warning(this,
                         tr(ProgId::Name),
                         tr("Destination path names cannot end with /"),
                         QMessageBox::Close);
    ui->lineEdit_DestPath->setFocus();
    return;
  }

  if (setHandler.isScriptExists(
        ui->comboBox_CreateScriptName->currentText().trimmed())) {
    switch (
      message_("A script with this name already exists.",
               "<b>Only choose save if you haven't run the script yet!<b>")) {
      case QMessageBox::Cancel: {
        ui->comboBox_CreateScriptName->setFocus();
        return;
      }
    }
  }

  SettingsHandler::ScriptDefs defs;
  for (int i_ = 0; i_ < ui->tableWidget_RepoNames->rowCount(); ++i_) {
    QLineEdit* name_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Name)));
    if (!name_) {
      break;
    }

    QString archiveText_{};
    if (auto* archiveCombo_ =
          qobject_cast<QComboBox*>(ui->tableWidget_RepoNames->cellWidget(
            i_, static_cast<int>(Columns::Archive)))) {

      archiveText_ = archiveCombo_->currentText().trimmed();

      if (archiveText_.isEmpty()) {
        continue;
      }
    } else {
      // If the cast failed, the widget in the cell is not a QComboBox.
      // This indicates a state error or desynchronization in the assembly of
      // the table.
      qCritical() << "Critical error: Widget on line" << i_
                  << "not a QComboBox.";
      break;
    }

    QComboBox* encmode_ =
      qobject_cast<QComboBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::EncMode)));

    QCheckBox* append_ =
      qobject_cast<QCheckBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Append)));

    QCheckBox* makedirpath_ =
      qobject_cast<QCheckBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::MakeDirPath)));

    QLineEdit* sto_quota_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::StoQuota)));
    auto t_ =
      std::make_tuple(name_->text().trimmed(),
                      archiveText_.trimmed(),
                      encmode_->currentIndex(),
                      (append_->checkState() == Qt::Checked ? 1 : 0),
                      (makedirpath_->checkState() == Qt::Checked ? 1 : 0),
                      sto_quota_->text());

    defs.initDefs_[i_] = t_;
  }

  QByteArray list(ui->treeView_CommonOptions_Init->getCheckedItems());

  GlobalOptions glbOptions_;
  QString coptions_str_{};
  for (auto&& a = list.begin(); a != list.end(); ++a) {
    coptions_str_ += glbOptions_.getOptions(
                       static_cast<GlobalOptions::GlobalOptions_e>((int)*a)) +
                     " ";
  }

  defs.scrName_ = ui->comboBox_CreateScriptName->currentText().trimmed();

  (ui->radioButton_Local->isChecked() ? defs.mode_ = 0 : defs.mode_ = 1);
  defs.destRepo_ = ui->lineEdit_DestPath->text().simplified();
  defs.cmOptions_ = coptions_str_;

  if (QString errorText_{}; !setHandler.testPermissions(errorText_)) {
    sl_fileError(errorText_);
    return;
  }

  if (!setHandler.saveInitScriptDefs(defs)) {
    QMessageBox::critical(this, tr("Warning"), tr("Error writing file"));
    return;
  }
  QMessageBox::information(
    this, tr("Information"), tr("Configuration saved successfully."));
}

void
FormScriptGenerator::sl_loadInitTable()
{
  if (ui->comboBox_CreateScriptName->currentText().isEmpty()) {
    QMessageBox::warning(this,
                         tr(ProgId::Name),
                         tr("Script name not provided!"),
                         QMessageBox::Close);
    ui->comboBox_CreateScriptName->setFocus();
    return;
  }
  SettingsHandler setHandler_;
  auto list_ = std::move(setHandler_.readInitScriptDefs(
    ui->comboBox_CreateScriptName->currentText().trimmed()));

  if (const auto& value_ = Globals::find_<QString, QString>(list_, "A_MODE");
      value_ != nullptr) {
    if (value_.toInt() == 0) {
      ui->radioButton_Local->setChecked(true);
    } else {
      ui->radioButton_Remote->setChecked(true);
    }
  } else {
    return;
  }
  ui->lineEdit_DestPath->setText(
    Globals::find_<QString, QString>(list_, "A_DESTREPO"));

  const auto&& cmoptions_ =
    Globals::find_<QString, QString>(list_, "A_CMOPTIONS");
  ui->treeView_CommonOptions_Init->updateCheckableItem(
    cmoptions_.split(u' ', Qt::SkipEmptyParts));

  for (int i_ = 0; i_ < ui->tableWidget_RepoNames->rowCount(); ++i_) {
    name_value_ = new NameRepoLineEdit();
    name_value_->setText(Globals::findOne_<QString>(
      QString("B_REPONAME_%0").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed()));
    if (name_value_->text().isEmpty()) {
      continue;
    }
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Name), name_value_);

    // combobox
    auto* archiveCombo = new QComboBox();
    archiveCombo->setEditable(true);
    archiveCombo->setInsertPolicy(
      QComboBox::NoInsert); // Avoid automatic duplicates.
    archiveCombo->addItems({ "",
                             R"({hostname}-{now:%Y-%m-%dT%H:%M:%S})",
                             R"({hostname}-daily-{now:%Y-%m-%d})",
                             R"({user}@{hostname}-{now:%Y-%m-%d})"
                             R"({hostname}-weekly-{now:%G-week%V})",
                             R"(archive-{now:%s})" });
    archiveCombo->lineEdit()->setPlaceholderText(
      tr("Select or type archive name..."));
    const QString savedArchive = Globals::findOne_<QString>(
      QStringLiteral("B_ARCHIVE_%1").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed());
    if (savedArchive.isEmpty()) {
      delete archiveCombo; // Evita vazamento de memória antes do continue
      continue;
    }
    archiveCombo->setCurrentText(savedArchive);

    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Archive), archiveCombo);
    // combobox

    encmode_value_ = new EncModeComboBox();
    const auto a_ = Globals::findOne_<QString>(
      QString("B_ENCMODE_%0").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed());
    encmode_value_->setCurrentIndex(a_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::EncMode), encmode_value_);

    //===
    append_value_ = new CheckBox();
    auto chk_ = Globals::findOne_<QString>(
      QString("B_APPEND_%0").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed());
    append_value_->setChecked(chk_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Append), append_value_);

    mkparentdir_value_ = new CheckBox();
    chk_ = Globals::findOne_<QString>(
      QString("B_MKPARENTDIR_%0").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed());
    mkparentdir_value_->setChecked(chk_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::MakeDirPath), mkparentdir_value_);

    stoquota_value_ = new StoQuotaLineEdit();
    stoquota_value_->setText(Globals::findOne_<QString>(
      QString("B_STOQUOTA_%0").arg(i_),
      ui->comboBox_CreateScriptName->currentText().trimmed()));
    if (stoquota_value_->text().isEmpty()) {
      continue;
    }
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::StoQuota), stoquota_value_);
  }
}

void
FormScriptGenerator::sl_genInitScript()
{
  if (ui->comboBox_CreateScriptName->currentText().isEmpty()) {
    return;
  }

  SettingsHandler settings_;
  SettingsHandler::Preferences prefs_ = settings_.relodPreferences();
  Globals glb_;
  auto result = glb_.getValidatedSavePath(
    this,
    tr("Save Script As ..."),
    QString("%0/init_%1")
      .arg(prefs_.localScriptPath,
           ui->comboBox_CreateScriptName->currentText().trimmed()),
    tr("Script (*.sh)"),
    "sh");

  QString fn_{};
  if (!result) {
    return;
  }
  fn_ = *result;

  qDebug() << fn_;

  FormScriptGenerator::TestFileNames testFileNames_;
  switch (testFileNames_.test(fn_)) {
    case TestFileNames::Reasons::SaveWithNewname: {
      bashGen_ = new BashScriptGenerator(
        QString("%0").arg(testFileNames_.newFilename()), this);
      break;
    }
    case TestFileNames::Reasons::NameProvidedByUser: {
      // any other new name provided by the user
      return;
    }
    case TestFileNames::Reasons::NotExists: {
      bashGen_ = new BashScriptGenerator(fn_, this);
    }
  }

  connect(
    bashGen_, SIGNAL(fileError(QString)), this, SLOT(sl_fileError(QString)));

  if (ui->radioButton_Remote->isChecked()) {
    bashGen_->setRepositoryPath("ssh://" +
                                ui->lineEdit_DestPath->text().simplified());
  } else {
    bashGen_->setRepositoryPath(ui->lineEdit_DestPath->text().simplified());
  }

  qDebug() << ui->tableWidget_RepoNames->rowCount();
  for (int i_ = 0; i_ < ui->tableWidget_RepoNames->rowCount(); ++i_) {
    const QLineEdit* name_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Name)));
    if (!name_) {
      qWarning() << QStringLiteral("%0: %1").arg(
        __FUNCTION__, "QLineEdit name_ error. [nullptr]");
      break;
    }

    const QComboBox* archive_ =
      qobject_cast<QComboBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Archive)));
    if (!archive_) {
      qWarning() << QStringLiteral("%0: %1").arg(
        __FUNCTION__, "QComboBox archive_ error. [nullptr]");
      break;
    }

    const QComboBox* encmode_ =
      qobject_cast<QComboBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::EncMode)));

    const QCheckBox* append_ =
      qobject_cast<QCheckBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Append)));

    const QCheckBox* makedirpath_ =
      qobject_cast<QCheckBox*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::MakeDirPath)));

    const QLineEdit* sto_quota_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::StoQuota)));

    auto t_ =
      std::make_tuple(name_->text().trimmed(),
                      archive_->currentText().trimmed(),
                      encmode_->currentIndex(),
                      (append_->checkState() == Qt::Checked ? 1 : 0),
                      (makedirpath_->checkState() == Qt::Checked ? 1 : 0),
                      sto_quota_->text());
    bashGen_->append(i_, t_);
  }

  QByteArray list(ui->treeView_CommonOptions_Init->getCheckedItems());

  // CommonOptions coptions_;
  GlobalOptions glbOptions_;
  QString coptions_str_{};
  for (auto&& a = list.begin(); a != list.end(); ++a) {
    coptions_str_ += glbOptions_.getOptions(
                       static_cast<GlobalOptions::GlobalOptions_e>((int)*a)) +
                     " ";
  }
  bashGen_->setCommons(coptions_str_);
  bashGen_->setMainModel(BashScriptGenerator::MainModel::Initialization);
  bashGen_->writeScript();
  Globals::setFilePermissions(fn_, 0755);
}

void
FormScriptGenerator::sl_getDestinationPath()
{
  QFileDialog dlg_;
  QString fn_ = dlg_.getExistingDirectory(this,
                                          tr("Local Destination Path"),
                                          QDir::homePath(),
                                          QFileDialog::ShowDirsOnly |
                                            QFileDialog::DontResolveSymlinks);
  dlg_.close();
  if (fn_.isEmpty()) {
    QMessageBox::warning(
      this, tr("Warning"), tr("The directory name cannot be left empty. "));
    return;
  }
  ui->lineEdit_DestPath->setText(fn_.simplified());
}

void
FormScriptGenerator::sl_genCreateSaveSettings()
{
  SettingsHandlerCreate settings_;
  auto checkedItems_ = ui->treeWidget_CreateBackup->getCheckedItems();
  if (!settings_.save(ui->comboBox_ScriptCreateName->currentText(),
                      checkedItems_)) {
    QMessageBox::critical(this, tr("Warning"), tr("Error writing file"));
    return;
  }
  QMessageBox::information(
    this, tr("Information"), tr("Configuration saved successfully."));
}

void
FormScriptGenerator::sl_genCreateBackup()
{
  if (ui->comboBox_ScriptCreateName->currentText().isEmpty()) {
    return;
  }

  SettingsHandler settings_;
  SettingsHandler::Preferences prefs_ = settings_.relodPreferences();
  Globals glb_;
  auto result = glb_.getValidatedSavePath(
    this,
    tr("Save Script As ..."),
    QString("%0/%1_%2")
      .arg(prefs_.localScriptPath,
           "create",
           ui->comboBox_ScriptCreateName->currentText()),
    tr("Script (*.sh)"),
    "sh");

  QString fn_{};
  if (!result) {
    return;
  }
  fn_ = *result;

  FormScriptGenerator::TestFileNames testFileNames_;
  switch (testFileNames_.test(fn_)) {
    case TestFileNames::Reasons::SaveWithNewname: {
      auto checkedItems = ui->treeWidget_CreateBackup->getCheckedItems();
      BashScriptGenerator gen_(testFileNames_.newFilename(), checkedItems);
      gen_.setMainModel(BashScriptGenerator::MainModel::Create);
      gen_.writeScript();
      Globals::setFilePermissions(testFileNames_.newFilename(), 0755);
      break;
    }
    case TestFileNames::Reasons::NameProvidedByUser: {
      // any other new name provided by the user
      break;
    }
    case TestFileNames::Reasons::NotExists: {
      auto checkedItems = ui->treeWidget_CreateBackup->getCheckedItems();
      BashScriptGenerator gen_(fn_, checkedItems);
      gen_.setMainModel(BashScriptGenerator::MainModel::Create);
      gen_.writeScript();
      Globals::setFilePermissions(fn_, 0755);
    }
  }
}

void
FormScriptGenerator::scriptExecution(ExecTypes type)
{
  ui->textBrowser_Results->clear();

  const QModelIndex index = ui->listView_ExecScripts->currentIndex();
  Q_ASSERT(!index.isValid());

  if (!index.isValid()) {
    QMessageBox::warning(this,
                         tr(ProgId::Name),
                         tr("Script was not selected."),
                         QMessageBox::Close);
    return;
  }

  const QString scriptName = index.data(Qt::DisplayRole).toString();

  QMessageBox msgBox;
  msgBox.setText(type == ExecTypes::Init ? tr("Repository Initialization")
                                         : tr("Backup Execution"));
  msgBox.setInformativeText(
    tr("Confirm the execution of script?: <b>%0</b>").arg(scriptName));
  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::No);
  if (msgBox.exec() == QMessageBox::No) {
    return;
  }

  if (!m_scriptRunner) {
    m_scriptRunner = new BashScriptRunner(this);

    connect(m_scriptRunner,
            &BashScriptRunner::sig_bashOutput,
            this,
            &FormScriptGenerator::sl_msgProcessOutput);

    connect(m_scriptRunner,
            &BashScriptRunner::sig_bashFinished,
            this,
            &FormScriptGenerator::sl_msgProcessTerminate);
  }

  if (m_scriptRunner->isRunning()) {
    [[maybe_unused]] auto ret_ = QMessageBox::information(
      this, "Information", "Please wait for the current script to finish.");
    return;
  }

  SettingsHandler settings_;
  SettingsHandler::Preferences prefs_ = settings_.relodPreferences();
  const QString dest_ =
    QString("%0/%1_%2.sh")
      .arg(prefs_.localScriptPath,
           (type == ExecTypes::Create ? QStringLiteral("create")
                                      : QStringLiteral("init")),
           scriptName);

  // run script thread
  m_scriptRunner->runScript(dest_, {});
}

void
FormScriptGenerator::createToolBar()
{
  QVBoxLayout* mainLayout_ = new QVBoxLayout(ui->frame_ToolBar);
  mainLayout_->addWidget(toolBarSettings);
  toolBarSettings->setFloatable(false);
  toolBarSettings->setMovable(false);
  toolButtonClear = new QToolButton;
  toolButtonClear->setIcon(QIcon::fromTheme("edit-clear"));
  toolButtonClear->setToolTip(tr("Clear all fields."));

  toolBarSettings->addWidget(toolButtonClear);
}

void
FormScriptGenerator::setupCharValidation()
{
  static QRegularExpression regExp_(R"(^[a-z0-9_]+$)");
  auto* validator_ = new QRegularExpressionValidator(regExp_, this);
  ui->comboBox_ScriptCreateName->setValidator(validator_);
  ui->comboBox_CreateScriptName->setValidator(validator_);

  static QRegularExpression regExp1_(R"(^[A-Za-z0-9_/@:.-]+$)");
  ui->lineEdit_DestPath->setValidator(
    new QRegularExpressionValidator(regExp1_, this));
}

void
FormScriptGenerator::setupTextBrowser()
{
  const int fontId_ = QFontDatabase::addApplicationFont(R"(:/Fonts/COUR.TTF)");
  const QString fontFamily_ =
    QFontDatabase::applicationFontFamilies(fontId_).at(0);
  const QFont font12_(fontFamily_, 12);
  ui->textBrowser_Results->setFont(font12_);

  QPalette pal = ui->textBrowser_Results->palette();
  pal.setColor(QPalette::Text, Qt::green);
  ui->textBrowser_Results->setPalette(pal);
}

void
FormScriptGenerator::setupButtons()
{
  connect(ui->tabWidget_ScriptGen,
          &QTabWidget::currentChanged,
          this,
          [this](int index) {
            switch (index) {
              case 0: {
                break;
              }
              case 1: {
                QPointer<comboBox_ScriptCreateName_P> cb_ =
                  new comboBox_ScriptCreateName_P(this);
                cb_->refresh();
                break;
              }
              case 2: {
                break;
              }
            }
          });

  // page #1
  connect(
    ui->radioButton_Local, &QRadioButton::clicked, this, [this](bool clicked) {
      if (clicked) {
        ui->toolButton_DestPath->setEnabled(true);
        ui->lineEdit_DestPath->setFocus();
        ui->lineEdit_DestPath->clear();
        ui->lineEdit_DestPath->setPlaceholderText(tr("/path/to/repo"));
      }
    });

  connect(
    ui->radioButton_Remote, &QRadioButton::clicked, this, [this](bool clicked) {
      if (clicked) {
        ui->lineEdit_DestPath->setFocus();
        ui->lineEdit_DestPath->clear();
        ui->lineEdit_DestPath->setPlaceholderText(
          tr("user@hostname[:port]/path/to/repo"));
        ui->toolButton_DestPath->setEnabled(false);
      }
    });

  connect(ui->toolButton_ScrOpen,
          &QToolButton::clicked,
          this,
          [this](bool clicked) { ; });

  connect(ui->toolButton_ScrOpen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_loadInitTable()));
  connect(ui->toolButton_ScrGen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_genInitScript()));
  connect(ui->toolButton_ScrSave,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_saveInitTable()));

  connect(ui->toolButton_DestPath,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_getDestinationPath()));

  /* Page 2 */
  connect(ui->toolButton_CreateOpen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_setupCreateBackup()));

  connect(ui->toolButton_CreateSave,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_genCreateSaveSettings()));

  connect(ui->toolButton_CreateGen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_genCreateBackup()));

  connect(ui->toolButton_CreateReload,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_genCreateBackupReload()));

  connect(ui->toolButton_ShowLog,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_dialogShowLogFile_triggered()));

  connect(ui->toolButton_ViewInitScript,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_dialogViewInitBashScript_triggered()));

  connect(ui->toolButton_ViewCreateScript,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_dialogViewCreateBashScript_triggered()));

  connect(ui->toolButton_ExecScipts,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_toolButton_ExecScript()));

  connect(ui->toolButton_RefreshList,
          &QToolButton::clicked,
          ui->listView_ExecScripts,
          &ListviewExecScripts_P::refresh);

  connect(ui->toolButton_ExecScipts,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_toolButton_ExecScript()));

  connect(ui->toolButton_ExecInitScripts,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_toolButton_ExecInitScripts()));

  connect(ui->toolButton_SaveResults,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_toolButton_SaveResults()));

  connect(ui->toolButton_ResultsClear,
          &QToolButton::clicked,
          ui->textBrowser_Results,
          &QTextBrowser::clear);
}
