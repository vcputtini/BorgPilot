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
#include "formgraphics.h"
#include "ui_formgraphics.h"

#include <QAbstractSpinBox>
#include <QLineSeries>
#include <QLinearGradient>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QtCharts/QBarSet>

#include "borgrunnerthread.h"
#include "global_tools.h"
#include "progid.h"
#include "settingshandler.h"

#include <filesystem>

using DiskInfo = Globals::DiskInfoFacility;

FormGraphics::FormGraphics(QWidget* parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::FormGraphics>())
  , m_filterFirst_{ 0 }
  , m_filterLast_{ 0 }
{
  ui->setupUi(this);
  setupToolButtons();
  setupTreeWidgetRepos();
  setupSpinBoxes();
}

FormGraphics::~FormGraphics() = default;

void
FormGraphics::createBarChartModel_01()
{
  auto borgInfo = BorgInfoCommandParser::fromJson(m_json_info_);
  if (!borgInfo)
    return;

  std::filesystem::path repoPath = borgInfo->repoLocation().toStdString();
  DiskInfo dskInfo(repoPath);

  // Hardware Stats
  double diskTotal = dskInfo.totalSpace(DiskInfo::Unit::GB).value_or(100.0);
  m_usedTotalGB_ = dskInfo.usedSpace(DiskInfo::Unit::GB).value_or(0.0);
  m_freeSpaceGB_ = dskInfo.freeSpace(DiskInfo::Unit::GB).value_or(0.0);

  ui->lcdNumber_StoSize->display(static_cast<int>(diskTotal));
  ui->lcdNumber_StoUsed->display(static_cast<int>(*m_usedTotalGB_));
  ui->lcdNumber_StoAvail->display(static_cast<int>(*m_freeSpaceGB_));

  constexpr double TO_GB = 1024.0 * 1024.0 * 1024.0;

  // Métricas do Archive Selecionado
  BorgMetrics archiveData{
    static_cast<qreal>(borgInfo->archiveOriginalSize() / TO_GB),
    static_cast<qreal>(borgInfo->archiveCompressedSize() / TO_GB),
    static_cast<qreal>(borgInfo->archiveDedupSize() / TO_GB)
  };

  // Métricas Globais do Repositório
  BorgMetrics repoData{ static_cast<qreal>(borgInfo->cacheTotalSize() / TO_GB),
                        static_cast<qreal>(borgInfo->cacheTotalCSize() / TO_GB),
                        static_cast<qreal>(borgInfo->cacheUniqueCSize() /
                                           TO_GB) };
  updateComparisonChart(borgInfo->archiveName(), archiveData, repoData);
}

void
FormGraphics::updateComparisonChart(const QString& archiveName,
                                    const BorgMetrics& archive,
                                    const BorgMetrics& repo)
{
  clearGraphic();

  auto chart = createBarChart();
  chart->setTitle(
    tr("Hardware Efficiency and Limit Analysis: %1").arg(archiveName));

  // 1. Configuração das Barras (Mesma lógica anterior)
  auto* setOriginal = new QBarSet(tr("Original (Raw)"));
  auto* setCompressed = new QBarSet(tr("Compressed"));
  auto* setDedup = new QBarSet(tr("Deduplicated (Disc)"));

  setOriginal->setColor(QColor(41, 128, 185));   // "#2980b9"
  setCompressed->setColor(QColor(22, 160, 113)); // #16a085
  setDedup->setColor(QColor(39, 174, 196));      // #27ae60

  *setOriginal << archive.original << repo.original;
  *setCompressed << archive.compressed << repo.compressed;
  *setDedup << archive.deduplicated << repo.deduplicated;

  m_series_ = new QBarSeries();
  m_series_->append({ setOriginal, setCompressed, setDedup });
  connect(
    m_series_, &QBarSeries::hovered, this, &FormGraphics::sl_showBarTooltip);
  chart->addSeries(m_series_);

  // 2. Implementação da Linha de Quota (Limite Físico)
  // Recuperamos o total do disco calculado em createBarChartModel_01
  double diskLimit =
    m_usedTotalGB_.value_or(0.0) + m_freeSpaceGB_.value_or(0.0);

  auto* quotaSeries = new QLineSeries();
  quotaSeries->setName(tr("Total Disk Capacity"));

  // Para a linha atravessar o gráfico de barras, usamos índices fictícios
  // que cobrem as categorias do BarSeries.
  quotaSeries->append(-0.5, diskLimit);
  quotaSeries->append(1.5, diskLimit);

  // Estilização da linha de "Alerta"
  QPen quotaPen(QColor(231, 76, 60)); // Red #e74c3c
  quotaPen.setWidth(3);
  quotaPen.setStyle(Qt::DashLine);
  quotaSeries->setPen(quotaPen);

  chart->addSeries(quotaSeries);

  // 3. Eixos e Escalonamento Inteligente
  auto* axisX = new QBarCategoryAxis();
  axisX->append({ tr("This Backup"), tr("Total Repository") });
  chart->addAxis(axisX, Qt::AlignBottom);
  m_series_->attachAxis(axisX);
  quotaSeries->attachAxis(axisX); // A linha compartilha o eixo X

  auto* axisY = new QValueAxis();
  axisY->setTitleText(tr("Capacity (GB)"));

  // O range deve ser o máximo entre o maior dado e o limite do disco
  qreal maxData = std::max({ archive.original, repo.original, diskLimit });
  axisY->setRange(0, maxData * 1.1); // 10% de margem superior

  chart->addAxis(axisY, Qt::AlignLeft);
  m_series_->attachAxis(axisY);
  quotaSeries->attachAxis(axisY);

  m_chartView = new QChartView(chart, this);
  m_chartView->setRenderHint(QPainter::Antialiasing);
  setupLayout();
}

void
FormGraphics::setupLayout()
{
  if (!ui->widget_Graphic->layout()) {
    new QVBoxLayout(ui->widget_Graphic);
  }
  ui->widget_Graphic->layout()->addWidget(m_chartView);
}

QPointer<QChart>
FormGraphics::createBarChart() noexcept
{
  auto* chart = new QChart();
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->setTheme(QChart::ChartThemeBlueIcy);
  return chart;
}

void
FormGraphics::setTitle(const QString& title) noexcept
{
  m_mainTitle_ = std::move(title);
}

void
FormGraphics::setCategories(const QStringList& categories) noexcept
{
  m_listCategories_ = std::move(categories);
}

void
FormGraphics::setBarLabels(QStringList& labels, QList<qreal>& values) noexcept
{
  // Clear previous hash to prevent leaks.
  qDeleteAll(m_barset_map_);
  m_barset_map_.clear();

  // Check equal sizes (important for label-value pairs)
  if (labels.size() != values.size()) {
    qWarning() << "Different sizes: labels(" << labels.size() << ") != values("
               << values.size() << ")";
    return;
  }

  // Loop for parallel (side by side) using index
  for (std::size_t i = 0; i < labels.size(); ++i) {
    // Create QBarSet with corresponding label
    QBarSet* barSet = new QBarSet(labels[static_cast<int>(i)]);

    // Adds float value to barSet (example: single bar)
    *barSet << static_cast<qreal>(values[static_cast<int>(i)]);

    // Stores in hash by int index
    m_barset_map_[static_cast<int>(i)] = barSet;
  }

#if 0
  // Debug  [index, label, value]
  qDebug() << "=== m_barset_map_ contents ===";
  for (auto it = m_barset_map_.cbegin(); it != m_barset_map_.cend(); ++it) {
    int idx = it.key();
    QString label = it.value()->label();
    qreal valor = it.value()->at(0); // First (unique) QBarSet's value

    qDebug()
      << QString("[%1, \"%2\", %3]").arg(idx).arg(label).arg(valor, 0, 'f', 2);
  }
  qDebug() << "========================";
#endif
}

void
FormGraphics::clearGraphic() noexcept
{
  if (m_chartView) {
    ui->widget_Graphic->layout()->removeWidget(m_chartView);
    m_chartView->deleteLater();
  }
}

// Auxiliares ================================================================

/*!
 * \brief Configures the tool button signals.
 *
 * Connects the buttons ui->toolButton_Exec to their respective slots or
 * lambdas, to:
 * - execute the repository listing;
 */
void
FormGraphics::setupToolButtons()
{
  connect(
    ui->toolButton_Exec, SIGNAL(clicked(bool)), this, SLOT(sl_listRepoNames()));
}

/*!
 * \brief
 */
void
FormGraphics::setupSpinBoxes()
{
  ui->radioButton_FilterNone->setChecked(true);
  ui->spinBox_FirstArchives->setEnabled(false);
  ui->spinBox_LastArchives->setEnabled(false);

  connect(ui->radioButton_FilterNone,
          &QRadioButton::clicked,
          this,
          [this](bool clicked) {
            if (clicked) {
              ui->spinBox_FirstArchives->setEnabled(false);
              ui->spinBox_FirstArchives->setValue(0);
              ui->spinBox_LastArchives->setEnabled(false);
              ui->spinBox_LastArchives->setValue(0);
            }
          });
  connect(ui->radioButton_FilterFirst,
          &QRadioButton::clicked,
          this,
          [this](bool clicked) {
            if (clicked) {
              ui->spinBox_FirstArchives->setEnabled(true);
              ui->spinBox_FirstArchives->setValue(0);
              ui->spinBox_LastArchives->setEnabled(false);
              ui->spinBox_LastArchives->setValue(0);
            }
          });

  connect(ui->radioButton_FilterLast,
          &QRadioButton::clicked,
          this,
          [this](bool clicked) {
            if (clicked) {
              ui->spinBox_FirstArchives->setEnabled(false);
              ui->spinBox_FirstArchives->setValue(0);
              ui->spinBox_LastArchives->setEnabled(true);
              ui->spinBox_LastArchives->setValue(0);
            }
          });
}

void
FormGraphics::sl_pushButon_ApplyFilter()
{
  if (ui->radioButton_FilterNone->isChecked()) {
    return;
  }
}

/*!
 * \brief Lists the repositories and their associated files from the selected
 * script.
 *
 * This function:
 * - checks if a script has been selected in ui->comboBox_ListScriptsNames;
 * - clears auxiliary panels (info, default, and treeWidget_Repos);
 * - gets a list of repositories via getScriptNames();
 * - loads script definitions using SettingsHandler and Globals;
 * - creates threads to list each repository with Borg::List and populates
 * ui->treeWidget_Repos with the files.
 */
void
FormGraphics::sl_listRepoNames()
{
  if (ui->comboBox_ListScriptsNames->currentText().isEmpty()) {
    QMessageBox::warning(this, tr("Warning"), tr("Script name not provided!"));
    ui->comboBox_ListScriptsNames->setFocus();
    return;
  }

  ui->treeWidget_Repos->clearTree();

  QStringList repoNames_ = getScriptNames();
  ui->treeWidget_Repos->addRepository(
    ui->comboBox_ListScriptsNames->currentText());

  Globals gTools_;
  SettingsHandler sHandler_;
  AppTypes::NamedValueList list_ =
    sHandler_.readInitScriptDefs(ui->comboBox_ListScriptsNames->currentText());
  auto pairs_ = gTools_.findRepoAndArchive(list_);

  // Check whether or not you should use filters.
  bool filter_{};
  if (ui->radioButton_FilterFirst->isChecked()) {
    m_filterValue_ = validateSpinBox(ui->spinBox_FirstArchives);
    filter_ = true;
  } else if (ui->radioButton_FilterLast->isChecked()) {
    m_filterValue_ = validateSpinBox(ui->spinBox_LastArchives);
    filter_ = false;
  } else {
    m_filterValue_ = std::nullopt;
  }

  for (const auto& it_ : std::as_const(pairs_.repoAndArchives)) {
    const QString repoPath_ = QString("%1/%2").arg(pairs_.destRepo, it_.first);
    const QString archive_ = it_.second;
    QStringList args_;
    if (m_filterValue_ == std::nullopt) {
      args_ << repoPath_ << R"(--format={archive}{NL})";
    } else {
      if (filter_) {
        args_ << repoPath_ << "--first" << QString::number(*m_filterValue_)
              << R"(--format={archive}{NL})";
      } else {
        args_ << repoPath_ << "--last" << QString::number(*m_filterValue_)
              << R"(--format={archive}{NL})";
      }
    }

    // qDebug() << args_;

    auto* thread_ =
      new BorgThreadRunner(BorgThreadRunner::BorgCommands::List, args_, this);
    connect(thread_,
            &BorgThreadRunner::sig_borgOutput,
            this,
            [this, repoPath_, archive_](const QString& outputBlock) {
              const QStringList lines_ =
                outputBlock.split('\n', Qt::SkipEmptyParts);
              for (const QString& line_ : lines_) {
                const QString trimmed_ = line_.trimmed();
                if (!trimmed_.isEmpty()) {
                  ui->treeWidget_Repos->addArchive(
                    QString("%1::%2").arg(repoPath_, trimmed_));
                }
              }
            });
    connect(thread_,
            &BorgThreadRunner::sig_borgError,
            this,
            [this, repoPath_](const QString& errorLine) {
              ui->treeWidget_Repos->addArchive(
                QString("Error while listing %1: %2")
                  .arg(repoPath_, errorLine.trimmed()));
            });
    connect(thread_, &QObject::destroyed, this, [thread_]() {
      thread_->deleteLater();
    });
    if (!thread_->isRunning()) {
      thread_->start();
    }
  } // for
}

/*!
 * \private
 * \brief Gets a list of repository names from the current settings.
 *
 * Reads a configuration group in QSettings, based on the selected script
 * in ui->comboBox_ListScriptsNames, and builds a list of strings with
 * full repository paths in the format "destRepo/repo::archive".
 * If the group does not exist or there are no valid entries, returns an empty
 * list.
 */
QStringList
FormGraphics::getScriptNames() noexcept
{
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());

  const QString completeGroupName_ =
    QString("INITREPO_%0")
      .arg(std::move(ui->comboBox_ListScriptsNames->currentText()));

  QStringList repoNames_;
  if (settings_.childGroups().contains(completeGroupName_)) {
    settings_.beginGroup(completeGroupName_);
    m_destRepo_ = std::move(settings_.value("A_DESTREPO").toString());
    for (int i = 0; i < 20; ++i) {
      const QString rName_ =
        std::move(settings_.value(QString("B_REPONAME_%1").arg(i)).toString());
      const QString rArchive_ =
        std::move(settings_.value(QString("B_ARCHIVE_%1").arg(i)).toString());

      if (rArchive_.isEmpty()) {
        continue;
      }
      const QString repoName_ = std::move(rName_ + "::" + rArchive_);

      if (!repoName_.isEmpty()) {
        repoNames_.append(std::move(m_destRepo_ + "/" + repoName_));
      }
    }
    settings_.endGroup();
  }
  return repoNames_;
}

/*!
 * \brief Configures the pattern signal in treeWidget_Repos.
 *
 * Connects the setPattern(const QString&) signal from ui->treeWidget_Repos
 * to the setPattern(const QString&) slot of this class, so that changes
 * to the search pattern are correctly passed to the form.
 */
void
FormGraphics::setupTreeWidgetRepos()
{
  connect(ui->treeWidget_Repos,
          SIGNAL(sig_setPattern(const QString&)),
          this,
          SLOT(sl_setPattern(const QString&)));
}

/*!
 * \brief Defines the search pattern and executes a Borg::Info command with
 * it.
 *
 * This function:
 * - populates ui->lineEdit_Pattern with the specified pattern;
 * - creates a BorgThreadRunner with the Borg::Info command using this
 * pattern;
 * - connects the output to m_json_info_ variable
 * - connects errors and thread termination to update the interface and clean
 * up resources.
 */
void
FormGraphics::sl_setPattern(const QString& pattern)
{
  Globals gTools_;
  QStringList args_;
  args_ << R"(--json)" << pattern;
  auto* thread_ =
    new BorgThreadRunner(BorgThreadRunner::BorgCommands::Info, args_, this);
  connect(thread_,
          &BorgThreadRunner::sig_borgOutput,
          this,
          [this, &gTools_, pattern](const QString& outputBlock) {
            const QStringList lines_ =
              outputBlock.split('\n', Qt::SkipEmptyParts);
            m_json_info_ = std::move(outputBlock);
            createBarChartModel_01();
            // debug();
          });
  connect(thread_,
          &BorgThreadRunner::sig_borgError,
          this,
          [this](const QString& errorLine) {
            m_json_info_ = std::move(
              QString("Error while listing %1: %2").arg(errorLine.trimmed()));
          });
  connect(thread_, &QObject::destroyed, this, [thread_]() {
    thread_->deleteLater();
  });
  if (!thread_->isRunning()) {
    thread_->start();
  }
}

void
FormGraphics::sl_showBarTooltip(bool status, int index, QBarSet* barset)
{
  if (!status) {
    QToolTip::hideText();
    return;
  }

  const double value = barset->at(index);
  const double diskLimit =
    m_usedTotalGB_.value_or(0.0) + m_freeSpaceGB_.value_or(0.0);
  const double percentUsed =
    (diskLimit > 0) ? (value / diskLimit) * 100.0 : 0.0;

  QLocale locale;
  const QString valStr = locale.toString(value, 'f', 2);
  const QString percStr = locale.toString(percentUsed, 'f', 1);
  const QString category =
    (index == 0) ? tr("Selected Backup") : tr("Repository Status");
  const QString color =
    (percentUsed > 90 ? QStringLiteral("red") : QStringLiteral("#27ae60"));

  QToolTip::showText(
    QCursor::pos(),
    tr("<div style='background: white; border: 1px solid #ccc; padding: 8px;'>"
       "<b style='color:#2c3e50;'>%1</b><br>"
       "Type: %2<br>"
       "Value: <b>%3 GB</b><br>"
       "Compromises: <span style='color:%4;'><b>%5% of the disk</b></span>"
       "</div>")
      .arg(category,        // %1
           barset->label(), // %2
           valStr,          // %3
           color,           // %4
           percStr),        // %5
    this,
    QRect(),
    TOOLTIP_DURATION_MS);
}

// HELPERS ==================================================================

// ---------------------------------------------------------------------------
/*!
 * \brief For debug only
 */
void
FormGraphics::debug() noexcept
{
  auto info = BorgInfoCommandParser::fromJson(m_json_info_);
  qDebug() << m_json_info_;
  // ── Archive ──────────────────────────────────────────────────────────────
  qDebug() << "=== ARCHIVE ===";
  qDebug() << "  Nome       :" << info->archiveName();
  qDebug() << "  ID         :" << info->archiveId();
  qDebug() << "  Hostname   :" << info->archiveHostname();
  qDebug() << "  Usuário    :" << info->archiveUsername();
  qDebug() << "  Início     :" << info->archiveStart();
  qDebug() << "  Fim        :" << info->archiveEnd();
  qDebug() << "  Duração    :" << info->archiveDuration() << "s";
  qDebug() << "  Comentário :" << info->archiveComment();
  qDebug() << "  Cmd line   :" << info->archiveCommandLine().join(u' ');
  qDebug() << "  Chunker    :" << info->archiveChunkerParams();

  // ── Archive › Stats ──────────────────────────────────────────────────────
  qDebug() << "\n=== STATS DO ARCHIVE ===";
  qDebug() << "  Arquivos       :" << info->archiveFileCount();
  qDebug() << "  Tam. original  :" << info->archiveOriginalSize() << "bytes";
  qDebug() << "  Tam. comprimido:" << info->archiveCompressedSize() << "bytes";
  qDebug() << "  Tam. dedup.    :" << info->archiveDedupSize() << "bytes";
  qDebug() << "  Max repo frac. :" << info->archiveMaxSize();

  // ── Cache
  // ─────────────────────────────────────────────────────────────────
  qDebug() << "\n=== CACHE ===";
  qDebug() << "  Caminho        :" << info->cachePath();
  qDebug() << "  Total chunks   :" << info->cacheTotalChunks();
  qDebug() << "  Chunks únicos  :" << info->cacheUniqueChunks();
  qDebug() << "  Total csize    :" << info->cacheTotalCSize() << "bytes";
  qDebug() << "  Total size     :" << info->cacheTotalSize() << "bytes";
  qDebug() << "  Unique csize   :" << info->cacheUniqueCSize() << "bytes";
  qDebug() << "  Unique size    :" << info->cacheUniqueSize() << "bytes";

  // ── Encryption ───────────────────────────────────────────────────────────
  qDebug() << "\n=== ENCRIPTAÇÃO ===";
  qDebug() << "  Modo :" << info->encryptionMode();

  // ── Repository ───────────────────────────────────────────────────────────
  qDebug() << "\n=== REPOSITÓRIO ===";
  qDebug() << "  ID             :" << info->repoId();
  qDebug() << "  Última modif.  :" << info->repoLastModified();
  qDebug() << "  Localização    :" << info->repoLocation();
}
