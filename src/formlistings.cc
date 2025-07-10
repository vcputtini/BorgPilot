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

#include "formlistings.h"
#include "src/ui_formlistings.h"

#include "QRadioButton"
#include "QSettings"
#include <QFileDialog>
#include <QFontDatabase>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QRadioButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringLiteral>
#include <QTreeWidgetItem>

#include "borgrunnerthread.h"
#include "progid.h"

/*!
 * \brief FormListings::FormListings
 * \param parent
 */
FormListings::FormListings(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormListings)
{
  ui->setupUi(this);

  ui->toolBox_SearchAndListing->setCurrentIndex(Pages::Search);
  connect(ui->lineEdit_ScriptName,
          &QLineEdit::textChanged,
          this,
          [this](const QString&) {
            ui->toolBox_SearchAndListing->setCurrentIndex(Pages::Search);
          });

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
 * \private slot
 * \brief FormListings::setPattern
 * \param pattern
 */
void
FormListings::setPattern(const QString& pattern)
{
  ui->lineEdit_Pattern->setText(pattern);
}

/*!
 * \private slot
 * \brief FormListings::listRepoNames
 */
void
FormListings::listRepoNames()
{
  if (ui->lineEdit_ScriptName->text().simplified().isEmpty()) {
    QMessageBox::warning(this, tr("Warning"), tr("Script name not provided!"));
    ui->lineEdit_ScriptName->setFocus();
    return;
  }

  ui->treeWidget_Repos->clearTree();
  ui->treeWidget_Repos->setColumnWidth(0, 500);
  ui->treeWidget_Repos->setColumnWidth(1, 200);
  QStringList repoNames_ = getScriptNames();

  ui->treeWidget_Repos->addRootItem(
    ui->lineEdit_ScriptName->text().simplified());
  for (const auto& rItem_ : std::as_const(repoNames_)) {
    ui->treeWidget_Repos->addRepoItem(std::move(rItem_.section("::", 0, 0)),
                                      std::move(rItem_.section("::", 1)));
  }

  ui->treeWidget_Repos->expandAll();
}

/*!
 * \privae slot
 * \brief FormListings::execSearch
 */
void
FormListings::execSearch()
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
    ui->lineEdit_ScriptName->setFocus();
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
 * \brief FormListings::getScriptNames
 * \return
 */
QStringList
FormListings::getScriptNames()
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  const QString completeGroupName_ =
    QString("INITREPO_%0").arg(std::move(ui->lineEdit_ScriptName->text()));

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
 * \private
 * \brief FormListings::setupToolButtons
 */
void
FormListings::setupToolButtons()
{
  connect(
    ui->toolButton_Exec, SIGNAL(clicked(bool)), this, SLOT(listRepoNames()));
  connect(
    ui->toolButton_ExecSearch, SIGNAL(clicked(bool)), this, SLOT(execSearch()));
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
 * \brief FormListings::setupTreeWidgetRepos
 */
void
FormListings::setupTreeWidgetRepos()
{
  connect(ui->treeWidget_Repos,
          SIGNAL(setPattern(const QString&)),
          this,
          SLOT(setPattern(const QString&)));
}

/*!
 * \private
 * \brief FormListings::setupTextEdit
 * \note Loads and sets the embedded font
 */
void
FormListings::setupTextEditFont()
{
  fontId_ = QFontDatabase::addApplicationFont(":/Fonts/COUR.TTF");
  fontFamily_ = QFontDatabase::applicationFontFamilies(fontId_).at(0);
  QFont font_(fontFamily_, 12);
  ui->textEdit_Results->setFont(font_);
}
