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
#include "dialogshowlogfile.h"
#include "ui_dialogshowlogfile.h"

#include <QColor>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QTextStream>

#include "global_tools.h"
#include "settingshandler.h"

DialogShowLogFile::DialogShowLogFile(DialogShowFilesModes::Modes mode,
                                     QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::DialogShowLogFile)
  , m_fileModes_{ mode }
{
  ui->setupUi(this);
  setup();
  m_highlighter_ = new BashHighlighterLite(ui->plainTextEdit_Show->document(),
                                           BashHighlighterLite::Theme::Dark);
}

DialogShowLogFile::~DialogShowLogFile()
{
  delete ui;
}

void
DialogShowLogFile::setup()
{
  connect(
    ui->toolButton_Open, SIGNAL(clicked(bool)), this, SLOT(openLogFile()));

  const int fontId_ = QFontDatabase::addApplicationFont(R"(:/Fonts/COUR.TTF)");
  const QString fontFamily_ =
    QFontDatabase::applicationFontFamilies(fontId_).at(0);
  const QFont font12_(fontFamily_, 12);
  ui->plainTextEdit_Show->setFont(font12_);
}

void
DialogShowLogFile::openLogFile()
{
  QDir dir_;
  QFileDialog dlg_;
  dlg_.setOptions(QFileDialog::ReadOnly);
  dlg_.setFileMode(QFileDialog::ExistingFiles);
  dlg_.setViewMode(QFileDialog::Detail);

  SettingsHandler settings_;
  SettingsHandler::Preferences prefs_ = settings_.relodPreferences();

  QString fn_{};

  switch (m_fileModes_) {
    case DialogShowFilesModes::Modes::Indefined: {
      break;
    }
    case DialogShowFilesModes::Modes::CreateScript: {
      dlg_.setNameFilter(R"(create_*.sh)");
      dir_.setPath(prefs_.localScriptPath);
      fn_ = dlg_.getOpenFileName(
        this, tr("Open"), prefs_.localScriptPath, tr("Files (create_*.sh)"));
      Globals gTools_;
      if (!gTools_.isBashScript(fn_)) {
        [[maybe_unused]] const int ret_ =
          QMessageBox::warning(this,
                               ProgId::Name,
                               tr("This file is not a bash script."),
                               QMessageBox::Cancel);
        return;
      }
      break;
    }
    case DialogShowFilesModes::Modes::InitScript: {
      dlg_.setNameFilter(R"(init_*.sh)");
      dir_.setPath(prefs_.localScriptPath);
      fn_ = dlg_.getOpenFileName(
        this, tr("Open"), prefs_.localScriptPath, tr("Files (init_*.sh)"));
      Globals gTools_;
      if (!gTools_.isBashScript(fn_)) {
        [[maybe_unused]] const int ret_ =
          QMessageBox::warning(this,
                               ProgId::Name,
                               tr("This file is not a bash script."),
                               QMessageBox::Cancel);
        return;
      }
      break;
    }
    case DialogShowFilesModes::Modes::Log: {
      dlg_.setNameFilter(R"(*.log)");
      dlg_.setDirectory(prefs_.localLogPath);
      fn_ = dlg_.getOpenFileName(
        this, tr("Open"), prefs_.localLogPath, tr("Files (*.log)"));
      break;
    }
    case DialogShowFilesModes::Modes::SaveResults: {
      dlg_.setNameFilter(R"(*.dat)");
      dlg_.setDirectory(prefs_.localDataPath);
      fn_ = dlg_.getOpenFileName(
        this, tr("Choose"), prefs_.localDataPath, tr("Files (*.dat)"));
      break;
    }
  }
  QFile file_(fn_);
  if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
    ui->plainTextEdit_Show->setPlainText(
      QString("Could not open file: %1").arg(file_.errorString()));
    return;
  }

  QTextStream in_(&file_);
  in_.setAutoDetectUnicode(true);

  QString content_;
  while (!in_.atEnd()) {
    QString line_ = std::move(in_.readLine());
    content_ += line_ + '\n';
  }

  ui->plainTextEdit_Show->setPlainText(content_);
  file_.close();
}
