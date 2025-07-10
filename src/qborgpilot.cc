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
#include "qborgpilot.h"
#include "./ui_qborgpilot.h"

#include "createdefaultborgconffile.h"
#include "progid.h"

/*!
 * \brief qBorgPilot::qBorgPilot
 * \param parent
 */
qBorgPilot::qBorgPilot(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::qBorgPilot)
{
  ui->setupUi(this);

  CreateActions();
  CreateMenus();

  CreateDefaultBorgConfFile conf;
  conf.create();
}

/*!
 * \brief qBorgPilot::~qBorgPilot
 */
qBorgPilot::~qBorgPilot()
{
  delete ui;
}

/*!
 * \private
 * \brief qBorgPilot::CreateMenus
 */
void
qBorgPilot::CreateMenus()
{
  ui->menu_Main_Actions->addAction(action_Modules);
  ui->menu_Main_Actions->addSeparator();
  ui->menu_Main_Actions->addAction(action_Exit);

  ui->menu_Main_Help->addAction(action_About);
  ui->menu_Main_Help->addAction(action_AboutQt);

  ui->menu_Tools->addAction(action_ExternalTools);
  ui->menu_Tools->addAction(action_FormSettings);
}

/*!
 * \private
 * \brief qBorgPilot::CreateActions
 */
void
qBorgPilot::CreateActions()
{
  action_Modules = new QAction(this);
  action_Modules = ui->modules_dockWidget->toggleViewAction();
  action_Modules->setText(tr("&Modules"));
  action_Modules->setShortcut(QString(tr("Ctrl+M")));
  action_Modules->setStatusTip(tr("System Module Access"));

  action_Exit = new QAction(this);
  action_Exit->setText(tr("Exit"));
  action_Exit->setShortcut(QString(tr("Ctrl+Q")));
  // action_Exit->setStatusTip(tr(""));
  connect(
    action_Exit, SIGNAL(triggered()), this, SLOT(action_Exit_triggered()));

  action_ExternalTools = new QAction(this);
  action_ExternalTools->setText(tr("Find &External Tools"));
  action_ExternalTools->setShortcut(QString(tr("Ctrl+E")));
  action_ExternalTools->setStatusTip(tr("Find External Tools"));
  connect(action_ExternalTools,
          SIGNAL(triggered()),
          this,
          SLOT(dialogFindExternalTools_triggered()));

  action_FormSettings = new QAction(this);
  action_FormSettings->setText(tr("BorgBackup Se&ttings"));
  action_FormSettings->setShortcut(QString(tr("Ctrl+T")));
  action_FormSettings->setStatusTip(tr("BorgBackup Exclusive Settings"));
  connect(action_FormSettings,
          SIGNAL(triggered()),
          this,
          SLOT(formSettings_triggered()));

  action_About = new QAction(this);
  action_About->setText(tr("About ..."));
  action_About->setStatusTip(tr("About %0").arg(ProgId::Name));
  connect(
    action_About, SIGNAL(triggered()), this, SLOT(action_About_triggered()));

  action_AboutQt = new QAction(this);
  action_AboutQt->setText(tr("About Qt"));
  action_AboutQt->setStatusTip(tr("Qt General Informations"));
  connect(action_AboutQt,
          SIGNAL(triggered()),
          this,
          SLOT(action_AboutQt_triggered()));
}

/*!
 * \private
 * \brief qBorgPilot::OpenMDIWindows
 * \param parent_
 * \param modules_
 */
void
qBorgPilot::OpenMDIWindows(QWidget* parent_, Globals::Modules modules_)
{
  auto openSubWindow = [this](QWidget* widget) {
    QMdiSubWindow* subWin = new QMdiSubWindow;
    subWin->setWidget(widget);
    subWin->setAttribute(Qt::WA_DeleteOnClose);
    ui->main_mdiArea->addSubWindow(subWin);
    subWin->showMaximized();
  };

  switch (modules_) {
    case Globals::Modules::ScriptGen: {
      openSubWindow(parent_);
      break;
    }
    case Globals::Modules::FormLists: {
      openSubWindow(parent_);
      break;
    }
    case Globals::Modules::FormUtils: {
      openSubWindow(parent_);
      break;
    }
    case Globals::Modules::Settings: {
      openSubWindow(parent_);
      break;
    }
    case Globals::Modules::ExternalTools:
    case Globals::Modules::None: {
    }
  }
}

/*!
 * \brief qBorgPilot::dialogFindExternalTools_triggered
 */
void
qBorgPilot::dialogFindExternalTools_triggered()
{
  dialogExternTools = new DialogFindReqExternalTools;
  dialogExternTools->setWindowFlags(Qt::Dialog);
  dialogExternTools->setModal(true);
  dialogExternTools->show();
}

/*!
 * \public SLOT
 * \brief qBorgPilot::formSettings_triggered
 */
void
qBorgPilot::formSettings_triggered()
{
  formSettings_ = new FormSettings;
  OpenMDIWindows(formSettings_, Globals::Modules::Settings);
}

/*!
 * \public SLOT
 * \brief qDocBase::action_About_triggered
 */
void
qBorgPilot::action_About_triggered()
{
  dialogAbout = new DialogAbout;
  dialogAbout->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
  dialogAbout->show();
}

/*!
 * \public SLOT
 * \brief qBorgPilot::action_AboutQt_triggered
 */
void
qBorgPilot::action_AboutQt_triggered()
{
  QMessageBox::aboutQt(this, "About Qt");
}

/*!
 * \public SLOT
 * \brief qBorgPilot::action_Exit_triggered
 */
void
qBorgPilot::action_Exit_triggered()
{
  close();
}

/*!
 * \public SLOT
 * \brief qBorgPilot::modules_listView_doubleClicked
 * \param idx
 */
void
qBorgPilot::modules_listView_doubleClicked(QModelIndex idx)
{
  Globals::Modules modules_ =
    static_cast<Globals::Modules>(idx.data(Qt::UserRole).toInt());

  ui->modules_dockWidget->setHidden(true);

  switch (modules_) {
    case Globals::Modules::ScriptGen: {
      formScriptGenerator_ = new FormScriptGenerator;
      OpenMDIWindows(formScriptGenerator_, Globals::Modules::ScriptGen);
      break;
    }
    case Globals::Modules::FormLists: {
      formListings_ = new FormListings;
      OpenMDIWindows(formListings_, Globals::Modules::FormLists);
      break;
    }
    case Globals::Modules::FormUtils: {
      formUtilities_ = new FormUtilities;
      OpenMDIWindows(formUtilities_, Globals::Modules::FormUtils);
      break;
    }
    case Globals::Modules::ExternalTools:
    case Globals::Modules::Settings:
    case Globals::Modules::None:
      break;
  }
}

void
qBorgPilot::showStatusMessage(const QString& message_)
{
  statusBar()->showMessage(message_);
}

/*!
 * \protected
 * \brief Handles Mainwindow closing events. Takes all the buttons that close
 * the window.
 * \param event
 */
void
qBorgPilot::closeEvent(QCloseEvent* event)
{
  if (!isActiveWindows()) {
    event->accept();
    close();
  } else
    event->ignore();
}

/*!
 * \protected
 * \brief qBorgPilot::showEvent
 * \param e
 */
void
qBorgPilot::showEvent(QShowEvent* e)
{
  e->accept();
}

/*!
 * \protected
 * \brief Checks for open windows. Returns true if yes and false if not
 * \return true|false
 */
bool
qBorgPilot::isActiveWindows(const QString&& title_)
{
  if (title_.contains(title_)) {
    if (ui->main_mdiArea->activeSubWindow()) { // ha janelas abertas
      QMessageBox::warning(this,
                           tr(ProgId::Name),
                           tr("For the security of your information,"
                              "\nit is not allowed to close the program"
                              "\nunless all windows are closed."),
                           QMessageBox::Close);
      return true;
    }
  }
  return false;
}
