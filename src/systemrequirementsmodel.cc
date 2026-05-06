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
#include "systemrequirementsmodel.h"

#include <QBrush>
#include <QColor>
#include <QProcess>
#include <QStandardPaths>

SystemRequirementsModel::SystemRequirementsModel(QObject* parent)
  : QAbstractTableModel(parent)
{
}

int
SystemRequirementsModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    return 0;
  }
  return static_cast<int>(m_data.size());
}

int
SystemRequirementsModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    return 0;
  }
  return 4; // Path, Name, Version, Status
}

QVariant
SystemRequirementsModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() >= static_cast<int>(m_data.size())) {
    return {};
  }

  const auto& item = m_data[index.row()];

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return item.path.isEmpty() ? QStringLiteral("N/A") : item.path;
      case 1:
        return item.name;
      case 2:
        return item.version.isEmpty() ? QStringLiteral("N/A") : item.version;
      case 3:
        return item.isInstalled ? tr("Installed") : tr("Unavailable");
      default:
        return {};
    }
  }
  // Automatic visual feedback based on status.
  else if (role == Qt::ForegroundRole && index.column() == 3) {
    return item.isInstalled ? QBrush(Qt::darkGreen) : QBrush(Qt::darkRed);
  }

  return {};
}

QVariant
SystemRequirementsModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return tr("Program Path");
      case 1:
        return tr("Program Name");
      case 2:
        return tr("Version");
      case 3:
        return tr("Status");
      default:
        return {};
    }
  }
  return {};
}

void
SystemRequirementsModel::checkPrograms(const QStringList& essentialPrograms)
{
  // Explanation: beginResetModel notifies the QTableView to prepare for new
  // data.
  beginResetModel();
  m_data.clear();
  m_data.reserve(
    essentialPrograms.size()); // Avoids dynamic vector relocations.

  for (const auto& prog : essentialPrograms) {
    m_data.push_back(resolveProgramInfo(prog));
  }
  endResetModel();
}

ProgramData
SystemRequirementsModel::resolveProgramInfo(const QString& progName) const
{
  ProgramData info;
  info.name = progName;

  // Performs a secure scan of the system PATH.
  info.path = QStandardPaths::findExecutable(progName);
  info.isInstalled = !info.path.isEmpty();

  if (info.isInstalled) {
    QProcess process;
    // Almost all binaries use --version. Adjust for exceptions.
    if (info.name == "ssh") {
      process.start(info.path, { QStringLiteral("-V") });
    } else {
      process.start(info.path, { QStringLiteral("--version") });
    }

    // Critical timeout: 1 second to prevent the Main Thread (GUI) from
    // freezing.
    if (process.waitForFinished(1000)) {
      QString output =
        QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
      if (output.isEmpty()) {
        output =
          QString::fromLocal8Bit(process.readAllStandardError()).trimmed();
      }
      // Isolate only the first line of the return.
      info.version = output.section(QLatin1Char('\n'), 0, 0);
    } else {
      process.kill();
      info.version = tr("Timeout/Unavailable");
    }
  }
  return info;
}
