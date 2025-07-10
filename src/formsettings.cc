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
#include "formsettings.h"
#include "ui_formsettings.h"

#include <QModelIndex>

#include "abstracttablemodel_settings_p.h"
#include "progid.h"
#include "types_global.h"

/*!
 * \brief FormSettings::FormSettings
 * \param parent
 */
FormSettings::FormSettings(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormSettings)
  , mainWindowToolBar(new QMainWindow)
  , toolBarSettings(new QToolBar)
{
  ui->setupUi(this);

  Messages_ = &Messages_->instance();

  ui->tabWidget_Settings->setCurrentIndex(0);
  ui->tabWidget_Settings->setTabText(0, "Directories and Files");
  ui->tabWidget_Settings->setTabText(1, "General");
  ui->tabWidget_Settings->setTabText(2, "Automatic Answerers");

  connect(ui->tabWidget_Settings,
          SIGNAL(tabBarClicked(int)),
          this,
          SLOT(tabWidget_Settings_tabBarClicked(int)));

  createToolBar();

  ShowHelp(Page::DirAndFiles);
  ShowHelp(Page::General);
  ShowHelp(Page::AutoAnswerers);
}

/*!
 * \brief FormSettings::~FormSettings
 */
FormSettings::~FormSettings()
{
  delete ui;
}

/*!
 * \public SLOT
 * \brief FormSettings::tabWidget_Settings_tabBarClicked
 * \param idx_
 */
void
FormSettings::tabWidget_Settings_tabBarClicked(int idx_)
{
  switch (idx_) {
    case static_cast<int>(Page::DirAndFiles): {
      toolButtonSave->setToolTip(tr("Save Directories And Files"));
      break;
    }
    case static_cast<int>(Page::General): {
      toolButtonSave->setToolTip(tr("Save General"));
      break;
    }
    case static_cast<int>(Page::AutoAnswerers): {
      toolButtonSave->setToolTip(tr("Save Auto 'Answerers'"));
      break;
    }
    default: {
      toolButtonSave->setToolTip("");
    }
  }
}

/*!
 * \private
 * \brief FormSettings::createToolBar
 */
void
FormSettings::createToolBar()
{
  QVBoxLayout* mainLayout_ = new QVBoxLayout(ui->frame_ToolBar);
  mainLayout_->addWidget(toolBarSettings);
  toolBarSettings->setFloatable(false);
  toolBarSettings->setMovable(false);
  toolButtonSave = new QToolButton;
  toolButtonSave->setIcon(QIcon::fromTheme("document-save"));
  toolButtonSave->setToolTip(tr("Save"));
  toolButtonRedo = new QToolButton;
  toolButtonRedo->setIcon(QIcon::fromTheme("edit-undo"));
  toolButtonRedo->setToolTip(tr("Reload"));

  toolBarSettings->addWidget(toolButtonSave);
  toolBarSettings->addWidget(toolButtonRedo);

  connect(toolButtonSave, &QToolButton::clicked, this, [this](bool clicked) {
    save(Globals::Modules::Settings,
         static_cast<Page>(ui->tabWidget_Settings->currentIndex()));
  });

  connect(toolButtonRedo, &QToolButton::clicked, this, [this](bool click_) {
    redo(Globals::Modules::Settings,
         static_cast<Page>(ui->tabWidget_Settings->currentIndex()));
  });
}

/*!
 * \private
 * \brief FormSettings::save
 * \param modules_
 * \param page_
 */
void
FormSettings::save(Globals::Modules modules_, Page page_)
{
  QMessageBox::StandardButton confirm_ =
    QMessageBox::warning(this,
                         tr(ProgId::Name),
                         tr("This operation will overwrite previously saved "
                            "data.\n\nDo you confirm?"),
                         QMessageBox::Yes | QMessageBox::No,
                         QMessageBox::No);
  if (confirm_ == QMessageBox::No) {
    return;
  }

  switch (modules_) {
    case Globals::Modules::Settings: {
      switch (page_) {
        case Page::DirAndFiles: {
          QList<QPair<QString, QString>> list_{};
          for (int i = 0; i < 7; ++i) {
            list_.append(qMakePair(
              std::move(
                ui->tableView_DirAndFiles->model()
                  ->data(ui->tableView_DirAndFiles->model()->index(i, 0))
                  .toString()),
              std::move(
                ui->tableView_DirAndFiles->model()
                  ->data(ui->tableView_DirAndFiles->model()->index(i, 1))
                  .toString())));
          }
          if (settingsHandler.saveDirAndFiles(list_)) {
            setSaved(Page::DirAndFiles);
            settingsHandler.readDirAndFiles();
          }
          break;
        }
        case Page::General: {
          QList<QPair<QString, QString>> list_{};
          for (int i = 0; i < settingsHandler.EnvVars_GBL_m_.size(); ++i) {
            list_.append(qMakePair(
              std::move(ui->tableView_General->model()
                          ->data(ui->tableView_General->model()->index(i, 0))
                          .toString()),
              std::move(ui->tableView_General->model()
                          ->data(ui->tableView_General->model()->index(i, 1))
                          .toString())));
          }
          if (settingsHandler.saveGeneral(list_)) {
            setSaved(Page::General);
            settingsHandler.readGeneral();
          }
          break;
        }
        case Page::AutoAnswerers: {
          QList<QPair<QString, QString>> list_{};
          for (int i = 0; i < settingsHandler.AutoAnswerers_m_.size(); ++i) {
            list_.append(qMakePair(
              std::move(
                ui->tableView_AutoAnswerers->model()
                  ->data(ui->tableView_AutoAnswerers->model()->index(i, 0))
                  .toString()),
              std::move(
                ui->tableView_AutoAnswerers->model()
                  ->data(ui->tableView_AutoAnswerers->model()->index(i, 1))
                  .toString())));
          }
          if (settingsHandler.saveAutoAnswerers(list_)) {
            setSaved(Page::AutoAnswerers);
            settingsHandler.readAutoAnswerers();
          }
          break;
        }
        case Page::None: {
          break;
        }
      }
      break;
    }
    case Globals::Modules::ScriptGen: {
      break;
    }
    case Globals::Modules::ExternalTools: {
      break;
    }
    case Globals::Modules::None: {
    }
  }
}

/*!
 * \private
 * \brief FormSettings::redo
 * \param modules_
 * \param page_
 * \todo At some point I'll refactor the code for this function to contain
 * less code repetition.
 */
void
FormSettings::redo(Globals::Modules modules_, Page page_)
{
  EnvironmentVars env_;
  switch (modules_) {
    case Globals::Modules::Settings: {
      switch (page_) {
        case Page::DirAndFiles: {
          AppTypes::NamedValueList values_ = settingsHandler.readDirAndFiles();
          const int rowCount_ =
            std::min(settingsHandler.EnvVars_DF_m_.size(), values_.size());
          AbstractTableModel_Settings_P* obj_ =
            new AbstractTableModel_Settings_P(
              rowCount_, 2, ui->tableView_DirAndFiles);
          Q_ASSERT(obj_);
          for (int row_ = 0; row_ < rowCount_; ++row_) {
            const QString& varName_ = values_.at(row_).first;
            const QString& varData_ = values_.at(row_).second;
            QModelIndex index_varname_ = obj_->index(row_, 0);
            obj_->setData(index_varname_, varName_);
            QModelIndex index_data_ = obj_->index(row_, 1);
            obj_->setData(index_data_, varData_);
          }
          ui->tableView_DirAndFiles->setModel(obj_);
          break;
        }
        case Page::General: {
          AppTypes::NamedValueList values_ = settingsHandler.readGeneral();
          const int rowCount_ =
            std::min(settingsHandler.EnvVars_GBL_m_.size(), values_.size());
          AbstractTableModel_Settings_P* obj_ =
            new AbstractTableModel_Settings_P(
              rowCount_, 2, ui->tableView_General);
          Q_ASSERT(obj_);
          for (int row_ = 0; row_ < rowCount_; ++row_) {
            const QString& varName_ = values_.at(row_).first;
            const QString& varData_ = values_.at(row_).second;
            QModelIndex index_varname_ = obj_->index(row_, 0);
            obj_->setData(index_varname_, varName_);
            QModelIndex index_data_ = obj_->index(row_, 1);
            obj_->setData(index_data_, varData_);
          }
          ui->tableView_General->setModel(obj_);
          break;
        }
        case Page::AutoAnswerers: {
          AppTypes::NamedValueList values_ =
            settingsHandler.readAutoAnswerers();
          const int rowCount_ =
            std::min(settingsHandler.AutoAnswerers_m_.size(), values_.size());
          AbstractTableModel_Settings_P* obj_ =
            new AbstractTableModel_Settings_P(
              rowCount_, 2, ui->tableView_AutoAnswerers);
          Q_ASSERT(obj_);
          for (int row_ = 0; row_ < rowCount_; ++row_) {
            const QString& varName_ = values_.at(row_).first;
            const QString& varData_ = values_.at(row_).second;
            QModelIndex index_varname_ = obj_->index(row_, 0);
            obj_->setData(index_varname_, varName_);
            QModelIndex index_data_ = obj_->index(row_, 1);
            obj_->setData(index_data_, varData_);
          }
          ui->tableView_AutoAnswerers->setModel(obj_);
          break;
        }
        case Page::None:
          break;
      }
      case Globals::Modules::ScriptGen: {
        break;
      }
      case Globals::Modules::ExternalTools:
        break;
      case Globals::Modules::None: {
      }
      case Globals::Modules::FormLists:
      case Globals::Modules::FormUtils:
        break;
    }
  }
}

/*!
 * \brief FormSettings::ShowHelp
 * \param page_
 */
void
FormSettings::ShowHelp(Page page_)
{
  switch (page_) {
    case Page::DirAndFiles: {
      int start_ = static_cast<int>(MessagesNS::Tokens::HELP_000);
      for (auto it_ = settingsHandler.EnvVars_DF_m_.cbegin();
           it_ != settingsHandler.EnvVars_DF_m_.cend();
           ++it_) {
        ui->textBrowser_Help->append(
          QString("<strong>%0</strong><br<br>%1<br>")
            .arg(
              *it_,
              Messages_->set(static_cast<MessagesNS::Tokens>(start_)).text()));
        ++start_;
      }
      break;
    }
    case Page::General: {
      int start_ = static_cast<int>(MessagesNS::Tokens::HELP_GEN_000);
      for (auto it_ = settingsHandler.EnvVars_GBL_m_.cbegin();
           it_ != settingsHandler.EnvVars_GBL_m_.cend();
           ++it_) {
        ui->textBrowser_Help_General->append(
          QString("<strong>%0</strong><br<br>%1<br>")
            .arg(
              *it_,
              Messages_->set(static_cast<MessagesNS::Tokens>(start_)).text()));
        ++start_;
      }
      break;
    }
    case Page::AutoAnswerers: {
      int start_ = static_cast<int>(MessagesNS::Tokens::HELP_GEN_015);
      for (auto it_ = settingsHandler.AutoAnswerers_m_.cbegin();
           it_ != settingsHandler.AutoAnswerers_m_.cend();
           ++it_) {
        ui->textBrowser_Help_AutoAnswerers->append(
          QString("<strong>%0</strong><br<br>%1<br>")
            .arg(
              *it_,
              Messages_->set(static_cast<MessagesNS::Tokens>(start_)).text()));
        ++start_;
      }
      break;
    }
    case Page::None: {
    }
  }
}
