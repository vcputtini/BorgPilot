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
#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include "global_tools.h" // Progid::name
#include "settingshandler.h"

DialogPreferences::DialogPreferences(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::DialogPreferences)
  , m_mainWindowToolBar(new QMainWindow)
  , m_toolBarPrefs(new QToolBar)
  , m_mode_(DialogShowFilesModes::Modes::Indefined)
{
  ui->setupUi(this);
  connect(&m_settingsHandler,
          SIGNAL(sig_status(QString)),
          this,
          SLOT(sl_settingsHandlerStatus(QString)));

  createToolBar();

  sl_reload();
}

DialogPreferences::~DialogPreferences()
{
  delete ui;
}

void
DialogPreferences::createToolBar()
{
  QVBoxLayout* mainLayout_ = new QVBoxLayout(ui->frame_ToolBar);
  mainLayout_->addWidget(m_toolBarPrefs);
  m_toolBarPrefs->setFloatable(false);
  m_toolBarPrefs->setMovable(false);
  m_toolButtonSave = new QToolButton;
  m_toolButtonSave->setIcon(QIcon::fromTheme("document-save"));
  m_toolButtonSave->setToolTip(tr("Save"));
  m_toolButtonRedo = new QToolButton;
  m_toolButtonRedo->setIcon(QIcon::fromTheme("edit-undo"));
  m_toolButtonRedo->setToolTip(tr("Reload"));

  m_toolBarPrefs->addWidget(m_toolButtonSave);
  m_toolBarPrefs->addWidget(m_toolButtonRedo);

  connect(&m_settingsHandler,
          SIGNAL(status(const QString&)),
          this,
          SLOT(sl_settingsHandlerStatus(QString)));
  connect(m_toolButtonSave, SIGNAL(clicked(bool)), this, SLOT(sl_save()));
  connect(m_toolButtonRedo, SIGNAL(clicked(bool)), this, SLOT(sl_reload()));

  connect(ui->toolButton_LocalScriptPath,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_localScriptPath(bool)));
  connect(ui->toolButton_LocalLogPath,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_localLogPath(bool)));
  connect(ui->toolButton_LocalDataPath,
          SIGNAL(clicked(bool)),
          this,
          SLOT(sl_localDataPath(bool)));
}

/*!
 * \private slot
 * \brief DialogPreferences::settingsHandlerStatus
 * \param status
 */
void
DialogPreferences::sl_settingsHandlerStatus(const QString& status)
{
  QMessageBox::warning(this, tr("Preferences"), status);
}

/*!
 * \private slot
 * \brief DialogPreferences::save
 */
void
DialogPreferences::sl_save()
{
  auto message_ = [this](const QString& msg) -> bool {
    QMessageBox::StandardButton confirm_ =
      QMessageBox::warning(this,
                           ProgId::Name,
                           msg.toStdString().c_str(),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);
    if (confirm_ == QMessageBox::No) {
      return false;
    }
    return true;
  };

  if (!message_(tr("This operation will overwrite previously saved "
                   "data.\n\nDo you confirm?"))) {
    return;
  }

  SettingsHandler::Preferences prefs_;
  prefs_.localScriptPath = ui->lineEdit_LocalScriptPath->text().trimmed();
  prefs_.localLogPath = ui->lineEdit_LocalLogPath->text().trimmed();
  prefs_.localDataPath = ui->lineEdit_LocalDataPath->text().trimmed();
  if (!m_settingsHandler.savePreferences(prefs_)) {
    message_(tr("Error trying to save preferences."));
    return;
  }
}

/*!
 * \private slot
 * \brief DialogPreferences::reload
 */
void
DialogPreferences::sl_reload()
{
  SettingsHandler::Preferences prefs_ = m_settingsHandler.relodPreferences();
  ui->lineEdit_LocalScriptPath->setText(prefs_.localScriptPath.toUtf8());
  ui->lineEdit_LocalLogPath->setText(prefs_.localLogPath.toUtf8());
  ui->lineEdit_LocalDataPath->setText(prefs_.localDataPath.toUtf8());
}

void
DialogPreferences::sl_localScriptPath(bool clicked)
{
  m_mode_ = std::move(DialogShowFilesModes::Modes::InitScript);
  ui->lineEdit_LocalScriptPath->setText(choosePath());
}

void
DialogPreferences::sl_localLogPath(bool clicked)
{
  m_mode_ = std::move(DialogShowFilesModes::Modes::Log);
  ui->lineEdit_LocalLogPath->setText(choosePath());
}

void
DialogPreferences::sl_localDataPath(bool clicked)
{
  m_mode_ = std::move(DialogShowFilesModes::Modes::SaveResults);
  ui->lineEdit_LocalDataPath->setText(choosePath());
}

const QString
DialogPreferences::choosePath() noexcept
{
  QDir dir_;
  QFileDialog dlg_;
  dlg_.setOptions(QFileDialog::ReadOnly);
  dlg_.setFileMode(QFileDialog::Directory);

  SettingsHandler settings_;
  SettingsHandler::Preferences prefs_ = settings_.relodPreferences();

  QString fn_{};

  switch (m_mode_) {
    case DialogShowFilesModes::Modes::CreateScript:
    case DialogShowFilesModes::Modes::Indefined: {
      break;
    }
    case DialogShowFilesModes::Modes::Log: {
      fn_ =
        dlg_.getExistingDirectory(this, tr("Directory"), prefs_.localLogPath);
      break;
    }
    case DialogShowFilesModes::Modes::InitScript: {
      fn_ = dlg_.getExistingDirectory(
        this, tr("Directory"), prefs_.localScriptPath);
      break;
    }
    case DialogShowFilesModes::Modes::SaveResults:
      fn_ =
        dlg_.getExistingDirectory(this, tr("Directory"), prefs_.localDataPath);
      break;
  }

  return fn_.trimmed();
}
