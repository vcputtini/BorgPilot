/***************************************************************************
 * Copyright (c) 2023-2024                                                 *
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
#ifndef GLOBALS_H
#define GLOBALS_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QPair>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "progid.h"

#include <filesystem>
#include <utility> // std::as_const()

namespace fs_ = std::filesystem;

/*!
 * \brief The Globals class
 */
class Globals
{
public:
  explicit Globals();

  enum class Modules
  {
    ScriptGen = 0x00,
    FormLists,
    FormUtils,
    Settings,
    ExternalTools,
    None = 0xffff
  };

  /**
   * \brief Searches for a value in a list of key-value pairs and returns the
   * value associated with the given key.
   *
   * This function iterates through a list of pairs (`QPair`) and returns the
   * value associated with the key (`key`). If the given key is found, the
   * corresponding value is returned. Otherwise, `nullptr` is returned,
   * indicating that the key was not found in the list.
   *
   * \param KeyT Type of the key, which will be compared with the given key.
   * \param ValueT Type of the value associated with each key in the list.
   *
   * \param list The list of pairs (`QPair<KeyT, ValueT>`) to search for.
   * \param key The key to search for in the list of pairs.
   *
   * \return The value associated with the key `key` if found, otherwise
   * `nullptr` is returned. *
   *
   * \note This function makes use of `std::as_const` to ensure that the list is
   * not modified during the search. The function returns `nullptr` in case the
   * key is not found.
   */
  template<typename KeyT, typename ValueT>
  static ValueT find_(const QList<QPair<KeyT, ValueT>>& list, const KeyT& key)
  {
    for (const auto& pair_ : std::as_const(list)) {
      if (pair_.first == key) {
        return pair_.second;
      }
    }
    return nullptr;
  }

  /**
   * \brief Searches for a value associated with a key in a setting, using
   * QSettings.
   *
   * This function searches for the value of a key within a group of
   * settings dynamically generated from the script name. The search is
   * performed using the QSettings class, which accesses settings
   * stored on disk.
   *
   * \param key Key to search for the associated value.
   * \param script_name Name of the script used to form the group of
   * settings.
   *
   * \return The value associated with the key in the group of
   * settings, or an empty string if not found.
   */
  template<typename KeyT = QString, typename T = QString>
  static T findOne_(const KeyT& key, const QString& script_name)
  {
    QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
    QString groupname = QString("INITREPO_%0").arg(script_name);
    return settings.value(groupname + "/" + key).toString();
  }

  /*!
   * \brief The function checks whether a file already exists in the specified
   * location. If the file exists, it suggests a new name, adding a number in
   * parentheses to ensure that the name is unique
   * \param sourcePath
   * \return
   */
  [[nodiscard]] static const QString suggestNewFilename(
    const QString& sourcePath)
  {
    QFileInfo fileInfo(sourcePath);
    const QString directory_ = fileInfo.absolutePath();
    const QString baseName_ = fileInfo.baseName();
    const QString extension_ = fileInfo.suffix();

    fs_::path pathDir_(directory_.toStdString());
    fs_::path basePath_(baseName_.toStdString());

    int index_{ 1 };
    fs_::path newPath_ =
      pathDir_ / (basePath_.string() + "." + extension_.toStdString());

    while (fs_::exists(newPath_)) {
      newPath_ =
        pathDir_ / (basePath_.string() + "_" + std::to_string(index_++) + "." +
                    extension_.toStdString());
    }

    return QString::fromStdString(newPath_.string());
  }

  /*!
   * \brief This function returns de [new]filename
   * \param sourcePath
   * \return
   */
  static const QString fileName(const QString& sourcePath)
  {
    QFileInfo fileInfo(sourcePath);
    const QString baseName_ = fileInfo.baseName();
    const QString extension_ = fileInfo.suffix();
    return baseName_ + "." + extension_;
  }
};

#endif // GLOBALS_H
