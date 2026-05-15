/***************************************************************************
 * Copyright (c) 2025-2026                                                *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
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
#include "combobox_scriptcreatename_p.h"

#include <QSettings>
#include <QStringList>

#include "progid.h"

comboBox_ScriptCreateName_P::comboBox_ScriptCreateName_P(QWidget* parent)
  : QComboBox(parent)
{
  populateScriptNames();
}

void
comboBox_ScriptCreateName_P::refresh()
{
  populateScriptNames();
}

void
comboBox_ScriptCreateName_P::populateScriptNames()
{
  blockSignals(true);

  clear();

  const QSettings settings(ProgId::strOrganization(),
                           ProgId::strInternalName());
  QStringList groups = settings.childGroups();

  // C++20: Filters using ranges for more efficient
  auto scriptNames = groups | std::views::filter([](const QString& group) {
                       return group.contains("INITREPO_");
                     }) |
                     std::views::transform([](const QString& group) {
                       return group.section("INITREPO_", 1);
                     });

  addItem(""); // First item is void.
  for (const auto& name : scriptNames) {
    addItem(name);
  }
  blockSignals(false);
  update();
}
