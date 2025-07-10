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
#ifndef FORMSETTINGS_H
#define FORMSETTINGS_H

#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QWidget>

#include "global_tools.h"

#include "settingshandler.h"
#include "tableview_autoanswerers_p.h"
#include "tableview_dirandfiles_p.h"
#include "tableview_general_p.h"

#include "messages.h"

namespace Ui {
class FormSettings;
}

/*!
 * \brief The FormSettings class
 */
class FormSettings : public QWidget
{
  Q_OBJECT

public:
  explicit FormSettings(QWidget* parent = nullptr);
  ~FormSettings();

private:
  Ui::FormSettings* ui;

private:
  SettingsHandler settingsHandler;

  QMainWindow* mainWindowToolBar;
  QToolBar* toolBarSettings;
  QToolButton* toolButtonSave;
  QToolButton* toolButtonRedo;

  TableView_DirAndFiles_P* tableView_DirAndFiles_P;
  TableView_General_P* tableView_General_P;
  TableView_AutoAnswerers_P* tableView_AutoAnswerers_P;

  MessagesNS::Messages* Messages_;

  enum class Page
  {
    DirAndFiles = 0x00,
    General,
    AutoAnswerers,
    None
  };

  struct Saved_t_
  {
    unsigned int p0 : 1;
    unsigned int p1 : 1;
    unsigned int p2 : 1;
  } saved_t_{};

  bool isSaved(Page page_)
  {
    switch (page_) {
      case Page::DirAndFiles: {
        return (saved_t_.p0 & (1 << 0));
      }
      case Page::General: {
        return (saved_t_.p1 & (1 << 0));
      }
      case Page::AutoAnswerers: {
        return (saved_t_.p2 & (1 << 0));
      }
      case Page::None: {
      }
    }
    return false;
  }

  void setSaved(Page page_)
  {
    switch (page_) {
      case Page::DirAndFiles: {
        saved_t_.p0 ^= (1 << 0);
      }
      case Page::General: {
        saved_t_.p1 ^= (1 << 0);
      }
      case Page::AutoAnswerers: {
        saved_t_.p2 ^= (1 << 0);
      }
      case Page::None: {
      }
    }
  }

  void createToolBar();

  void save(Globals::Modules modules_ = Globals::Modules::None,
            Page page_ = Page::None);
  void redo(Globals::Modules modules_ = Globals::Modules::None,
            Page page_ = Page::None);

  void ShowHelp(Page page_);

public slots:
  void tabWidget_Settings_tabBarClicked(int idx_);
};

#endif // FORMSETTINGS_H
