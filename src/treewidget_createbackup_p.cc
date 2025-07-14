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
#include "treewidget_createbackup_p.h"

#include <QLineEdit>
#include <QStandardItemModel>

#include "command2.h"
#include "global_options.h"
#include "progid.h"

#include <algorithm> // std::find_if()

/*!
 * \brief treeWidget_CreateBackup_P::treeWidget_CreateBackup_P
 * \param parent
 */
treeWidget_CreateBackup_P::treeWidget_CreateBackup_P(QWidget* parent)
  : QTreeWidget{ parent }
{
}

/*!
 * \brief treeWidget_CreateBackup_P::setScriptName
 * \param name_
 */
void
treeWidget_CreateBackup_P::setScriptName(const QString&& name_) noexcept
{
  scriptName_ = name_;
}

/*!
 * \brief treeWidget_CreateBackup_P::getScriptNames
 * \return
 */
QStringList
treeWidget_CreateBackup_P::getScriptNames()
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  const QString completeGroupName_ = QString("INITREPO_%0").arg(scriptName_);

  QStringList repoNames_;
  if (settings_.childGroups().contains(completeGroupName_)) {
    settings_.beginGroup(completeGroupName_);
    destRepo_ = settings_.value("A_DESTREPO").toString();
    for (int i = 0; i < 20; ++i) {
      const QString rName_ =
        settings_.value(QString("B_REPONAME_%1").arg(i)).toString();
      const QString rArchive_ =
        settings_.value(QString("B_ARCHIVE_%1").arg(i)).toString();

      if (rArchive_.isEmpty()) {
        continue;
      }
      const QString repoName_ = rName_ + "::" + rArchive_;

      if (!repoName_.isEmpty()) {
        repoNames_.append(destRepo_ + "/" + repoName_);
      }
    }
    settings_.endGroup();
  }
  return repoNames_;
}

/*!
 * \brief treeWidget_CreateBackup_P::getCheckedItems
 * \return
 */
AppTypes::RepoistoryCreateDataMap
treeWidget_CreateBackup_P::getCheckedItems()
{
  AppTypes::RepoistoryCreateDataMap data_m_{};

  for (int iTopLevelItem_ = 0; iTopLevelItem_ < this->topLevelItemCount();
       ++iTopLevelItem_) {
    QTreeWidgetItem* rootItem_ = this->topLevelItem(iTopLevelItem_);
    if (!rootItem_) {
      continue;
    }

    QList<QString> repoList_{};

    for (int iChildItem_ = 0; iChildItem_ < rootItem_->childCount();
         ++iChildItem_) { // repo name
      QTreeWidgetItem* childItem_ = rootItem_->child(iChildItem_);
      QCheckBox* repoCheckBox_ =
        qobject_cast<QCheckBox*>(this->itemWidget(childItem_, 0));
      QString repoName_{};
      if (repoCheckBox_ && repoCheckBox_->isChecked()) {

        QLineEdit* archive_ =
          qobject_cast<QLineEdit*>(this->itemWidget(childItem_, 1));
        repoName_ = repoCheckBox_->text() + "#" + archive_->text().simplified();

        // qDebug() << repoCheckBox_->text().simplified() + "#" +
        //               archive_->text().simplified();

        repoList_.append(repoName_);

        QList<QString> commonOptsList_{};
        QList<QString> argumentsList_{};

        // common options
        for (int iChildItemCm_ = 0; iChildItemCm_ < childItem_->childCount();
             ++iChildItemCm_) {
          QTreeWidgetItem* childItem_cm_ = childItem_->child(iChildItemCm_);
          QCheckBox* checkBox_ =
            qobject_cast<QCheckBox*>(this->itemWidget(childItem_cm_, 2));
          if (checkBox_ && checkBox_->isChecked()) {
            commonOptsList_.append(checkBox_->text());
          }
        }
        // optional arguments
        for (int iChildItemArg_ = 0; iChildItemArg_ < childItem_->childCount();
             ++iChildItemArg_) {
          QTreeWidgetItem* childItem_args_ = childItem_->child(iChildItemArg_);
          QCheckBox* checkBoxArgs_ =
            qobject_cast<QCheckBox*>(this->itemWidget(childItem_args_, 3));
          if (checkBoxArgs_ && checkBoxArgs_->isChecked()) {
            QLineEdit* flags_ =
              qobject_cast<QLineEdit*>(this->itemWidget(childItem_args_, 4));
            if (flags_ && !flags_->text().isEmpty()) {
              argumentsList_.append(checkBoxArgs_->text() + "#" +
                                    flags_->text().simplified());
            } else {
              argumentsList_.append(checkBoxArgs_->text());
            }
          }
        }
        // add to map
        data_m_.insert(repoName_, qMakePair(commonOptsList_, argumentsList_));
      }
    } // end for iChildItem_
  } // end for

  return data_m_;
}

// ==========================================================================

/*!
 * \public
 * \brief treeWidget_CreateBackup_P::reloadCreateBackupSettings
 */
void
treeWidget_CreateBackup_P::reloadCreateBackupSettings(
  const AppTypes::RepoistoryCreateDataMap& dm_)
{

  // -----------------------------------------------------------------------
  // Set properties for OptionalsLineEdit
  auto setProperties = [this](const auto& keyword_,
                              const QString& value_ = QString()) -> void {
    if (GlobalOptions glb_; glb_.haveArgs(keyword_)) {
      OptionalsLineEdit* edit_ = new OptionalsLineEdit(keyword_, this);
      edit_->setText(value_);
      setItemWidget(levelItem2_, 4, edit_);
    }
  };

  // QPair<bool, value>
  auto splitParameter = [](const QString keyword_, QList<QString>& list_) {
    auto it =
      std::find_if(list_.cbegin(), list_.cend(), [&keyword_](const QString& a) {
        return a.contains(keyword_);
      });
    return (it != list_.cend()) ? qMakePair(true, it->section("#", 1))
                                : qMakePair(false, QString());
  };
  // -----------------------------------------------------------------------

  GlobalOptions glbOpts_;

  using ArgumentPair_ = QPair<QList<QString>, QList<QString>>;
  using ArgumentMap_ = QMap<QString, ArgumentPair_>;

  // Columns setup
  setColumnWidth(0, 500);
  setColumnWidth(1, 200);
  setColumnWidth(2, 200);
  setColumnWidth(3, 200);
  setColumnWidth(4, 200);

  // Initialize the root item
  rootItem_ = new QTreeWidgetItem(this);
  rootItem_->setText(0, scriptName_);

  for (QString& key_ : dm_.keys()) { // repoNames
    QCheckBox* repoBox_ = new QCheckBox(key_.section('#', 0, 0), this);
    repoBox_->setChecked(false);
    levelItem1_ = new QTreeWidgetItem(rootItem_);
    setItemWidget(levelItem1_, 0, repoBox_);

    QLineEdit* archive_ = new QLineEdit(key_.section('#', 1), this);
    archive_->setMaxLength(254);
    setItemWidget(levelItem1_, 1, archive_);

    auto optionalEndIt_ = GlobalOptions::cendMapOptional();
    auto optionalBeginIt_ = GlobalOptions::cbeginMapOptional();

    auto commonEndIt_ = GlobalOptions::cendMap();
    auto commonBeginIt_ = GlobalOptions::cbeginMap();

    while (commonBeginIt_ != commonEndIt_ ||
           optionalBeginIt_ != optionalEndIt_) {

      const QString col1_ = (commonBeginIt_ != commonEndIt_)
                              ? commonBeginIt_->second.first
                              : QString();
      const QString col2_ = (optionalBeginIt_ != optionalEndIt_)
                              ? optionalBeginIt_->second.first
                              : QString();

      GlobalOptions::GlobalOptions_e opt_ = commonBeginIt_->first;

      // Commons traits ---------------------------------------------------
      levelItem2_ = new QTreeWidgetItem(levelItem1_);
      if (!col1_.isEmpty()) {
        QCheckBox* cmBox =
          new QCheckBox(col1_, this); // First element of QPair is the name
        setItemWidget(levelItem2_, 2, cmBox);
        cmBox->setToolTip(
          commonBeginIt_->second
            .second); // Second element of QPair is the description
        cmBox->setChecked(opt_ == GlobalOptions::GlobalOptions_e::Show_rc ||
                          opt_ == GlobalOptions::GlobalOptions_e::Info ||
                          opt_ == GlobalOptions::GlobalOptions_e::Iec);
      }

      // "Optionals" traits -------------------------------------------------
      if (!col2_.isEmpty()) {
        auto splittedParm_pair_ = splitParameter(optionalBeginIt_->second.first,
                                                 dm_.find(key_)->second);
        auto* argBox_ = new QCheckBox(optionalBeginIt_->second.first, this);
        argBox_->setToolTip(
          optionalBeginIt_->second
            .second); // Second element of QPair is the description
        argBox_->setChecked(splittedParm_pair_.first);
        setItemWidget(levelItem2_, 3, argBox_);
        setProperties(optionalBeginIt_->first, splittedParm_pair_.second);
      }

      // Increments
      commonBeginIt_++;
      optionalBeginIt_++;
    } // while()
  } // repoNames
}

/*!
 * \brief treeWidget_CreateBackup_P::populate
 */
void
treeWidget_CreateBackup_P::populate()
{
  // qDebug() << __FUNCTION__;
  GlobalOptions glbOpts_;
  Command args_;

  setColumnWidth(0, 500);
  setColumnWidth(1, 200);
  setColumnWidth(2, 200);
  setColumnWidth(3, 200);
  setColumnWidth(4, 200);

  QStringList repoNames_ = getScriptNames();

  // merge
  QSet<QPair<QString, QString>> mergedKeys_;
  for (auto& key_ : mergedKeys_) {
    auto begin_ = static_cast<int>(GlobalOptions::eGlobalOptions::Critical);
    auto end_ = static_cast<int>(GlobalOptions::eGlobalOptions::Rsh);
  }

  if (repoNames_.count() > 0) {
    rootItem_ = new QTreeWidgetItem(this);
    rootItem_->setText(0, scriptName_);

    for (const auto& a_ : std::as_const(repoNames_)) {
      QCheckBox* repoBox_ = new QCheckBox(a_, this);
      repoBox_->setChecked(false);
      levelItem1_ = new QTreeWidgetItem(rootItem_);
      setItemWidget(levelItem1_, 0, repoBox_);

      QLineEdit* archive_ = new QLineEdit("", this);
      archive_->setMaxLength(254);
      setItemWidget(levelItem1_, 1, archive_);

      auto optionalEndIt_ = GlobalOptions::cendMapOptional();
      auto optionalBeginIt_ = GlobalOptions::cbeginMapOptional();

      auto commonEndIt_ = GlobalOptions::cendMap();
      auto commonBeginIt_ = GlobalOptions::cbeginMap();

      /*!
       *Inserts the unbalanced columns "common options" and
       * "optional options" side by side correctly into the table.
       */
      while (commonBeginIt_ != commonEndIt_ ||
             optionalBeginIt_ != optionalEndIt_) {
        const QString col1_ = (commonBeginIt_ != commonEndIt_)
                                ? commonBeginIt_->second.first
                                : QString();
        const QString col2_ = (optionalBeginIt_ != optionalEndIt_)
                                ? optionalBeginIt_->second.first
                                : QString();

        GlobalOptions::GlobalOptions_e opt_ = commonBeginIt_->first;

        levelItem2_ = new QTreeWidgetItem(levelItem1_);
        if (!col1_.isEmpty()) {
          QCheckBox* cmBox_ =
            new QCheckBox(col1_, this); // First element of QPair is the name
          setItemWidget(levelItem2_, 2, cmBox_);
          cmBox_->setToolTip(
            commonBeginIt_->second
              .second); // Second element of QPair is the description
          cmBox_->setChecked(opt_ == GlobalOptions::GlobalOptions_e::Show_rc ||
                             opt_ == GlobalOptions::GlobalOptions_e::Info ||
                             opt_ == GlobalOptions::GlobalOptions_e::Iec);
        }
        // Adds a optional argument item in column 3, if it exists
        if (!col2_.isEmpty()) {
          QCheckBox* argBox_ = new QCheckBox(col2_, this);
          argBox_->setChecked(false); // All items are 'falses' by default
          argBox_->setToolTip(
            optionalBeginIt_->second
              .second); // Second element of QPair is the description
          setItemWidget(levelItem2_, 3, argBox_);
          switch (optionalBeginIt_->first) {
            case GlobalOptions::Optionals_e::Filter: {
              OptionalsLineEdit* filter_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, filter_);
              break;
            }
            case GlobalOptions::Optionals_e::StdinName: {
              OptionalsLineEdit* name_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, name_);
              break;
            }
            case GlobalOptions::Optionals_e::Stdinuser: {
              OptionalsLineEdit* user_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, user_);
              break;
            }
            case GlobalOptions::Optionals_e::StdinGroup: {
              OptionalsLineEdit* group_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, group_);
              break;
            }
            case GlobalOptions::Optionals_e::StdinMode: {
              OptionalsLineEdit* mode_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, mode_);
              break;
            }
            case GlobalOptions::Optionals_e::PathsDelimiter: {
              OptionalsLineEdit* paths_delim_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, paths_delim_);
              break;
            }
            case GlobalOptions::Optionals_e::Exclude: {
              OptionalsLineEdit* exclude_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, exclude_);
              break;
            }
            case GlobalOptions::Optionals_e::ExcludeFrom: {
              OptionalsLineEdit* exclude_from_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, exclude_from_);
              break;
            }
            case GlobalOptions::Optionals_e::Pattern: {
              OptionalsLineEdit* pattern_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, pattern_);
              break;
            }
            case GlobalOptions::Optionals_e::PatternsFrom: {
              OptionalsLineEdit* exclude_from_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, exclude_from_);
              break;
            }
            case GlobalOptions::Optionals_e::Comment: {
              OptionalsLineEdit* comment_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, comment_);
              break;
            }
            case GlobalOptions::Optionals_e::Compression: {
              OptionalsLineEdit* compression_ =
                new OptionalsLineEdit(optionalBeginIt_->first, this);
              setItemWidget(levelItem2_, 4, compression_);
              break;
            }

            default: {
              break;
            }
          }
        }

        // Increments
        commonBeginIt_++;
        optionalBeginIt_++;
      }
    } // reponames
  }
}

/*!
 * \brief treeWidget_CreateBackup_P::clearTree
 */
void
treeWidget_CreateBackup_P::clearTree()
{
  if (rootItem_) {
    clear();
    // Reset the rootItem_ pointer (without deleting directly)
    rootItem_ = nullptr;
  }
}
