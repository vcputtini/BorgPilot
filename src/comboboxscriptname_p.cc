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
#include "comboboxscriptname_p.h"

#include "progid.h"
#include <QSettings>
#include <QString>

// #include <algorithm>
// #include <ranges> // C++20 para views

#ifdef NOAUTOREFRESH
/*!
 * \brief ComboBoxScriptName_P::ComboBoxScriptName_P
 * \param parent
 */
ComboBoxScriptName_P::ComboBoxScriptName_P(QWidget* parent)
  : QComboBox(parent)
{
  getScriptNames();
}

/*!
 * \brief ComboBoxScriptName_P::getScriptNames
 */
void
ComboBoxScriptName_P::getScriptNames()
{
  const QSettings settings_(ProgId::strOrganization(),
                            ProgId::strInternalName());
  QStringList groups_ = settings_.childGroups();
  addItem("");
  for (const QString& group_ : std::as_const(groups_)) {
    if (group_.contains("INITREPO_")) {
      addItem(group_.section("INITREPO_", 1));
    }
  }
}

#else

// auto refresh
ComboBoxScriptName_P::ComboBoxScriptName_P(QWidget* parent)
  : QComboBox(parent)
{
  qDebug() << "USING AUTOREFRESH";

  populateScriptNames();

  // Watcher para mudanças no arquivo INI
  connect(&m_watcher,
          &QFileSystemWatcher::fileChanged,
          this,
          &ComboBoxScriptName_P::refreshScriptNames);

  // Timer para polling a cada 5s (fallback se watcher falhar)
  m_timer.setInterval(5000);
  connect(&m_timer,
          &QTimer::timeout,
          this,
          &ComboBoxScriptName_P::refreshScriptNames);
  m_timer.start();

  // Adiciona o path do settings ao watcher
  QString settingsPath = getSettingsFilePath();
  if (QFileInfo::exists(settingsPath)) {
    m_watcher.addPath(settingsPath);
  }
}

void
ComboBoxScriptName_P::refreshScriptNames()
{
  QString settingsPath = getSettingsFilePath();
  if (!QFileInfo::exists(settingsPath)) {
    clear();
    return;
  }

  const int actual_index_ = currentIndex();
  // Re-adiciona se necessário (ex: renomeado)
  if (!m_watcher.files().contains(settingsPath)) {
    m_watcher.addPath(settingsPath);
    // populateScriptNames();
  }

  populateScriptNames();
  setCurrentIndex(actual_index_);
}

void
ComboBoxScriptName_P::populateScriptNames()
{
  blockSignals(true);

  clear(); // Clear list

  const QSettings settings(ProgId::strOrganization(),
                           ProgId::strInternalName());
  QStringList groups = settings.childGroups();

  // C++20: Filters using ranges (more efficient than manual loops)
  auto scriptNames = groups | std::views::filter([](const QString& group) {
                       return group.contains("INITREPO_");
                     }) |
                     std::views::transform([](const QString& group) {
                       return group.section("INITREPO_", 1);
                     });

  for (const auto& name : scriptNames) {
    addItem(name);
  }
  blockSignals(false);
  update();
}

QString
ComboBoxScriptName_P::getSettingsFilePath() const
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  qDebug() << settings.fileName();
  return settings.fileName();
}

#endif //
