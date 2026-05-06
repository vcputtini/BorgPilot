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
#ifndef LISTWIDGET_DELETE_P_H
#define LISTWIDGET_DELETE_P_H

#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPair>
#include <QSet>
#include <unordered_map>

#include "types_global.h"

/*!
 * \brief The ListWidget_Delete_P class
 */
class ListWidget_Delete_P : public QListWidget
{
  Q_OBJECT

public:
  explicit ListWidget_Delete_P(QWidget* parent = nullptr);
  ~ListWidget_Delete_P() = default;

  void populate();
  void unCheckAllOptions();

  enum class DeleteOptions
  {
    DryRun = 0,
    List,
    Stats,
    Force,
    SaveSpace,
    KeepSecurityInfo
  };

private:
  [[nodiscard]] AppTypes::SelectedPairs getSelectedPairs() const;
  static const std::unordered_map<DeleteOptions,
                                  std::pair<std::string, std::string>>
    Delete_m_;

private slots:
  void sl_checkboxStateChanged();

signals:
  void sig_itemsSelectedChanged(QSet<QPair<QString, QString>> selected);
};

#endif // LISTWIDGET_DELETE_P_H
