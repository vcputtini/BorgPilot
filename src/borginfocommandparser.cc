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
#include "borginfocommandparser.h"

#include <QJsonParseError>

BorgInfoCommandParser::BorgInfoCommandParser(QJsonObject root) noexcept
  : m_root(std::move(root))
  , m_archives(m_root[QStringLiteral("archives")].toArray())
{
}

std::optional<BorgInfoCommandParser>
BorgInfoCommandParser::fromJson(const QString& json) noexcept
{
  QJsonParseError err{};
  const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);
  if (err.error != QJsonParseError::NoError) {
    return std::nullopt;
  }
  return fromDocument(doc);
}

std::optional<BorgInfoCommandParser>
BorgInfoCommandParser::fromDocument(const QJsonDocument& doc) noexcept
{
  if (!doc.isObject()) {
    return std::nullopt;
  }
  return BorgInfoCommandParser{ doc.object() };
}

// Helper: safe navigation on the road
QJsonValue
BorgInfoCommandParser::deepValue(
  const QJsonObject& obj,
  std::initializer_list<QString> path) const noexcept
{
  QJsonValue current = obj;
  for (const auto& key : path) {
    if (!current.isObject()) {
      return QJsonValue::Undefined;
    }
    current = current.toObject().value(key);
  }

  return current;
}

// Helper: archive by index
QJsonObject
BorgInfoCommandParser::archiveObj(int idx) const noexcept
{
  if (idx < 0 || idx >= m_archives.size()) {
    return {};
  }
  return m_archives.at(idx).toObject();
}

// Archives
int
BorgInfoCommandParser::archiveCount() const noexcept
{
  return static_cast<int>(m_archives.size());
}

QJsonObject
BorgInfoCommandParser::archive(int idx) const noexcept
{
  return archiveObj(idx);
}

QString
BorgInfoCommandParser::archiveName(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("name")).toString();
}

QString
BorgInfoCommandParser::archiveId(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("id")).toString();
}

QString
BorgInfoCommandParser::archiveHostname(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("hostname")).toString();
}

QString
BorgInfoCommandParser::archiveUsername(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("username")).toString();
}

QString
BorgInfoCommandParser::archiveStart(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("start")).toString();
}

QString
BorgInfoCommandParser::archiveEnd(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("end")).toString();
}

double
BorgInfoCommandParser::archiveDuration(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("duration")).toDouble();
}

QString
BorgInfoCommandParser::archiveComment(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("comment")).toString();
}

QStringList
BorgInfoCommandParser::archiveCommandLine(int idx) const noexcept
{
  QStringList result;
  const auto arr =
    archiveObj(idx).value(QStringLiteral("command_line")).toArray();
  result.reserve(static_cast<qsizetype>(arr.size()));
  for (const auto& v : arr)
    result << v.toString();
  return result;
}

QVariantList
BorgInfoCommandParser::archiveChunkerParams(int idx) const noexcept
{
  return archiveObj(idx)
    .value(QStringLiteral("chunker_params"))
    .toArray()
    .toVariantList();
}

// Archive: stats
QJsonObject
BorgInfoCommandParser::archiveStats(int idx) const noexcept
{
  return archiveObj(idx).value(QStringLiteral("stats")).toObject();
}

qint64
BorgInfoCommandParser::archiveCompressedSize(int idx) const noexcept
{
  return static_cast<qint64>(
    archiveStats(idx).value(QStringLiteral("compressed_size")).toDouble());
}

qint64
BorgInfoCommandParser::archiveDedupSize(int idx) const noexcept
{
  return static_cast<qint64>(
    archiveStats(idx).value(QStringLiteral("deduplicated_size")).toDouble());
}

qint64
BorgInfoCommandParser::archiveOriginalSize(int idx) const noexcept
{
  return static_cast<qint64>(
    archiveStats(idx).value(QStringLiteral("original_size")).toDouble());
}

qint64
BorgInfoCommandParser::archiveFileCount(int idx) const noexcept
{
  return static_cast<qint64>(
    archiveStats(idx).value(QStringLiteral("nfiles")).toDouble());
}

// Archive: limits
double
BorgInfoCommandParser::archiveMaxSize(int idx) const noexcept
{
  return deepValue(
           archiveObj(idx),
           { QStringLiteral("limits"), QStringLiteral("max_archive_size") })
    .toDouble();
}

// Cache
QString
BorgInfoCommandParser::cachePath() const noexcept
{
  return deepValue(m_root, { QStringLiteral("cache"), QStringLiteral("path") })
    .toString();
}

qint64
BorgInfoCommandParser::cacheTotalChunks() const noexcept
{
  return static_cast<qint64>(deepValue(m_root,
                                       { QStringLiteral("cache"),
                                         QStringLiteral("stats"),
                                         QStringLiteral("total_chunks") })
                               .toDouble());
}

qint64
BorgInfoCommandParser::cacheUniqueChunks() const noexcept
{
  return static_cast<qint64>(
    deepValue(m_root,
              { QStringLiteral("cache"),
                QStringLiteral("stats"),
                QStringLiteral("total_unique_chunks") })
      .toDouble());
}

qint64
BorgInfoCommandParser::cacheTotalCSize() const noexcept
{
  return static_cast<qint64>(deepValue(m_root,
                                       { QStringLiteral("cache"),
                                         QStringLiteral("stats"),
                                         QStringLiteral("total_csize") })
                               .toDouble());
}

qint64
BorgInfoCommandParser::cacheTotalSize() const noexcept
{
  return static_cast<qint64>(deepValue(m_root,
                                       { QStringLiteral("cache"),
                                         QStringLiteral("stats"),
                                         QStringLiteral("total_size") })
                               .toDouble());
}

qint64
BorgInfoCommandParser::cacheUniqueCSize() const noexcept
{
  return static_cast<qint64>(deepValue(m_root,
                                       { QStringLiteral("cache"),
                                         QStringLiteral("stats"),
                                         QStringLiteral("unique_csize") })
                               .toDouble());
}

qint64
BorgInfoCommandParser::cacheUniqueSize() const noexcept
{
  return static_cast<qint64>(deepValue(m_root,
                                       { QStringLiteral("cache"),
                                         QStringLiteral("stats"),
                                         QStringLiteral("unique_size") })
                               .toDouble());
}

// Encryption
QString
BorgInfoCommandParser::encryptionMode() const noexcept
{
  return deepValue(m_root,
                   { QStringLiteral("encryption"), QStringLiteral("mode") })
    .toString();
}

// Repository
QString
BorgInfoCommandParser::repoId() const noexcept
{
  return deepValue(m_root,
                   { QStringLiteral("repository"), QStringLiteral("id") })
    .toString();
}

QString
BorgInfoCommandParser::repoLastModified() const noexcept
{
  return deepValue(
           m_root,
           { QStringLiteral("repository"), QStringLiteral("last_modified") })
    .toString();
}

QString
BorgInfoCommandParser::repoLocation() const noexcept
{
  return deepValue(m_root,
                   { QStringLiteral("repository"), QStringLiteral("location") })
    .toString();
}
