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
#ifndef SQLCONFIGWRAPPER_H
#define SQLCONFIGWRAPPER_H

#include <QDebug>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

/*!
 * \warning EXPERIMENTAL SUPPORT FOR CRUD OPERATIONS
 *          IN THE CONFIGURATION FILES.
 *
 *          IMPORTANT: NOT YET IMPLEMENTED
 */

class SQLConfigWrapper : public QObject
{
  Q_OBJECT
public:
  // explicit SQLConfigWrapper(QObject* parent = nullptr);
  explicit SQLConfigWrapper(const QString& dbPath = nullptr,
                            QObject* parent = nullptr);
  ~SQLConfigWrapper();

  [[nodiscard]] bool openDatabase();
  void closeDatabase();

  // CRUD
  [[nodiscard]] bool create(const QString& table, const QString& sqlInsert);
  [[nodiscard]] bool read(const QString& table,
                          const QString& sqlSelect,
                          QList<QVariantMap>& results);
  [[nodiscard]] bool update(const QString& table, const QString& sqlUpdate);
  [[nodiscard]] bool remove(const QString& table, const QString& sqlDelete);

  // Default function for creating a table.
  [[nodiscard]] bool createDefaultTable(const QString& tableName);
  [[nodiscard]] bool populateDefaultTable(const QString& tableName);

  [[nodiscard]] QString lastError() const { return m_lastError; }
  [[nodiscard]] bool isOpen() const { return m_db.isOpen(); }

private:
  QSqlDatabase m_db;
  QString m_dbPath;
  QString m_lastError;

  bool executeQuery(const QString& sql);
  bool executeQuery(QSqlQuery& query);

protected:
  const QString m_defaultPath;

  /*! \attention DO NOT CHANGE THIS VARIABLE! THIS WILL NEVER BE NECESSARY. */
  const QString m_SQLNameConnection = "DBCONFIGWRAPPERCONN";
};

#endif // SQLCONFIGWRAPPER_H
