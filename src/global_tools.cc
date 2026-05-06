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
#include "global_tools.h"

#include "borgrunnerthread.h"

#include <QFontDatabase>
#include <QRegularExpression>
#include <QStringList>

#include <cmath>
// #include <sys/statvfs.h>
#include <system_error>

/*!
 * \brief Globals::Globals
 * \param parent
 */
Globals::Globals(QObject* parent) {}

const int
displayMessages(const QString& text, const QString& info) noexcept
{
  QMessageBox msgBox;
  msgBox.setText(QObject::tr(text.toUtf8()));
  msgBox.setInformativeText(QObject::tr(info.toUtf8()));
  msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Save);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  const int ret_ = msgBox.exec();
  return ret_;
}

/*!
 * \brief Globals::formatarBorgInfo
 * \param outputRaw
 * \param textBrowser
 * Command line: /usr/bin/borg --info --iec --show-rc create
 * '/dados2/backup_teste/BKP01::{hostname}-{now:%Y-%m-%d}'
 * /home/volnei/Documentos/
 */
void
Globals::formatBorgInfo(const QString& outputRaw, QTextBrowser* textBrowser)
{
  textBrowser->clear();

  QStringList borgComm_{};
  borgComm_ << "--info" << "--iec" << "--show-rc" << "create"
            << "'/dados2/backup_teste/BKP01::{hostname}-{now:%Y-%m-%d}'"
            << "/home/volnei/Documentos/";
  auto* thread_ =
    new BorgThreadRunner(BorgThreadRunner::BorgCommands::Info, borgComm_, this);
  Q_ASSERT(thread_);

  connect(thread_,
          &BorgThreadRunner::sig_borgOutput,
          this,
          [=, this](const QString& line) { textBrowser->append(line); });
  connect(
    thread_,
    &BorgThreadRunner::sig_borgError,
    this,
    [=, this](const QString& err) { textBrowser->append("[ERROR] " + err); });
  connect(thread_,
          &BorgThreadRunner::sig_borgFinished,
          this,
          [=, this](int code, QProcess::ExitStatus status) {
            textBrowser->append("Completed (code " + QString::number(code) +
                                ")");
            thread_->deleteLater();
          });

  // textBrowser->append("Executing...");

  if (!thread_->isRunning()) {
    thread_->start();
  }

  // QString html = genHtmlBorgInfo(outputRaw);
  // textBrowser->setHtml(html);
}

/*!
 * \brief Globals::genHtmlBorgInfo
 * \param output
 * \return
 */
QString
Globals::genHtmlBorgInfo(const QString& output)
{
  QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <style>
        body { font-family: 'Segoe UI', Ubuntu, sans-serif; margin: 20px; line-height: 1.4; }
        h2 { color: #2E7D32; border-bottom: 2px solid #4CAF50; padding-bottom: 5px; }
        .info-box { background: #E8F5E8; border-left: 4px solid #4CAF50; padding: 15px; margin: 10px 0; }
        .section { background: #F5F5F5; border-radius: 8px; padding: 15px; margin: 10px 0; }
        .stats-table { width: 100%; border-collapse: collapse; margin: 10px 0; }
        .stats-table th, .stats-table td { padding: 8px; text-align: right; border-bottom: 1px solid #ddd; }
        .stats-table th { background: #4CAF50; color: white; font-weight: bold; }
        .highlight { font-weight: bold; color: #1976D2; }
        .chunks { font-family: monospace; }
    </style>
</head>
<body>
)";

  // Extrair e formatar informações principais
  QStringList linhas = output.split('\n', Qt::SkipEmptyParts);

  QString archiveName, fingerprint, hostname, username;
  QDateTime startTime, endTime;
  qint64 duration = 0, numFiles = 0;
  double originalSize = 0, compressedSize = 0, dedupSize = 0;
  int uniqueChunks = 0, totalChunks = 0;

  for (const QString& linha : linhas) {
    if (linha.startsWith("Archive name:"))
      archiveName = linha.mid(13).trimmed();
    else if (linha.startsWith("Archive fingerprint:"))
      fingerprint = linha.mid(20).trimmed();
    else if (linha.startsWith("Hostname:"))
      hostname = linha.mid(10).trimmed();
    else if (linha.startsWith("Username:"))
      username = linha.mid(10).trimmed();
    else if (linha.startsWith("Time (start):")) {
      startTime = QDateTime::fromString(linha.mid(13).trimmed(),
                                        "ddd, yyyy-MM-dd hh:mm:ss");
    } else if (linha.startsWith("Time (end):")) {
      endTime = QDateTime::fromString(linha.mid(11).trimmed(),
                                      "ddd, yyyy-MM-dd hh:mm:ss");
    } else if (linha.startsWith("Duration:")) {
      QString dur = linha.mid(10).trimmed();
      duration = QTime::fromString(dur).msecsSinceStartOfDay() / 1000;
    } else if (linha.startsWith("Number of files:")) {
      numFiles = linha.mid(17).trimmed().toInt();
    } else if (linha.contains("This archive:")) {
      QStringList tamanhos =
        linha.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
      if (tamanhos.size() >= 4) {
        originalSize = parseSize(tamanhos[0] + " " + tamanhos[1]);
        compressedSize = parseSize(tamanhos[2] + " " + tamanhos[3]);
        dedupSize = parseSize(tamanhos[4] + " " + tamanhos[5]);
      }
    } else if (linha.startsWith("Chunk index:")) {
      QStringList chunks = linha.split(':');
      if (chunks.size() >= 2) {
        QStringList valores =
          chunks[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (valores.size() >= 2) {
          uniqueChunks = valores[0].toInt();
          totalChunks = valores[1].toInt();
        }
      }
    }
  }

  // Main header
  html += QString("<div class='info-box'>");
  html += QString("<h2>BorgBackup - Informações do Arquivo</h2>");
  html += QString("<p><span class='highlight'>%1</span></p>").arg(archiveName);
  html += QString("</div>");

  // Basic informations
  html += "<div class='section'>";
  html += "<h3>ℹ️ Informações Básicas</h3>";
  html += QString("<p><strong>Hostname:</strong> %1</p>").arg(hostname);
  html += QString("<p><strong>Usuário:</strong> %1</p>").arg(username);
  html += QString("<p><strong>Fingerprint:</strong> <code>%1</code></p>")
            .arg(fingerprint.left(16) + "...");
  html += QString("<p><strong>Início:</strong> %1</p>")
            .arg(startTime.toString("dd/MM/yyyy hh:mm:ss"));
  html += QString("<p><strong>Fim:</strong> %1</p>")
            .arg(endTime.toString("dd/MM/yyyy hh:mm:ss"));
  html += QString("<p><strong>Duração:</strong> %1</p>")
            .arg(formatDuration(duration));
  html += QString("<p><strong>Arquivos:</strong> %1</p>").arg(numFiles);
  html += "</div>";

  // Length statistics
  html += "<div class='section'>";
  html += "<h3> Estatísticas de Compressão</h3>";
  html += QString("<table class='stats-table'>");
  html += "<tr><th></th><th>Original</th><th>Comprimido</th><th>Deduplicado</"
          "th></tr>";
  html += QString("<tr><td><strong>Tamanho</strong></td>"
                  "<td>%1</td><td>%2</td><td>%3</td></tr>")
            .arg(formatSize(originalSize),
                 formatSize(compressedSize),
                 formatSize(dedupSize));
  html += "</table>";
  html += "</div>";

  // Chunks
  html += "<div class='section'>";
  html += "<h3> Deduplicação</h3>";
  html +=
    QString("<p class='chunks'>Chunks únicos: <strong>%1</strong> | "
            "Total de chunks: <strong>%2</strong> | "
            "Eficiência: <strong>%3</strong></p>")
      .arg(uniqueChunks)
      .arg(totalChunks)
      .arg(totalChunks > 0
             ? QString::number(100.0 * uniqueChunks / totalChunks, 'f', 1) + "%"
             : "N/A");
  html += "</div>";

  html += "</body></html>";

  return html;
}

/*!
 * \brief Globals::parseSize
 * \param sizeStr
 * \return
 */
double
Globals::parseSize(const QString& sizeStr)
{
  static QRegularExpression re("([0-9.]+)\\s*([KMGT]B)");
  QRegularExpressionMatch match = re.match(sizeStr.toUpper());
  if (!match.hasMatch()) {
    return 0;
  }

  double valor = match.captured(1).toDouble();
  QString unidade = match.captured(2);

  if (unidade == "KB") {
    return valor * 1024;
  }
  if (unidade == "MB") {
    return valor * 1024 * 1024;
  }
  if (unidade == "GB") {
    return valor * 1024 * 1024 * 1024;
  }
  if (unidade == "TB") {
    return valor * 1024LL * 1024 * 1024 * 1024;
  }

  return valor;
}

/*!
 * \brief Globals::formatSize
 * \param bytes
 * \return
 */
QString
Globals::formatSize(double bytes)
{
  if (bytes < 1024) {
    return QString::number(bytes, 'f', 1) + " B";
  }
  if (bytes < 1024 * 1024) {
    return QString::number(bytes / 1024, 'f', 1) + " KiB";
  }
  if (bytes < 1024 * 1024 * 1024) {
    return QString::number(bytes / (1024 * 1024), 'f', 2) + " MiB";
  }
  return QString::number(bytes / (1024 * 1024 * 1024), 'f', 2) + " GiB";
}

/*!
 * \brief Globals::formatDuration
 * \param segundos
 * \return
 */
QString
Globals::formatDuration(qint64 segundos)
{
  if (segundos < 60) {
    return QString::number(segundos) + "s";
  }
  return QString("%1m %2s").arg(segundos / 60).arg(segundos % 60);
}

/*!
 * \brief Globals::findRepoAndArchive
 * \param dataList_
 * \return
 */
Globals::RepoArchiveInfo
Globals::findRepoAndArchive(const AppTypes::NamedValueList& dataList_) const
{
  QHash<QString, QString> repoMap_;
  QHash<QString, QString> archiveMap_;
  QString destRepo_;

  static const QRegularExpression re_repo_(R"(^B_REPONAME_(\d+)$)");
  static const QRegularExpression re_arch_(R"(^B_ARCHIVE_(\d+)$)");
  static const QRegularExpression re_destrepo_(R"(^A_DESTREPO$)");

  for (const auto& pair_ : dataList_) {
    const QString& key_ = pair_.first.trimmed();
    const QString value_ = pair_.second.trimmed();

    if (value_.isEmpty()) {
      continue;
    }

    if (auto match_ = re_repo_.match(key_); match_.hasMatch()) {
      const QString id_ = match_.captured(1);
      repoMap_[id_] = value_;
    } else if (auto match_ = re_arch_.match(key_); match_.hasMatch()) {
      const QString id_ = match_.captured(1);
      archiveMap_[id_] = value_;
    } else if (re_destrepo_.match(key_).hasMatch()) {
      destRepo_ = value_;
    }
  }

  AppTypes::NamedValueList result_;
  for (QString id_ : repoMap_.keys()) {
    if (archiveMap_.contains(id_)) {
      result_.append(qMakePair(repoMap_[id_], archiveMap_[id_]));
    }
  }
  return { destRepo_, result_ };
}

/*! DiskInfo ============================================================== */

// Converte bytes para outra unidade, retornando um valor em ponto flutuante.
// bytes: valor em bytes.
// unit: unidade desejada (Bytes, KB, MB, GB).
// Retorna o valor convertido (arredondado se for inteiro).
[[nodiscard]] static double
convertBytesToUnit(std::int64_t bytes,
                   Globals::DiskInfoFacility::Unit unit) noexcept
{
  constexpr double kKb = 1024.0;
  constexpr double kMb = kKb * 1024.0;
  constexpr double kGb = kMb * 1024.0;
  constexpr double kTb = kGb * 1024.0;
  constexpr double kPb = kTb * 1024.0;

  switch (unit) {
    case Globals::DiskInfoFacility::Unit::Bytes:
      return static_cast<double>(bytes);
    case Globals::DiskInfoFacility::Unit::KB:
      return bytes / kKb;
    case Globals::DiskInfoFacility::Unit::MB:
      return bytes / kMb;
    case Globals::DiskInfoFacility::Unit::GB:
      return bytes / kGb;
    case Globals::DiskInfoFacility::Unit::TB:
      return bytes / kTb;
    case Globals::DiskInfoFacility::Unit::PB:
      return bytes / kPb;
    default:
      return static_cast<double>(bytes);
  }
}

// Converte bytes para a unidade sempre como inteiro (arredondado).
// bytes: valor em bytes.
// unit: unidade de saída.
// retorna nullopt se bytes < 0; caso contrário, retorna o valor inteiro
// arredondado.
[[nodiscard]] static std::optional<std::int64_t>
bytesToUnit(std::int64_t bytes, Globals::DiskInfoFacility::Unit unit) noexcept
{
  if (bytes < 0)
    return std::nullopt;

  const double d = convertBytesToUnit(bytes, unit);
  return std::make_optional(static_cast<std::int64_t>(std::round(d)));
}

// Construtor.
// path: qualquer arquivo ou diretório dentro do sistema de arquivos que você
// quer medir. O std::filesystem::space vai usar o path para descobrir o fs em
// que ele está montado.
Globals::DiskInfoFacility::DiskInfoFacility(const std::filesystem::path& path)
  : m_path(path)
{
}

// Obtém o espaço total do sistema de arquivos (capacity) em bytes e converte
// para a unidade. Usa std::filesystem::space, que por baixo chama statvfs e
// preenche:
//   capacity = f_blocks * f_frsize
// Em caso de erro (por exemplo, caminho inválido ou fs não acessível),
// std::filesystem::space pode lançar std::filesystem::filesystem_error ou
// retornar (-1) em algum campo; nesse caso, retorna nullopt.
[[nodiscard]] std::optional<std::int64_t>
Globals::DiskInfoFacility::DiskInfoFacility::totalSpace(
  Globals::DiskInfoFacility::Unit unit) const
{
  std::error_code ec{};
  const auto si = std::filesystem::space(m_path, ec);

  if (ec || si.capacity == static_cast<std::uintmax_t>(-1))
    return std::nullopt;

  const std::int64_t totalBytes = static_cast<std::int64_t>(si.capacity);
  return bytesToUnit(totalBytes, unit);
}

/*!
 * \brief Gets the free space available to the user (available) in bytes.
 * available reflects the space that non-privileged processes can use
 * similar to f_bavail in statvfs).
 * If the call fails or available is -1, returns nullopt.
 */
[[nodiscard]] std::optional<std::int64_t>
Globals::DiskInfoFacility::DiskInfoFacility::freeSpace(
  Globals::DiskInfoFacility::DiskInfoFacility::Unit unit) const
{
  std::error_code ec{};
  const auto si = std::filesystem::space(m_path, ec);

  if (ec || si.available == static_cast<std::uintmax_t>(-1))
    return std::nullopt;

  const std::int64_t freeBytes = static_cast<std::int64_t>(si.available);
  return bytesToUnit(freeBytes, unit);
}

/*!
 * \brief Calculates the space used as total - free, converted to the unit.
 * As in Linux/df, it uses:
 * used = total - free
 * If total or free cannot be obtained, or if free > total (impossible
 * numerically, but we protect it here), returns nullopt.
 */
[[nodiscard]] std::optional<std::int64_t>
Globals::DiskInfoFacility::DiskInfoFacility::usedSpace(
  Globals::DiskInfoFacility::DiskInfoFacility::Unit unit) const
{
  const auto t = totalSpace(Unit::Bytes);
  const auto f = freeSpace(Unit::Bytes);

  if (!t || !f || *f > *t)
    return std::nullopt;

  const std::int64_t usedBytes = *t - *f;
  return bytesToUnit(usedBytes, unit);
}
