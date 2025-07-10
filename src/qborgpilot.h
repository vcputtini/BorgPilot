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
#ifndef QBORGPILOT_H
#define QBORGPILOT_H

#include <QAction>
#include <QDebug>
#include <QDialog>
#include <QIcon>
#include <QMainWindow>
#include <QMap>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMessageBox>
#include <QShowEvent>
#include <QString>
#include <QStyle>
#include <QToolBar>
#include <QToolButton>

#include "dialogabout.h"
#include "dialogfindreqexternaltools.h"
#include "formlistings.h"
#include "formscriptgenerator.h"
#include "formsettings.h"
#include "formutilities.h"

#include "global_tools.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class qBorgPilot;
}
QT_END_NAMESPACE

/*!
 * \brief The qBorgPilot class
 */
class qBorgPilot : public QMainWindow
{
  Q_OBJECT

public:
  explicit qBorgPilot(QWidget* parent = nullptr);
  ~qBorgPilot();

private:
  Ui::qBorgPilot* ui;

private:
  FormSettings* formSettings_;
  FormScriptGenerator* formScriptGenerator_;
  DialogAbout* dialogAbout;
  DialogFindReqExternalTools* dialogExternTools;
  FormListings* formListings_;
  FormUtilities* formUtilities_;

  QAction* action_Modules;
  QAction* action_Tools;
  QAction* action_About;
  QAction* action_AboutQt;
  QAction* action_Exit;

  QAction* action_FormSettings;
  QAction* action_ExternalTools;

  void CreateActions();
  void CreateMenus();

  void OpenMDIWindows(QWidget* parent_ = nullptr,
                      Globals::Modules modules_ = Globals::Modules::None);

private slots:
  void action_About_triggered();
  void action_AboutQt_triggered();

  void action_Exit_triggered();

  void dialogFindExternalTools_triggered();
  void formSettings_triggered();

  void showStatusMessage(const QString& message_);

  virtual void modules_listView_doubleClicked(QModelIndex);

protected:
  bool isActiveWindows(const QString&& title_ = QString());
  void closeEvent(QCloseEvent* event);
  void showEvent(QShowEvent* e);
};
#endif // QBORGPILOT_H
