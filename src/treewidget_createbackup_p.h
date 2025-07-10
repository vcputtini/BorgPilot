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
#ifndef TREEWIDGET_CREATEBACKUP_P_H
#define TREEWIDGET_CREATEBACKUP_P_H

#include <QCheckBox>
#include <QLineEdit>
#include <QList>
#include <QMultiHash>
#include <QObject>
#include <QPair>
#include <QSettings>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>
#include <QWidget>

#include "global_options.h"

/*!
 * \brief The OptionalsLineEdit class
 */
class OptionalsLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit OptionalsLineEdit(GlobalOptions::Optionals_e optional_,
                             QWidget* parent = nullptr)
    : QLineEdit(parent)
  {
    switch (optional_) {
      case GlobalOptions::Optionals_e::Filter: {
        setMaxLength(5);
        setToolTip("Max. length: <b>1 byte</b><br><br>"
                   "Flags: <b>A|M|U|C|E</b><br><br>");
        QRegularExpression regExp_("[A|M|U|C|E]*"); // valid chars
        setValidator(new QRegularExpressionValidator(regExp_, this));
        break;
      }
      case GlobalOptions::Optionals_e::StdinName:
      case GlobalOptions::Optionals_e::Stdinuser:
      case GlobalOptions::Optionals_e::StdinGroup: {
        setToolTip("Max. length: <b>30 bytes</b><br><br>");
        setMaxLength(30);
        break;
      }
      case GlobalOptions::Optionals_e::StdinMode: {
        setMaxLength(4);
        setToolTip("Max. length: <b>4 byte</b><br><br>"
                   "Default: EMPTY = <b>0600</b><br><br>");
        QRegularExpression regExp_("[0-7]*");
        setValidator(new QRegularExpressionValidator(regExp_, this));
        break;
      }
      case GlobalOptions::Optionals_e::PathsDelimiter: {
        setMaxLength(5);
        break;
      }
      case GlobalOptions::Optionals_e::Exclude:
      case GlobalOptions::Optionals_e::ExcludeFrom:
      case GlobalOptions::Optionals_e::Pattern:
      case GlobalOptions::Optionals_e::PatternsFrom:
      case GlobalOptions::Optionals_e::ExcludeIfPresent: {
        setToolTip("Must be enclosed in single quotes: Ex.: '*.dat'");
        setMaxLength(254);
        QRegularExpression regExp_("^'[^']*'$");
        setValidator(new QRegularExpressionValidator(regExp_, this));
        break;
      }
      case GlobalOptions::Optionals_e::Dryrun:
      case GlobalOptions::Optionals_e::Stats:
      case GlobalOptions::Optionals_e::List:
      case GlobalOptions::Optionals_e::Json:
      case GlobalOptions::Optionals_e::NoCacheSync:
      case GlobalOptions::Optionals_e::ContentFromCommand:
      case GlobalOptions::Optionals_e::PathsFromStdin:
      case GlobalOptions::Optionals_e::PathsFromCommand:
      case GlobalOptions::Optionals_e::ExcludeCaches:
      case GlobalOptions::Optionals_e::KeepExcludeTags:
      case GlobalOptions::Optionals_e::ExcludeNoDump:
      case GlobalOptions::Optionals_e::OneFileSystem:
      case GlobalOptions::Optionals_e::NumericIds:
      case GlobalOptions::Optionals_e::NoAtime:
      case GlobalOptions::Optionals_e::Atime:
      case GlobalOptions::Optionals_e::NocTime:
      case GlobalOptions::Optionals_e::NoBirthtime:
      case GlobalOptions::Optionals_e::NoFlags:
      case GlobalOptions::Optionals_e::NoAcls:
      case GlobalOptions::Optionals_e::NoXAttres:
      case GlobalOptions::Optionals_e::Sparse:
      case GlobalOptions::Optionals_e::FilesCache:
      case GlobalOptions::Optionals_e::ReadSpecial:
      case GlobalOptions::Optionals_e::Comment:
      case GlobalOptions::Optionals_e::Timestamp:
      case GlobalOptions::Optionals_e::CheckpointInterval:
      case GlobalOptions::Optionals_e::ChunkerParams:
      case GlobalOptions::Optionals_e::Compression:
        break;
    }
  }
};

/*!
 * \brief The treeWidget_CreateBackup_P class
 */
class treeWidget_CreateBackup_P : public QTreeWidget
{
  Q_OBJECT
public:
  explicit treeWidget_CreateBackup_P(QWidget* parent = nullptr);

  void populate();

  void setScriptName(const QString&& name_) noexcept;

  using DATA_MAP_ = QMultiHash<QString, QPair<QStringList, QStringList>>;
  DATA_MAP_ getCheckedItems();

  void reloadCreateBackupSettings(const DATA_MAP_& dm_);

private:
  QSettings settings_;
  GlobalOptions commonOpts_;

  QTreeWidgetItem* rootItem_;
  QTreeWidgetItem* levelItem1_;
  QTreeWidgetItem* levelItem2_;
  QTreeWidgetItem* levelItem3_;

  QCheckBox* checkRepos_;

  QString scriptName_;
  QString destRepo_;

  void extracted(QStringList& scriptNames_);
  QStringList getScriptNames();
};

#endif // TREEWIDGET_CREATEBACKUP_P_H
