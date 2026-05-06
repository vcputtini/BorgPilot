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
#ifndef FORMGRAPHICS_H
#define FORMGRAPHICS_H

#include <QMap>
#include <QMetaObject>
#include <QPointer>
#include <QSpinBox>
#include <QStringList>
#include <QToolTip>
#include <QWidget>
#include <array>
#include <concepts>
#include <memory>
#include <optional>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

#include "borginfocommandparser.h"

namespace Ui {
class FormGraphics;
}

/*!
 * \class FormGraphics
 * \brief Manages the visual representation of BorgBackup deduplication metrics.
 * * This class uses Qt Charts to provide a visual comparison between individual
 * backup snapshots (Archives) and the overall repository storage health.
 */
class FormGraphics : public QWidget
{
  Q_OBJECT

public:
  explicit FormGraphics(QWidget* parent = nullptr);
  ~FormGraphics() override;

  /*!
   * \brief Parses JSON data and triggers the chart generation sequence.
   */
  void createBarChartModel_01();

private slots:
  /*! \brief populates the tree widget with detected repository names. */
  void sl_listRepoNames();

  /*! \brief Updates the repository search filter pattern. */
  void sl_setPattern(const QString& pattern);

  /*! \brief */
  void sl_pushButon_ApplyFilter();

  /*! \brief Logs diagnostic information regarding the current data state. */
  [[maybe_unused]] void debug() noexcept;

  /*! \brief Tratar o hover nas barras*/
  void sl_showBarTooltip(bool status, int index, QBarSet* barset);

private:
  /*! \brief Configures UI spin boxes for axis scaling and steps. */
  void setupSpinBoxes();

  /*!
   * \brief Technical metrics retrieved from BorgBackup.
   * \note Tracks Original, Compressed, and actual Deduplicated disk usage.
   */
  struct BorgMetrics
  {
    qreal original;     /*!< Uncompressed source data size. */
    qreal compressed;   /*!< Size after compression but before global
                           deduplication. */
    qreal deduplicated; /*!< Final physical space occupied on disk. */
  };

  /*!
   * \brief The BorgDataPoints class
   *
   * ira substituir Borgmetrics
   */
  struct BorgDataPoints
  {
    QString label;
    qreal original;
    qreal compressed;
    qreal deduplicated;
    qreal ratio; // Calculado: (1 - dedup/original) * 100
  };

  // Auxiliar para formatação de bytes
  [[nodiscard]] QString formatBytes(qreal gigabytes) const;

  /*!
   * \brief Refreshes the bar chart to compare an archive's impact against the
   * repository.
   * \param archiveName The identifier for the specific backup snapshot.
   * \param archive Metrics for the selected point-in-time backup.
   * \param repo Global metrics for the entire backup repository.
   */
  void updateComparisonChart(const QString& archiveName,
                             const BorgMetrics& archive,
                             const BorgMetrics& repo);

  /*! \brief Initializes the primary layout for the graphics widget. */
  void setupLayout();

  /*! \brief Connects UI control signals to their respective slots. */
  void setupToolButtons();

  /*! \brief Initializes columns and behavior for the repository tree. */
  void setupTreeWidgetRepos();

  /*! \brief Clears the current chart and releases view resources. */
  void clearGraphic() noexcept;

  /*! \brief Sets the main display title for the chart. */
  void setTitle(const QString& title) noexcept;

  /*! \brief Sets category labels for the X-axis. */
  void setCategories(const QStringList& categories) noexcept;

  /*! \brief Assigns labels and values to the active bar sets. */
  void setBarLabels(QStringList& labels, QList<qreal>& values) noexcept;

  /*!
   * \brief Factory method that creates a styled QChart instance.
   * \return QPointer<QChart> A monitored pointer to the new chart.
   */
  [[nodiscard]] QPointer<QChart> createBarChart() noexcept;

  /*! \brief Fetches the list of backup scripts available in the system. */
  QStringList getScriptNames() noexcept;

  std::unique_ptr<Ui::FormGraphics>
    ui; /*!< Managed pointer to the UI generated from formgraphics.ui. */
  QPointer<QChartView>
    m_chartView; /*!< Widget responsible for rendering the chart. */
  QPointer<QBarSeries>
    m_series_; /*!< Container for the current chart data series. */

  QString m_mainTitle_;
  QString m_json_info_; //!< text resulting from the command borg info --json*/
  QString m_destRepo_;
  std::optional<double>
    m_usedTotalGB_; /*!< Physical storage capacity in GB for axis scaling. */
  std::optional<double> m_freeSpaceGB_;

  QStringList m_listCategories_;
  QStringList m_listBarLabels_;
  QMap<int, QBarSet*>
    m_barset_map_; /*!< Map for tracking and modifying active bar sets. */

  int m_filterFirst_;
  int m_filterLast_;

  std::optional<int> m_filterValue_;

  static constexpr int TOOLTIP_DURATION_MS = 80000;

  // HELPERS
  /*!
   * \brief Validates if an object pointer is a QSpinBox and if it has a value
   * different from zero.
   * \tparam T Object type (must derive from QObject).
   * \param pointer Pointer to the object to be tested.
   * \return std::optional<int> Contains the value if it is a QSpinBox and != 0;
   * otherwise, std::nullopt.
   *
   * \note C++20 concepts: Restricts the template at compile time to accept only
   * pointers to classes derived from QObject. Prevents passing primitive types
   * or classes that do not have the Qt Meta-Object system.
   */
  template<typename T>
    requires std::derived_from<T, QObject>
  [[nodiscard]] auto validateSpinBox(T* pointer) -> std::optional<int>
  {
    // Tests whether the object is a QSpinBox (or derives from it).
    auto* spinBox = qobject_cast<QSpinBox*>(pointer);

    if (!spinBox) {
      return std::nullopt; // is equivalent to 'false'
    }

    // It retrieves the value and checks if it is zero.
    const int value = spinBox->value();

    if (value == 0) {
      return std::nullopt;
    }

    return value;
  }
};

#endif // FORMGRAPHICS_H
