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
#include "formscriptgenerator.h"
#include "src/ui_formscriptgenerator.h"

#include "global_options.h"
#include "global_tools.h"
#include "progid.h"
#include "settingshandler.h"
#include "types_global.h"

/*!
 * \brief FormScriptGenerator::FormScriptGenerator
 * \param parent
 */
FormScriptGenerator::FormScriptGenerator(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormScriptGenerator)
  , mainWindowToolBar(new QMainWindow)
  , toolBarSettings(new QToolBar)
  , newItem(new QStandardItem)
{
  ui->setupUi(this);

  setupInitRepoTable();
  setupCreateBackup();

  /*!
   * \brief createToolBar
   * \note Future Use (maybe)
   */
#if 0
  createToolBar();
  connect(toolButtonClear,
          &QToolButton::clicked,
          this,
          [this]() {
              if (ui->tab_Init->isVisible()) {
                  qDebug() << "INIT";
              } else if (ui->tab_Create->isVisible()) {
                  qDebug() << "create";
              }
          }
    );
#endif

  QRegularExpression regExp_(R"([a-z0-9_]*)"); // valid chars
  ui->lineEdit_ScriptName->setValidator(
    new QRegularExpressionValidator(regExp_, this));

  QRegularExpression regExp1_(R"([A-Za-z0-9_/@:.]*)"); // valid chars
  ui->lineEdit_DestPath->setValidator(
    new QRegularExpressionValidator(regExp1_, this));

  setupButtons();
}

/* ======================================================================= */

/*!
 * \brief FormScriptGenerator::~FormScriptGenerator
 */
FormScriptGenerator::~FormScriptGenerator()
{
  delete ui;
}

/*!
 * \private slot
 * \brief FormScriptGenerator::fileError
 */
void
FormScriptGenerator::fileError(const QString& text_)
{
  qWarning("%s", text_.toStdString().c_str());

  QMessageBox::warning(
    this, tr(ProgId::Name), tr(text_.toUtf8()), QMessageBox::Close);
}

/*!
 * \private slot
 * \brief FormScriptGenerator::dialogShowLogFile_triggered
 */
void
FormScriptGenerator::dialogShowLogFile_triggered()
{
  dialogShowFile = new DialogShowLogFile;
  dialogShowFile->setWindowFlags(Qt::Dialog);
  dialogShowFile->setModal(true);
  dialogShowFile->show();
}

/*!
 * \private slot
 * \brief FormScriptGenerator::setupInitTable
 */
void
FormScriptGenerator::setupInitRepoTable()
{
  ui->tabWidget_ScriptGen->setTabText((int)Pages::InitRepos,
                                      tr("Initialization"));
  ui->tabWidget_ScriptGen->setTabText((int)Pages::CreateRepos, tr("Create"));

  ui->tabWidget_ScriptGen->setCurrentIndex(0); // set Init page

  ui->tableWidget_RepoNames->setColumnWidth(0, 200);
  ui->tableWidget_RepoNames->setColumnWidth(1, 200);
  ui->tableWidget_RepoNames->setColumnWidth(2, 130);
  ui->tableWidget_RepoNames->setColumnWidth(3, 160);
  ui->tableWidget_RepoNames->setColumnWidth(4, 160);
  ui->tableWidget_RepoNames->setColumnWidth(5, 200);
  ui->tableWidget_RepoNames->setHorizontalHeaderLabels(
    QStringList() << tr("Repository Name") << tr("::Archive")
                  << tr("Encryption Mode") << tr("Append")
                  << tr("Make Parents Dir") << tr("Storage Quota"));

  for (int i_ = 0; i_ < AppTypes::MaxRepositoriesRows; ++i_) {
    ui->tableWidget_RepoNames->insertRow(ui->tableWidget_RepoNames->rowCount());

    name_value_ = new NameRepoLineEdit();
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Name), name_value_);

    archive_value_ = new ArchiveLineEdit();
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Archive), archive_value_);

    encmode_value_ = new EncModeComboBox;
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::EncMode), encmode_value_);

    append_value_ = new CheckBox();
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Append), append_value_);

    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::MakeDirPath), new QCheckBox);

    stoquota_value_ = new StoQuotaLineEdit();
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::StoQuota), stoquota_value_);
  }
}

/*!
 * \private slot
 * \brief FormScriptGenerator::setupCreateBackup
 */
void
FormScriptGenerator::setupCreateBackup()
{
  ui->treeWidget_CreateBackup->clear();

  ui->treeWidget_CreateBackup->setScriptName(
    ui->lineEdit_ScriptCreateName->text().simplified());
  ui->treeWidget_CreateBackup->populate();
}

/*!
 * \private
 * \brief FormScriptGenerator::TestFileNames::test
 * \param fn_
 * \return
 */
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

/*!
 * \brief FormScriptGenerator::genCreateBackupReload
 */
void
FormScriptGenerator::genCreateBackupReload()
{
  SettingsHandlerCreate settings_;

  AppTypes::RepoistoryCreateDataMap undo_map_ =
    settings_.read(ui->lineEdit_ScriptCreateName->text().simplified());

  ui->treeWidget_CreateBackup->clearTree(); // reset tree
  ui->treeWidget_CreateBackup->setScriptName(
    ui->lineEdit_ScriptCreateName->text().simplified());
  ui->treeWidget_CreateBackup->reloadCreateBackupSettings(undo_map_);
}

/*!
 * \private slot
 * \brief FormScriptGenerator::saveInitTable
 */
void
FormScriptGenerator::saveInitTable()
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

  if (setHandler.isScriptExists(ui->lineEdit_ScriptName->text().simplified())) {
    switch (
      message_("A script with this name already exists.",
               "<b>Only choose save if you haven't run the script yet!<b>")) {
      case QMessageBox::Cancel: {
        ui->lineEdit_ScriptName->setFocus();
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

    QLineEdit* archive_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Archive)));
    if (!archive_) {
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
      std::make_tuple(name_->text().simplified(),
                      archive_->text().simplified(),
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

  defs.scrName_ = ui->lineEdit_ScriptName->text().simplified();

  (ui->radioButton_Local->isChecked() ? defs.mode_ = 0 : defs.mode_ = 1);
  defs.destRepo_ = ui->lineEdit_DestPath->text().simplified();
  defs.cmOptions_ = coptions_str_;

  if (QString errorText_{}; !setHandler.testPermissions(errorText_)) {
    fileError(errorText_);
    return;
  }

  if (!setHandler.saveInitScriptDefs(defs)) {
    QMessageBox::critical(this, tr("Warning"), tr("Error writing file"));
    return;
  }
  QMessageBox::information(
    this, tr("Information"), tr("Configuration saved successfully."));
}

/*!
 * \private slot
 * \brief FormScriptGenerator::loadInitTable
 */
void
FormScriptGenerator::loadInitTable()
{
  if (ui->lineEdit_ScriptName->text().simplified().isEmpty()) {
    QMessageBox::warning(this,
                         tr(ProgId::Name),
                         tr("Script name not provided!"),
                         QMessageBox::Close);
    ui->lineEdit_ScriptName->setFocus();
    return;
  }
  SettingsHandler setHandler_;
  auto list_ = std::move(setHandler_.readInitScriptDefs(
    ui->lineEdit_ScriptName->text().simplified()));

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
    name_value_->setText(
      Globals::findOne_<QString>(QString("B_REPONAME_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified()));
    if (name_value_->text().isEmpty()) {
      continue;
    }
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Name), name_value_);

    archive_value_ = new ArchiveLineEdit();
    archive_value_->setText(
      Globals::findOne_<QString>(QString("B_ARCHIVE_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified()));
    if (archive_value_->text().isEmpty()) {
      continue;
    }
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Archive), archive_value_);

    encmode_value_ = new EncModeComboBox();
    const auto a_ =
      Globals::findOne_<QString>(QString("B_ENCMODE_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified());
    encmode_value_->setCurrentIndex(a_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::EncMode), encmode_value_);

    //===
    append_value_ = new CheckBox();
    auto chk_ =
      Globals::findOne_<QString>(QString("B_APPEND_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified());
    append_value_->setChecked(chk_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::Append), append_value_);

    mkparentdir_value_ = new CheckBox();
    chk_ =
      Globals::findOne_<QString>(QString("B_MKPARENTDIR_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified());
    mkparentdir_value_->setChecked(chk_.toInt());
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::MakeDirPath), mkparentdir_value_);

    stoquota_value_ = new StoQuotaLineEdit();
    stoquota_value_->setText(
      Globals::findOne_<QString>(QString("B_STOQUOTA_%0").arg(i_),
                                 ui->lineEdit_ScriptName->text().simplified()));
    if (stoquota_value_->text().isEmpty()) {
      continue;
    }
    ui->tableWidget_RepoNames->setCellWidget(
      i_, static_cast<int>(Columns::StoQuota), stoquota_value_);
  }
}

/*!
 * \private slot
 * \brief FormScriptGenerator::genInitTable
 */
void
FormScriptGenerator::genInitScript()
{
  if (ui->lineEdit_ScriptName->text().simplified().isEmpty()) {
    return;
  }

  QFileDialog dlg_;
  dlg_.setFileMode(QFileDialog::ExistingFiles);
  dlg_.setViewMode(QFileDialog::Detail);
  dlg_.setNameFilter("*.sh");
  dlg_.setDirectory(QDir::homePath());

  QString fn_ = dlg_.getSaveFileName(
    this,
    tr("Save Script As ..."),
    QString("init_%1").arg(ui->lineEdit_ScriptName->text().simplified()),
    tr("Script (*.sh)"));
  dlg_.close();
  fn_ += ".sh";

  if (fn_.isEmpty()) {
    QMessageBox::warning(
      this, tr("Warning"), tr("The file name cannot be left empty. "));
    return;
  }

  FormScriptGenerator::TestFileNames testFileNames_;
  switch (testFileNames_.test(fn_)) {
    case TestFileNames::Reasons::SaveWithNewname: {
      shgen_ =
        new ScriptGen(QString("%0").arg(testFileNames_.newFilename()), this);
      break;
    }
    case TestFileNames::Reasons::NameProvidedByUser: {
      // any other new name provided by the user
      return;
    }
    case TestFileNames::Reasons::NotExists: {
      shgen_ = new ScriptGen(fn_, this);
    }
  }

  connect(shgen_, SIGNAL(fileError(QString)), this, SLOT(fileError(QString)));
  if (ui->radioButton_Remote->isChecked()) {
    shgen_->setRepositoryPath("ssh://" +
                              ui->lineEdit_DestPath->text().simplified());
  } else {
    shgen_->setRepositoryPath(ui->lineEdit_DestPath->text().simplified());
  }

  for (int i_ = 0; i_ < ui->tableWidget_RepoNames->rowCount(); ++i_) {
    QLineEdit* name_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Name)));
    if (!name_) {
      break;
    }

    QLineEdit* archive_ =
      qobject_cast<QLineEdit*>(ui->tableWidget_RepoNames->cellWidget(
        i_, static_cast<int>(Columns::Archive)));
    if (!archive_) {
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
      std::make_tuple(name_->text().simplified(),
                      archive_->text().simplified(),
                      encmode_->currentIndex(),
                      (append_->checkState() == Qt::Checked ? 1 : 0),
                      (makedirpath_->checkState() == Qt::Checked ? 1 : 0),
                      sto_quota_->text());

    shgen_->append(i_, t_);
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

  shgen_->sg_fnc_init_repos(coptions_str_);
  Globals::setFilePermissions(fn_, 0755);
}

/*!
 * \private slot
 * \brief FormScriptGenerator::genCreateSaveSettings
 * \note Saves configuration
 */
void
FormScriptGenerator::genCreateSaveSettings()
{
  SettingsHandlerCreate settings_;
  auto checkedItems_ = ui->treeWidget_CreateBackup->getCheckedItems();
  if (!settings_.save(ui->lineEdit_ScriptCreateName->text().simplified(),
                      checkedItems_)) {
    QMessageBox::critical(this, tr("Warning"), tr("Error writing file"));
    return;
  }
  QMessageBox::information(
    this, tr("Information"), tr("Configuration saved successfully."));
}

/*!
 * \private slot
 * \brief FormScriptGenerator::genCreateBackup
 */
void
FormScriptGenerator::genCreateBackup()
{
  if (ui->lineEdit_ScriptCreateName->text().simplified().isEmpty()) {
    return;
  }

  QFileDialog dlg_;
  dlg_.setFileMode(QFileDialog::ExistingFiles);
  dlg_.setViewMode(QFileDialog::Detail);
  dlg_.setNameFilter("*.sh");
  dlg_.setDirectory(QDir::homePath());

  QString fn_ = dlg_.getSaveFileName(
    this,
    tr("Save Script As ..."),
    QString("%1_%2").arg("create",
                         ui->lineEdit_ScriptCreateName->text().simplified()),
    tr("Script (*.sh)"));
  dlg_.close();
  fn_ += ".sh";

  if (fn_.isEmpty()) {
    QMessageBox::warning(
      this, tr("Warning"), tr("The file name cannot be left empty. "));
    return;
  }

  FormScriptGenerator::TestFileNames testFileNames_;
  switch (testFileNames_.test(fn_)) {
    case TestFileNames::Reasons::SaveWithNewname: {
      auto checkedItems = ui->treeWidget_CreateBackup->getCheckedItems();
      ScriptGen gen_(testFileNames_.newFilename());
      gen_.sg_fnc_create(checkedItems);
      Globals::setFilePermissions(testFileNames_.newFilename(), 0755);
      return;
    }
    case TestFileNames::Reasons::NameProvidedByUser: {
      // any other new name provided by the user
      return;
    }
    case TestFileNames::Reasons::NotExists: {
      auto checkedItems = ui->treeWidget_CreateBackup->getCheckedItems();
      ScriptGen gen_(fn_);
      gen_.sg_fnc_create(checkedItems);
      Globals::setFilePermissions(fn_, 0755);
    }
  }
}

/*!
 * \private
 * \brief FormScriptGenerator::createToolBar
 * \note Future use
 */
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

/*!
 * \private
 * \brief setupToolButtons
 */
void
FormScriptGenerator::setupButtons()
{
  connect(
    ui->radioButton_Local, &QRadioButton::clicked, this, [this](bool clicked) {
      if (clicked) {
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
      }
    });

  connect(ui->toolButton_ScrOpen,
          &QToolButton::clicked,
          this,
          [this](bool clicked) { ; });

  connect(
    ui->toolButton_ScrOpen, SIGNAL(clicked(bool)), this, SLOT(loadInitTable()));
  connect(
    ui->toolButton_ScrGen, SIGNAL(clicked(bool)), this, SLOT(genInitScript()));
  connect(
    ui->toolButton_ScrSave, SIGNAL(clicked(bool)), this, SLOT(saveInitTable()));

  connect(ui->toolButton_CreateOpen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(setupCreateBackup()));

  connect(ui->toolButton_CreateSave,
          SIGNAL(clicked(bool)),
          this,
          SLOT(genCreateSaveSettings()));

  connect(ui->toolButton_CreateGen,
          SIGNAL(clicked(bool)),
          this,
          SLOT(genCreateBackup()));

  connect(ui->toolButton_CreateReload,
          SIGNAL(clicked(bool)),
          this,
          SLOT(genCreateBackupReload()));

  connect(ui->toolButton_ShowLog,
          SIGNAL(clicked(bool)),
          this,
          SLOT(dialogShowLogFile_triggered()));
}
