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

#include "formlistings.h"
#include "src/ui_formlistings.h"

#include "QRadioButton"
#include "QSettings"
#include <QComboBox>
#include <QFileDialog>
#include <QFontDatabase>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QOverload>
#include <QRadioButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringLiteral>
#include <QTreeWidgetItem>

#include "borginfocommandparser.h"
#include "borgrunnerthread.h"
#include "global_tools.h"
#include "progid.h"
#include "settingshandler.h"

/*!
 * \brief Listing form constructor.
 *
 * Initializes the interface from the .ui file (ui->setupUi(this)),
 * sets the initial page of ui->toolBox_SearchAndListing to "Search",
 * connects the text change signal of ui->comboBox_ListScriptsNames to
 * always return to the search page, and calls setup functions for:
 * - tool buttons (setupToolButtons);
 * - repository tree widget (setupTreeWidgetRepos);
 * - text editor font (setupTextEditFont).
 * Finally, populates ui->treeView_PatDesc with the initial data.
 */
FormListings::FormListings(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormListings)
  , borgError_(false)
{
  ui->setupUi(this);

  setupToolButtons();
  setupTreeWidgetRepos();
  setupTextEditFont();

  ui->treeView_PatDesc->populate();
}

/*!
 * \brief FormListings::~FormListings
 */
FormListings::~FormListings()
{
  delete ui;
}

/*!
 * \brief Defines the search pattern and executes a Borg::Info command with it.
 *
 * This function:
 * - populates ui->lineEdit_Pattern with the specified pattern;
 * - creates a BorgThreadRunner with the Borg::Info command using this pattern;
 * - connects the output to ui->textBrowser_Info (displaying the raw Borg info);
 * - connects errors and thread termination to update the interface and clean up
 * resources.
 */
void
FormListings::sl_setPattern(const QString& pattern)
{
  ui->lineEdit_Pattern->setText(pattern);

  Globals gTools_;
  QStringList args_;
  args_ << pattern;
  auto* thread_ =
    new BorgThreadRunner(BorgThreadRunner::BorgCommands::Info, args_, this);
  connect(thread_,
          &BorgThreadRunner::sig_borgOutput,
          this,
          [this, &gTools_, pattern](const QString& outputBlock) {
            const QStringList lines_ =
              outputBlock.split('\n', Qt::SkipEmptyParts);
            for (const QString& line_ : lines_) {
              const QString trimmed_ = line_.trimmed();
              if (!trimmed_.isEmpty()) {
                {
                  ui->textBrowser_Info->setText(outputBlock);
                }
              }
            } // end-for
          });
  connect(thread_,
          &BorgThreadRunner::sig_borgError,
          this,
          [this](const QString& errorLine) {
            ui->textBrowser_Info->setText(
              QString("Error while listing %1: %2").arg(errorLine.trimmed()));
          });
  connect(thread_, &QObject::destroyed, this, [thread_]() {
    thread_->deleteLater();
  });
  if (!thread_->isRunning()) {
    thread_->start();
  }
}

/*!
 * \brief Lists the repositories and their associated files from the selected
 * script.
 *
 * This function:
 * - checks if a script has been selected in ui->comboBox_ListScriptsNames;
 * - clears auxiliary panels (info, default, and treeWidget_Repos);
 * - gets a list of repositories via getScriptNames();
 * - loads script definitions using SettingsHandler and Globals;
 * - creates threads to list each repository with Borg::List and populates
 * ui->treeWidget_Repos with the files.
 */
void
FormListings::sl_listRepoNames()
{
  if (ui->comboBox_ListScriptsNames->currentText().isEmpty()) {
    QMessageBox::warning(this, tr("Warning"), tr("Script name not provided!"));
    ui->comboBox_ListScriptsNames->setFocus();
    return;
  }

  ui->textBrowser_Info->clear();
  ui->lineEdit_Pattern->clear();
  ui->treeWidget_Repos->clearTree();

  QStringList repoNames_ = getScriptNames();
  ui->treeWidget_Repos->addRepository(
    ui->comboBox_ListScriptsNames->currentText());

  Globals gTools_;
  SettingsHandler sHandler_;
  AppTypes::NamedValueList list_ =
    sHandler_.readInitScriptDefs(ui->comboBox_ListScriptsNames->currentText());
  auto pairs_ = gTools_.findRepoAndArchive(list_);

  for (const auto& it_ : std::as_const(pairs_.repoAndArchives)) {
    const QString repoPath_ = QString("%1/%2").arg(pairs_.destRepo, it_.first);
    const QString archive_ = it_.second;
    QStringList args_;
    args_ << repoPath_ << R"(--format={archive}{NL})";
    auto* thread_ =
      new BorgThreadRunner(BorgThreadRunner::BorgCommands::List, args_, this);
    connect(thread_,
            &BorgThreadRunner::sig_borgOutput,
            this,
            [this, repoPath_, archive_](const QString& outputBlock) {
              const QStringList lines_ =
                outputBlock.split('\n', Qt::SkipEmptyParts);
              for (const QString& line_ : lines_) {
                const QString trimmed_ = line_.trimmed();
                if (!trimmed_.isEmpty()) {
                  ui->treeWidget_Repos->addArchive(
                    QString("%1::%2").arg(repoPath_, trimmed_));
                }
              }
            });
    connect(thread_,
            &BorgThreadRunner::sig_borgError,
            this,
            [this, repoPath_](const QString& errorLine) {
              ui->treeWidget_Repos->addArchive(
                QString("Error while listing %1: %2")
                  .arg(repoPath_, errorLine.trimmed()));
            });
    connect(thread_, &QObject::destroyed, this, [thread_]() {
      thread_->deleteLater();
    });
    if (!thread_->isRunning()) {
      thread_->start();
    }
  } // for
}

/*!
 * \private
 * \brief Executes the search using the pattern specified in the search edit.
 *
 * This function:
 * - extracts a pattern (--pattern '...') from the input text, when present;
 * - builds a list of arguments for the Borg command (list);
 * - creates and starts a BorgThreadRunner to execute the search in the
 *   background;
 * - displays the output and errors in ui->textEdit_Results, and indicates the
 *   end of execution.
 */
void
FormListings::sl_execSearch()
{
  // ----------------------------------------------------------------------
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
  // ----------------------------------------------------------------------

  ui->textEdit_Results->clear();

  const QString input_ = std::move(ui->lineEdit_Pattern->text().simplified());

  if (input_.isEmpty()) {
    QMessageBox::warning(this, tr("Warning"), tr("Pattern not provided!"));
    ui->comboBox_ListScriptsNames->setFocus();
    return;
  }

  ui->toolBox_SearchAndListing->setCurrentIndex(Pages::Results);

  static QRegularExpression re(R"(^(.+?)\s+(--format=.*)$)");
  QRegularExpressionMatch match = re.match(std::move(input_));

  QStringList args_{};

  if (match.hasMatch()) {
    args_.append(std::move(match.captured(1)));
    args_.append(std::move(match.captured(2)));
  } else {
    args_.append(std::move(input_.section(' ', 0, 0)));
    args_.append(std::move(extractPatterns(input_)));
  }

#if 0
  for (auto& a : args_) {
    qDebug() << "> " << a;
  }
#endif

  auto* thread_ = new BorgThreadRunner(
    BorgThreadRunner::BorgCommands::List, std::move(args_), this);
  Q_ASSERT(thread_);

  connect(
    thread_,
    &BorgThreadRunner::sig_borgOutput,
    this,
    [=, this](const QString& line) { ui->textEdit_Results->append(line); });
  connect(thread_,
          &BorgThreadRunner::sig_borgError,
          this,
          [=, this](const QString& err) {
            ui->textEdit_Results->append("[ERROR] " + err);
          });
  connect(thread_,
          &BorgThreadRunner::sig_borgFinished,
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
 * \brief Gets a list of repository names from the current settings.
 *
 * Reads a configuration group in QSettings, based on the selected script
 * in ui->comboBox_ListScriptsNames, and builds a list of strings with
 * full repository paths in the format "destRepo/repo::archive".
 * If the group does not exist or there are no valid entries, returns an empty
 * list.
 */
QStringList
FormListings::getScriptNames()
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  const QString completeGroupName_ =
    QString("INITREPO_%0")
      .arg(std::move(ui->comboBox_ListScriptsNames->currentText()));

  QStringList repoNames_;
  if (settings_.childGroups().contains(completeGroupName_)) {
    settings_.beginGroup(completeGroupName_);
    destRepo_ = std::move(settings_.value("A_DESTREPO").toString());
    for (int i = 0; i < 20; ++i) {
      const QString rName_ =
        std::move(settings_.value(QString("B_REPONAME_%1").arg(i)).toString());
      const QString rArchive_ =
        std::move(settings_.value(QString("B_ARCHIVE_%1").arg(i)).toString());

      if (rArchive_.isEmpty()) {
        continue;
      }
      const QString repoName_ = std::move(rName_ + "::" + rArchive_);

      if (!repoName_.isEmpty()) {
        repoNames_.append(std::move(destRepo_ + "/" + repoName_));
      }
    }
    settings_.endGroup();
  }
  return repoNames_;
}

// Setups
/*!
 * \brief Configures the tool button signals.
 *
 * Connects the buttons ui->toolButton_Exec, ui->toolButton_ExecSearch
 * and ui->toolButtonSaveToFile to their respective slots or lambdas, to:
 * - execute the repository listing;
 * - execute the search;
 * - save the content of ui->textEdit_Results to a text file.
 */
void
FormListings::setupToolButtons()
{
  ui->toolBox_SearchAndListing->setCurrentIndex(Pages::Search);
  connect(ui->comboBox_ListScriptsNames,
          &QComboBox::currentTextChanged,
          this,
          [this](const QString&) {
            ui->toolBox_SearchAndListing->setCurrentIndex(Pages::Search);
          });

  connect(
    ui->toolButton_Exec, SIGNAL(clicked(bool)), this, SLOT(sl_listRepoNames()));

  connect(ui->toolButton_ExecSearch,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_execSearch()));
  connect(ui->toolButtonSaveToFile, &QToolButton::clicked, this, [this]() {
    if (ui->textEdit_Results->toPlainText().isEmpty()) {
      QMessageBox::information(
        this, tr("Information"), tr("No listings were generated."));
      return;
    }

    QFileDialog dlg_;
    dlg_.setFileMode(QFileDialog::ExistingFiles);
    dlg_.setViewMode(QFileDialog::Detail);
    dlg_.setNameFilter(R"(*.txt)");
    dlg_.setDirectory(QDir::homePath());

    const QString fn_ = dlg_.getSaveFileName(
      this, tr("Save File As"), QDir::homePath(), tr("Listing (*.txt)"));
    if (fn_.isEmpty()) {
      const int ret_ = QMessageBox::warning(this,
                                            ProgId::Name,
                                            tr("File name was not provided!"),
                                            QMessageBox::Cancel);
      dlg_.close();
      return;
    }
    QFile file_(
      (!fn_.contains(R"(.txt)") ? QString(R"(%0.txt)").arg(fn_) : fn_));
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
 * \brief Configures the pattern signal in treeWidget_Repos.
 *
 * Connects the setPattern(const QString&) signal from ui->treeWidget_Repos
 * to the setPattern(const QString&) slot of this class, so that changes
 * to the search pattern are correctly passed to the form.
 */
void
FormListings::setupTreeWidgetRepos()
{
  connect(ui->treeWidget_Repos,
          SIGNAL(sig_setPattern(const QString&)),
          this,
          SLOT(sl_setPattern(const QString&)));
}

/*! \brief Configures fixed fonts for textEdit_Results and textBrowser_Info.
 *
 * This function loads the monospaced font COUR.TTF from the resources folder,
 * obtains the font family and sets specific sizes in the text areas:
 * - ui->textEdit_Results: 12-point font.
 * - ui->textBrowser_Info: 10-point font.
 */
void
FormListings::setupTextEditFont()
{
  fontId_ = QFontDatabase::addApplicationFont(R"(:/Fonts/COUR.TTF)");
  fontFamily_ = QFontDatabase::applicationFontFamilies(fontId_).at(0);
  const QFont font12_(fontFamily_, 12);
  const QFont font10_(fontFamily_, 10);
  ui->textEdit_Results->setFont(font12_);
  ui->textBrowser_Info->setFont(font10_);
}
