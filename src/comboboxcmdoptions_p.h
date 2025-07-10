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
#ifndef ComboBoxCmdOptions_P_H
#define ComboBoxCmdOptions_P_H

#include <QAbstractItemModel>
#include <QComboBox>
#include <QObject>
#include <QStandardItem>
#include <QString>
#include <QWidget>

#include <string>
#include <unordered_map>

/*!
 * \brief The ComboBoxCmdOptions_P class
 */
class ComboBoxCmdOptions_P : public QComboBox
{
  Q_OBJECT
public:
  explicit ComboBoxCmdOptions_P(QWidget* parent = nullptr);

  enum class DeleteOptions
  {
    DryRun = 0x00,
    List,
    Stats,
    Force,
    SaveSpace,
    KeepSecurityInfo
  };

  enum class PruneOptions
  {
    DryRun = 0x00,
    List,
    Stats,
    SaveSpace,
    KeepWithin,
    KeepLast,
    KeepSecondly,
    KeepMinutely,
    KeepHourly,
    KeepDaily,
    KeepWeekly,
    KeepMonthly,
    Keep13Weekly,
    Keep3Monthly,
    KeepYearly
  };

  void setOption(const QString& option);
  void uncheckAllItems(QComboBox* cb_ = nullptr);

public:
  void setModel(QAbstractItemModel* model_) override;

private:
  QStandardItemModel* model1_;

  static const std::unordered_map<DeleteOptions,
                                  std::pair<std::string, std::string>>
  initializeDeleteMap();
  static const std::unordered_map<PruneOptions,
                                  std::pair<std::string, std::string>>
  initializePruneMap();

  static const std::unordered_map<ComboBoxCmdOptions_P::DeleteOptions,
                                  std::pair<std::string, std::string>>
    Delete_m_;

  static const std::unordered_map<ComboBoxCmdOptions_P::PruneOptions,
                                  std::pair<std::string, std::string>>
    Prune_m_;

  template<typename CBoxType>
  void populateComboBox(CBoxType& cb_, const QString& mode_);

private slots:
  void handleSelectionChanged(int index);

signals:
  void optionSelected(const QString& value);
  void optionsChanged(const QString& checkdItems);
};

#endif // ComboBoxCmdOptions_P_H
