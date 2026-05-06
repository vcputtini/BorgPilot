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
#ifndef SYSTEMREQUIREMENTSMODEL_H
#define SYSTEMREQUIREMENTSMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QString>
#include <vector>

/**
 * \brief POD structure for storing the state of system dependencies.
 */
struct ProgramData
{
  QString path;
  QString name;
  QString version;
  bool isInstalled{ false };
};

/**
 * \brief Customized model to abstract system verification data.
 * Specialization of QAbstractTableModel to be coupled to
 * tableView_Results. Uses C++ Standard Library (std::vector) for contiguous and
 * cache-friendly allocation.
 *
 */
class SystemRequirementsModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit SystemRequirementsModel(QObject* parent = nullptr);

  //!< \note Mandatory overrides of QAbstractTableModel (Qt 6)
  [[nodiscard]] int rowCount(
    const QModelIndex& parent = QModelIndex()) const override;
  [[nodiscard]] int columnCount(
    const QModelIndex& parent = QModelIndex()) const override;
  [[nodiscard]] QVariant data(const QModelIndex& index,
                              int role = Qt::DisplayRole) const override;
  [[nodiscard]] QVariant headerData(int section,
                                    Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const override;

  /**
   * \brief Starts scanning and populates the template.
   * \param essentialPrograms List of executable names (e.g., {"borg", "git",
   * "rsync"}).
   */
  void checkPrograms(const QStringList& essentialPrograms);

private:
  std::vector<ProgramData> m_data;

  /**
   * \brief Resolves the binary information (Path and Version) in the operating
   * system.
   * \param progName The name of the executable.
   * \return ProgramData filled in.
   */
  [[nodiscard]] ProgramData resolveProgramInfo(const QString& progName) const;
};

#endif // SYSTEMREQUIREMENTSMODEL_H
