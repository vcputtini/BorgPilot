/***************************************************************************
 * Copyright (c) 2025                                                      *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      <vcputtini@gmail.com> | <vcputtini@vcplinux.com.br>
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

#ifndef FORMUTILITIES_H
#define FORMUTILITIES_H

#include <QModelIndex>
#include <QWidget>

#include <QButtonGroup>
#include <QList>
#include <QPair>
#include <QRadioButton>
#include <QVector>

#include <functional>

#include "comboboxcmdoptions_p.h"
#include "command2.h"
#include "listviewrepos_p.h"
#include "listwidget_delete_p.h"
#include "listwidget_prune_p.h"
#include "types_global.h"

namespace Ui {
class FormUtilities;
}

/*!
 * \brief The FormUtilities class
 */
class FormUtilities : public QWidget
{
  Q_OBJECT

public:
  explicit FormUtilities(QWidget* parent = nullptr);
  ~FormUtilities() override;

private:
  Ui::FormUtilities* ui;

  ListViewRepos_P* listViewRepos_P;
  ComboBoxCmdOptions_P* comboBoxCmdOptions_P;
  ListWidget_Delete_P* listWidget_Delete_P;
  ListWidget_Prune_P* listWidget_Prune_P;

  QHash<QRadioButton*, Command::Command_EC> radiosHash_;

  QString nameRepoSelected_;
  QString archiveSelected_;
  QString commandSelected_;

  struct RepoArchiveInfo
  {
    QString destRepo;                         // A_DESTREPO VALUE'S
    AppTypes::NamedValueList repoAndArchives; // pairs (REPONAME, ARCHIVE)
  };
  [[nodiscard]] RepoArchiveInfo findRepoAndArchive(
    const AppTypes::NamedValueList& dataList) const;

  void configBehaviorRadioButtons();
  void configBehaviorComboBoxes();
  void configBehaviorToolButtons();
  void configBehaviorListWidget();

  /**
   * \brief Lists all child objects of a QWidget that are of type T and executes
   * a function on each one.
   *
   * This generic function loops through all child objects of type T* of the
   * widget given as `parent`. For each object found, executes the given
   * function (`func`) on it.
   *
   * \tparam T Type of child objects to be searched for.
   * \param parent Pointer to the parent QWidget that will be inspected.
   * \param func Function (or lambda) to be executed for each object of type T*
   * found.
   *
   * \note Raises a warning (`qWarning`) and returns if `parent` is null.
   */

  template<typename T>
  void listObjects(QWidget* parent, const std::function<void(T*)> func)
  {
    if (!parent) {
      qWarning() << "Parent Widget Invalid!";
      return;
    }
    QList<T*> objetos = parent->findChildren<T*>();
    for (T* obj : objetos) {
      func(obj); // Exec lambda
    }
  }

private slots:
  void execCommand();
  void actionSelectedRadio(QRadioButton* button_ = nullptr);
};

#endif // FORMUTILITIES_H
