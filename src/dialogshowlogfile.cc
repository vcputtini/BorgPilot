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
#include "dialogshowlogfile.h"
#include "ui_dialogshowlogfile.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "progid.h"
// #include "settingshandler.h"

DialogShowLogFile::DialogShowLogFile(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::DialogShowLogFile)
{
  ui->setupUi(this);

  connect(
    ui->toolButton_Open, SIGNAL(clicked(bool)), this, SLOT(openLogFile()));
}

DialogShowLogFile::~DialogShowLogFile()
{
  delete ui;
}

void
DialogShowLogFile::openLogFile()
{
  QDir dir_(R"(/var/log/borgpilot)");

  QFileDialog dlg_;
  dlg_.setOptions(QFileDialog::ReadOnly);
  dlg_.setFileMode(QFileDialog::ExistingFiles);
  dlg_.setViewMode(QFileDialog::Detail);
  dlg_.setNameFilter(R"(*.log)");
  dlg_.setDirectory(dir_.path());

  QString fn_ =
    dlg_.getOpenFileName(this, tr("Open"), dir_.path(), tr("Files (*.log)"));
  if (fn_.isEmpty()) {
    [[maybe_unused]] const int ret_ =
      QMessageBox::warning(this,
                           ProgId::Name,
                           tr("File name was not provided!"),
                           QMessageBox::Cancel);
    dlg_.close();
    return;
  }
  QFile file_(fn_);
  if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
    ui->textBrowser_Show->setPlainText(
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
  ui->textBrowser_Show->setPlainText(std::move(content_));
  file_.close();
}
