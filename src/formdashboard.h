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
#ifndef FORMDASHBOARD_H
#define FORMDASHBOARD_H

#include "borginfocommandparser.h"
#include <QLabel>
#include <QMap>
#include <QTableWidget>
#include <QWidget>
#include <cstdint>
#include <memory>
#include <mutex>

namespace Ui {
class FormDashboard;
}

/*!
 * \brief Class responsible for managing and displaying backup metrics in the
 * Dashboard. This class orchestrates data recovery from Borg repositories,
 * metadata processing, and real-time updating of the graphical interface.
 */
class FormDashboard : public QWidget
{
  Q_OBJECT

public:
  explicit FormDashboard(QWidget* parent = nullptr);
  ~FormDashboard() override;

private:
  std::unique_ptr<Ui::FormDashboard> ui;

  /*!
   * \brief Structure for individual repository metrics. Stores the raw,
   * compressed, and deduplicated values ​​extracted from Borg.
   */
  struct RepositoryMetrics
  {
    uint64_t rawSize{ 0 };          // Tamanho original
    uint64_t compressedSize{ 0 };   // Tamanho após compressão
    uint64_t deduplicatedSize{ 0 }; // Tamanho real ocupado (após dedup)
  };

  // Map to facilitate searching by repository name/path.
  using RepoDataMap = QMap<QString, RepositoryMetrics>;
  RepoDataMap m_repositoryStats;
  std::mutex m_statsMutex; // Protect access to map

  /*!
   * \brief Updates all dashboard components based on a data map.
   * \param dataMap Map containing the repository name and its respective
   * metrics.
   */
  void updateFromBorgData(const RepoDataMap& dataMap);

  /*!
   * \brief Starts the process of retrieving data from all configured
   * repositories. This function scans the system settings to identify the
   * initialization scripts and triggers the Borg execution threads.
   * \sa processRepositoryMetadata()
   */
  void recoveryRepoData();

  /*!
   * \brief Initializes the layout of custom widgets and graphic elements.
   */
  void setupCustomLayout();

  /*!
   * \brief Applies the modern design system and style sheets (QSS) to widgets.
   */
  void applyModernStyling();

  /**
   * \brief Converts a numeric value in bytes to a human-readable representation
   * (e.g., GB, MB).
   * \param bytes The byte value to be converted.
   * \return QString containing the formatted value with the respective unit.
   */
  static QString formatBytes(uint64_t bytes);

  /*!
   * \brief Processes and integrates the metadata of a BorgBackup repository
   * into the Dashboard.
   * This function is responsible for receiving the already validated and parsed
   * data from the 'borg info --json' command and updating the internal data
   * structures (such as repository mapping) and the user interface.
   *
   * \param repoPath The full path or identifier of the processed repository.
   * \param parser A constant reference to the BorgInfoCommandParser object
   * containing the data extracted from the JSON.
   *
   * \note This function should preferably be called on the main thread (UI
   * thread) to ensure security when updating widgets.
   *
   * \sa recoveryRepoData(), BorgInfoCommandParser
   */
  void processRepositoryMetadata(const QString& repoPath,
                                 const BorgInfoCommandParser& parser);

  /*! \brief Structure for quick access to KPI labels. */
  struct KpiWidgets
  {
    QLabel* valueLabel;
    QLabel* titleLabel;
  };
  QMap<QString, KpiWidgets> m_kpiMap;
  QTableWidget* m_repoTable{ nullptr };

  /*! \brief Mapping metric names to their respective widgets in the UI. */
  QString m_json_info_; //!< text resulting from the command borg info --json*/
};

#endif // FORMDASHBOARD_H
