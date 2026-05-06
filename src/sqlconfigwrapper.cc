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
#include "sqlconfigwrapper.h"

#include "environmentvars.h"

#include <QDir>

SQLConfigWrapper::SQLConfigWrapper(const QString& dbPath, QObject* parent)
  : QObject(parent)
  , m_dbPath(dbPath)
  , m_defaultPath(QDir::homePath() + R"(/.config/VCPLinux/qborgpilot.sqlite)")
{
  m_db = QSqlDatabase::addDatabase("QSQLITE", m_SQLNameConnection);
  m_db.setDatabaseName(dbPath == nullptr ? m_defaultPath : dbPath);
}

SQLConfigWrapper::~SQLConfigWrapper()
{
  closeDatabase();
  m_db.removeDatabase(m_SQLNameConnection);
}

bool
SQLConfigWrapper::openDatabase()
{
  if (!m_db.open()) {
    m_lastError = m_db.lastError().text();
    return false;
  }
  return true;
}

void
SQLConfigWrapper::closeDatabase()
{
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool
SQLConfigWrapper::createDefaultTable(const QString& tableName)
{
  const QString sql = QString(R"(
        CREATE TABLE IF NOT EXISTS %1 (
            ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
            F_SECTION CHAR(100),
            F_KEY CHAR(100),
            F_VALUE TEXT
        )
    )")
                        .arg(tableName);

  return executeQuery(sql);
}

bool
SQLConfigWrapper::populateDefaultTable(const QString& tableName)
{
  const QString sql = QString(R"(
        INSERT INTO %1 (F_SECTION, F_KEY, F_VALUE)
        VALUES (
        "AutoAnswerers","BORG_UNKNOWN_UNENCRYPTED_REPO_ACCESS_IS_OK","0",
        ),
        (
        "AutoAnswerers","BORG_RELOCATED_REPO_ACCESS_IS_OK","1",
        ),
        (
        "AutoAnswerers","BORG_CHECK_I_KNOW_WHAT_I_AM_DOING","1",
        ),
        (
        "AutoAnswerers","BORG_DELETE_I_KNOW_WHAT_I_AM_DOING","1",
        );
    )");
  return executeQuery(sql);
}

bool
SQLConfigWrapper::create(const QString& table, const QString& sqlInsert)
{
  QString sql = QString("INSERT INTO %1 %2").arg(table, sqlInsert);
  return executeQuery(sql);
}

bool
SQLConfigWrapper::read(const QString& table,
                       const QString& sqlSelect,
                       QList<QVariantMap>& results)
{
  results.clear();

  QString sql = QString("SELECT * FROM %1 WHERE %2").arg(table, sqlSelect);
  QSqlQuery query(m_db);

  if (!query.exec(sql)) {
    m_lastError = query.lastError().text();
    return false;
  }

  while (query.next()) {
    QVariantMap row;
    for (int i = 0; i < query.record().count(); ++i) {
      row[query.record().fieldName(i)] = query.value(i);
    }
    results.append(row);
  }

  return true;
}

bool
SQLConfigWrapper::update(const QString& table, const QString& sqlUpdate)
{
  QString sql = QString("UPDATE %1 SET %2").arg(table, sqlUpdate);
  return executeQuery(sql);
}

bool
SQLConfigWrapper::remove(const QString& table, const QString& sqlDelete)
{
  QString sql = QString("DELETE FROM %1 WHERE %2").arg(table, sqlDelete);
  return executeQuery(sql);
}

bool
SQLConfigWrapper::executeQuery(const QString& sql)
{
  QSqlQuery query(m_db);
  query.prepare(sql);

  if (!query.exec()) {
    m_lastError = query.lastError().text();
    return false;
  }

  return true;
}

bool
SQLConfigWrapper::executeQuery(QSqlQuery& query)
{
  if (!query.exec()) {
    m_lastError = query.lastError().text();
    return false;
  }
  return true;
}