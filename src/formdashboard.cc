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

#include "formdashboard.h"
#include "ui_formdashboard.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

#include "borginfocommandparser.h"
#include "borgrunnerthread.h"
#include "global_tools.h"
#include "settingshandler.h"

FormDashboard::FormDashboard(QWidget* parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::FormDashboard>())
{
  ui->setupUi(this);
  setupCustomLayout();
  applyModernStyling();
  recoveryRepoData();
}

FormDashboard::~FormDashboard() = default;

void
FormDashboard::recoveryRepoData()
{
  // Dynamically extracts script names from QSettings.
  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());
  const QStringList sections_ = settings_.childGroups();

  QStringList scriptNames;
  const QString prefix = QStringLiteral("INITREPO_");

  for (const QString& section_ : std::as_const(sections_)) {
    if (section_.startsWith(prefix)) {
      // Extrai o nome do script (ex: "BackupHome") removendo o prefixo
      scriptNames.append(section_.mid(prefix.length()));
    }
  }

  if (scriptNames.isEmpty()) {
    QMessageBox::warning(this, tr("Warning"), tr("No startup script found."));
    return;
  }

  SettingsHandler sHandler;
  Globals gTools;

  // Iterates over each script.
  for (const QString& currentScript : std::as_const(scriptNames)) {
    // Read the specific definitions of the current script.
    AppTypes::NamedValueList list = sHandler.readInitScriptDefs(currentScript);
    auto pairs = gTools.findRepoAndArchive(list);

    // It iterates over the repositories/files of this script.
    for (const auto& it : std::as_const(pairs.repoAndArchives)) {
      const QString repoPath =
        QStringLiteral("%1/%2").arg(pairs.destRepo, it.first);

      QStringList args;
      args << repoPath << QStringLiteral("--info") << QStringLiteral("--json");

      auto* thread =
        new BorgThreadRunner(BorgThreadRunner::BorgCommands::Info, args, this);

      connect(thread, &QThread::finished, thread, &QObject::deleteLater);
      connect(thread,
              &BorgThreadRunner::sig_borgOutput,
              this,
              [this, repoPath, currentScript](const QString& outputBlock) {
                if (outputBlock.isEmpty()) {
                  return;
                }

                //!< \note: In parallel executions, this variable will contain
                //!< the last result received.
                this->m_json_info_ = outputBlock;

                // JSON parser
                if (auto parserOpt =
                      BorgInfoCommandParser::fromJson(outputBlock);
                    parserOpt.has_value()) {
                  // processing for metadata logic
                  this->processRepositoryMetadata(repoPath, *parserOpt);
                } else {
                  qCritical().noquote()
                    << QStringLiteral("JSON parser failed: %1 (Script: %2)")
                         .arg(repoPath, currentScript);
                }
              });
      connect(thread,
              &BorgThreadRunner::sig_borgError,
              this,
              [repoPath, currentScript](const QString& errorLine) {
                qWarning().noquote()
                  << QStringLiteral("Borg Error [%1 | %2]: %3")
                       .arg(currentScript, repoPath, errorLine.trimmed());
              });

      // Start safe thread
      if (!thread->isRunning()) {
        thread->start();
      }
    }
  }

// #define SAMPLE
#ifdef SAMPLE
  // Create the data map
  RepoDataMap mockData;

  // We use the ULL (unsigned long long) suffix to avoid overflow in
  // calculations of bytes
  mockData["Servidor_Producao"] = {
    1099511627776ULL, // 1 TB Raw
    858993459200ULL,  // 800 GB Comp
    536870912000ULL   // 500 GB Dedup
  };

  mockData["Backup_Local_Docs"] = {
    53687091200ULL, // 50 GB Raw
    42949672960ULL, // 40 GB Comp
    10737418240ULL  // 10 GB Dedup
  };

  // Inject the data into the widget.
  updateFromBorgData(mockData);
  show();
#endif
} // FormDashboard::recoveryRepoData()

void
FormDashboard::processRepositoryMetadata(const QString& repoPath,
                                         const BorgInfoCommandParser& parser)
{
  // Extraction and conversion of types (Ensuring uint64_t). The parser methods
  // return qint64, we convert it to uint64_t according to the struct.
  RepositoryMetrics metrics;
  metrics.rawSize = static_cast<uint64_t>(parser.cacheTotalSize());
  metrics.compressedSize = static_cast<uint64_t>(parser.cacheTotalCSize());
  metrics.deduplicatedSize = static_cast<uint64_t>(parser.cacheUniqueCSize());

  {
    // Critical: Map Update
    // Since the sig_borgOutput signal comes from different threads,
    // we ensure that writing to QMap is atomic.
    std::lock_guard<std::mutex> lock(m_statsMutex);

    // Insertion | update
    m_repositoryStats[repoPath] = metrics;
  }

#if 0
  qDebug() << "Calculated metrics for" << repoPath
           << "-> Raw:" << (metrics.rawSize / (1024.0 * 1024.0 * 1024.0))
           << "GB";
#endif

  // Updating data in the UI
  updateFromBorgData(m_repositoryStats);
  show();
}

void
FormDashboard::setupCustomLayout()
{
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(20, 20, 20, 20);
  mainLayout->setSpacing(25);

  // Top Row: KPI Cards (Side-by-Side)
  auto* cardsLayout = new QHBoxLayout();

  auto createKpiCard = [&](const QString& title, const QString& id) {
    auto* frame = new QFrame();
    frame->setObjectName("KpiCard");
    auto* vLayout = new QVBoxLayout(frame);

    auto* val = new QLabel("--");
    val->setObjectName("KpiValue");
    val->setAlignment(Qt::AlignCenter);

    auto* titleLab = new QLabel(title);
    titleLab->setObjectName("KpiTitle");
    titleLab->setAlignment(Qt::AlignCenter);

    vLayout->addWidget(val);
    vLayout->addWidget(titleLab);

    m_kpiMap[id] = { val, titleLab };
    cardsLayout->addWidget(frame);
  };

  createKpiCard(tr("REPOSITORIES"), "count");
  createKpiCard(tr("TOTAL ORIGINAL"), "totalRaw");
  createKpiCard(tr("TOTAL DEDUP"), "totalDedup");
  createKpiCard(tr("EFICIENCY"), "efficiency");

  mainLayout->addLayout(cardsLayout);

  // Central Section: Details Table ---
  auto* tableLabel = new QLabel(tr("Detailed by Repository"));
  tableLabel->setStyleSheet(
    "font-size: 14pt; font-weight: bold; color: #343a40;");
  mainLayout->addWidget(tableLabel);

  m_repoTable = new QTableWidget(0, 4);
  m_repoTable->setHorizontalHeaderLabels({ tr("Repository Name"),
                                           tr("Original (Raw)"),
                                           tr("Compressed"),
                                           tr("Deduplicate") });

  // Table behavior configuration
  m_repoTable->horizontalHeader()->setSectionResizeMode(0,
                                                        QHeaderView::Stretch);
  m_repoTable->setAlternatingRowColors(true);
  m_repoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_repoTable->verticalHeader()->setVisible(false);

  mainLayout->addWidget(m_repoTable);
}

void
FormDashboard::updateFromBorgData(const RepoDataMap& dataMap)
{
  if (!m_repoTable) {
    return;
  }

  m_repoTable->setRowCount(0);
  uint64_t gRaw{ 0 }, gComp{ 0 }, gDedup{ 0 };

  for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
    const auto& name = it.key();
    const auto& m = it.value();

    gRaw += m.rawSize;
    gComp += m.compressedSize;
    gDedup += m.deduplicatedSize;

    int row = m_repoTable->rowCount();
    m_repoTable->insertRow(row);

    m_repoTable->setItem(row, 0, new QTableWidgetItem(name));
    m_repoTable->setItem(row, 1, new QTableWidgetItem(formatBytes(m.rawSize)));
    m_repoTable->setItem(
      row, 2, new QTableWidgetItem(formatBytes(m.compressedSize)));
    m_repoTable->setItem(
      row, 3, new QTableWidgetItem(formatBytes(m.deduplicatedSize)));

    for (int i = 1; i < 4; ++i) {
      m_repoTable->item(row, i)->setTextAlignment(Qt::AlignRight |
                                                  Qt::AlignVCenter);
    }
  }

  // Update Global KPIs
  m_kpiMap["count"].valueLabel->setText(QString::number(dataMap.size()));
  m_kpiMap["totalRaw"].valueLabel->setText(formatBytes(gRaw));
  m_kpiMap["totalDedup"].valueLabel->setText(formatBytes(gDedup));

  double eff = gDedup > 0 ? static_cast<double>(gRaw) / gDedup : 0.0;
  m_kpiMap["efficiency"].valueLabel->setText(QString::number(eff, 'f', 2) +
                                             "x");
}

void
FormDashboard::applyModernStyling()
{
#if 1
  // Definição da Paleta KDE Breeze Dark (Cores constantes para manutenção)
  const QString colorWindowBg =
    QStringLiteral("#232629"); // Cinza escuro principal
  const QString colorWidgetBg =
    QStringLiteral("#31363b"); // Cinza ligeiramente mais claro (Cards/Tabela)
  const QString colorAlternateBg =
    QStringLiteral("#2a2e32"); // Para o efeito zebra (sem ser branco)
  const QString colorText = QStringLiteral("#eff0f1"); // Branco "off-white"
  const QString colorTextSecondary =
    QStringLiteral("#bdc3c7"); // Cinza para labels secundárias
  const QString colorHighlight = QStringLiteral("#3daee9"); // Azul KDE Breeze
  const QString colorSelectionText =
    QStringLiteral("#ffffff");                           // Texto na seleção
  const QString colorBorder = QStringLiteral("#4d4d4d"); // Bordas discretas

  this->setStyleSheet(QString(R"(
        /* Background principal do Widget */
        #FormDashboard {
            background-color: %1;
        }

        /* Cards de KPI */
        #KpiCard {
            background-color: %2;
            border: 1px solid %8;
            border-radius: 6px;
        }

        #KpiValue {
            font-size: 22pt;
            font-weight: bold;
            color: %6;
        }

        #KpiTitle {
            font-size: 10pt;
            color: %5;
            font-weight: bold;
        }

        /* Padronização Total da Tabela */
        QTableWidget {
            background-color: %2;
            color: %4;
            gridline-color: %1;
            border: 1px solid %8;
            alternate-background-color: %3; /* FORÇA a cor da linha alternada */
            selection-background-color: %6;
            selection-color: %7;
            outline: none;
        }

        /* Garante que o item selecionado ignore cores de sistema */
        QTableWidget::item:selected {
            background-color: %6;
            color: %7;
        }

        /* Estilização do Cabeçalho */
        QHeaderView::section {
            background-color: %2;
            color: %5;
            padding: 8px;
            font-weight: bold;
            border: none;
            border-right: 1px solid %1;
            border-bottom: 1px solid %8;
        }

        /* Canto superior esquerdo da tabela onde os headers se cruzam */
        QTableCornerButton::section {
            background-color: %2;
            border: none;
            border-right: 1px solid %1;
            border-bottom: 1px solid %8;
        }

        /* Scrollbar padronizado para não destoar */
        QScrollBar:vertical {
            border: none;
            background: %1;
            width: 12px;
        }
        QScrollBar::handle:vertical {
            background: %8;
            min-height: 20px;
            border-radius: 6px;
            margin: 2px;
        }
        QScrollBar::handle:vertical:hover {
            background: %5;
        }
    )")
                        .arg(colorWindowBg,      // %1
                             colorWidgetBg,      // %2
                             colorAlternateBg,   // %3
                             colorText,          // %4
                             colorTextSecondary, // %5
                             colorHighlight,     // %6
                             colorSelectionText, // %7
                             colorBorder));      // %8

  // Certifica-se que a tabela está configurada para usar as cores alternadas
  // if (ui && ui->tableWidget_repositories) {
  //  ui->tableWidget_repositories->setAlternatingRowColors(true);
  // }
#else
  this->setStyleSheet(R"(
        #FormDashboard { background-color: #ffffff; }
        #KpiCard {
            background-color: #f8f9fa;
            border: 1px solid #dee2e6;
            border-radius: 10px;
        }
        #KpiValue {
            font-size: 20pt;
            font-weight: bold;
            color: #007bff;
        }
        #KpiTitle {
            font-size: 9pt;
            color: #6c757d;
            font-weight: bold;
        }
        QTableWidget {
            border: 1px solid #dee2e6;
            gridline-color: #f1f3f5;
            selection-background-color: #e7f1ff;
        }
        QHeaderView::section {
            background-color: #f8f9fa;
            padding: 8px;
            font-weight: bold;
            border: none;
            border-bottom: 2px solid #dee2e6;
        }
    )");
#endif
}

QString
FormDashboard::formatBytes(uint64_t bytes)
{
  if (bytes == 0)
    return "0 B";
  static const QStringList units = { "B", "KB", "MB", "GB", "TB", "PB" };
  int i = 0;
  double d_bytes = static_cast<double>(bytes);
  while (d_bytes >= 1024 && i < units.size() - 1) {
    d_bytes /= 1024;
    i++;
  }
  return QString("%1 %2").arg(d_bytes, 0, 'f', 2).arg(units[i]);
}
