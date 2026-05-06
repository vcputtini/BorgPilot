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
#ifndef GLOBAL_TOOLS_H
#define GLOBAL_TOOLS_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QPair>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextBrowser>
#include <QTextEdit>

#include "progid.h"
#include "types_global.h"

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <utility> // std::as_const()

namespace fs_ = std::filesystem;

class Globals : public QObject
{
  Q_OBJECT
public:
  explicit Globals(QObject* parent = nullptr);

  enum class Modules
  {
    ScriptGen = 0x00,
    FormLists,
    FormUtils,
    GraphMetrics,
    FormDashboard,
    Settings,
    ExternalTools,
    None = 0xffff
  };

  static constexpr const double BYTES = 1024;
  static constexpr const double MBYTES = BYTES * 1024;
  static constexpr const double GBYTES = MBYTES * 1024;
  static constexpr const double TBYTES = GBYTES * 1024;
  static constexpr const double PBYTES = TBYTES * 1024;

  /*!
   * \brief Displays a generic message box.
   * \param text Sentence or phrase that describes the situation.
   * \param info It contains informative text that provides a more complete
   * description of the message.
   * \return
   */
  static const int displayMessages(const QString& text,
                                   const QString& info) noexcept;

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
   * @brief Prompts the user for a location to save a file with customized
   * validations.
   *
   * This template function allows you to define the directory context and base
   * name generically, ensuring that the correct extension is applied and
   * validating that the user has not canceled the operation.
   *
   * @param parent The parent QWidget for the dialog (usually 'this').
   * @param title The translated title of the dialog window.
   * @param defaultPath The full path or suggested starting directory.
   * @param filter The file filter (e.g., "Scripts (*.sh)").
   * @param suffix The required extension without the dot (e.g., "sh").
   * @return std::optional<QString> The file path if valid, or std::nullopt if
   * canceled/empty.
   *
   * @note Requires C++17 for std::optional.
   *
   * \code
   * // call template
   * auto result = getValidatedSavePath(
   *     this,
   *     tr("Save Script As ..."),
   *     suggestedName,
   *     tr("Script (*.sh)"),
   *     u"sh" // sufixo
   * );
   *
   *  // If the optional value is present, proceed with writing the file.
   * if (result) {
   *     QString finalPath = *result;
   *     // saveFile(finalPath);
   * }
   * \endcode
   */
  // template<typename T>
  auto getValidatedSavePath(QWidget* parent,
                            const QString& title,
                            const QString& defaultPath,
                            const QString& filter,
                            const QString& suffix) -> std::optional<QString>
  {
    // Get the filename using the OS's native dialog.
    QString filePath =
      QFileDialog::getSaveFileName(parent, title, defaultPath, filter);

    // Cancellation validation
    if (filePath.isEmpty()) {
      QMessageBox::warning(parent,
                           QObject::tr("Warning"),
                           QObject::tr("The filename cannot be empty."));
      return std::nullopt;
    }

    // Normalizing the extension
    const QString dotSuffix = u"." + suffix;
    if (!filePath.endsWith(dotSuffix, Qt::CaseInsensitive)) {
      filePath += dotSuffix;
    }

    return filePath;
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

  /*!
   * \brief setFilePerssions
   * \param filePath
   * \param mode
   * \return true | false
   */
  static bool setFilePermissions(const QString& filePath, int mode)
  {
    if (!QFile::exists(filePath)) { // file not found
      return false;
    }

    constexpr std::array<std::pair<int, QFileDevice::Permission>, 9> permsMap_{
      {
        { 0400, QFileDevice::ReadOwner },
        { 0200, QFileDevice::WriteOwner },
        { 0100, QFileDevice::ExeOwner },
        { 0040, QFileDevice::ReadGroup },
        { 0020, QFileDevice::WriteGroup },
        { 0010, QFileDevice::ExeGroup },
        { 0004, QFileDevice::ReadOther },
        { 0002, QFileDevice::WriteOther },
        { 0001, QFileDevice::ExeOther },
      }
    };

    QFile::Permissions perms_;
    for (const auto& [bit_, perm_] : permsMap_) {
      if (mode & bit_)
        perms_ |= perm_;
    }

    QFile file(filePath);
    if (!file.setPermissions(perms_)) {
      return false;
    }

    return true;
  };

  /*!
   * \brief isBashScript
   * \param fName
   * \return
   */
  bool isBashScript(const QString& fName)
  {
    QFile file_(fName);
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return false;
    }
    QTextStream in_(&file_);
    in_.setAutoDetectUnicode(true);
    QString textId_ = in_.readLine(30).trimmed();
    in_.flush();
    file_.close();
    qDebug() << textId_;
    return (textId_ == "#!/usr/bin/env bash" ? true : false);
  }

  // Formata o texto do comando: borg info ....
  // Ainda precisa de revisoes
  void formatBorgInfo(const QString& outputRaw_, QTextBrowser* textBrowser_);
  QString genHtmlBorgInfo(const QString& output_);
  double parseSize(const QString& sizeStr);
  QString formatSize(double bytes);
  QString formatDuration(qint64 segundos);

  /*!
   * \brief The RepoArchiveInfo class
   */
  struct RepoArchiveInfo
  {
    QString destRepo;                         // A_DESTREPO VALUE'S
    AppTypes::NamedValueList repoAndArchives; // pairs (REPONAME, ARCHIVE)
  };
  [[nodiscard]] RepoArchiveInfo findRepoAndArchive(
    const AppTypes::NamedValueList& dataList) const;

  // DiskInfo facilty ========================================================

  // Classe que encapsula leitura de espaço em disco via std::filesystem::space.
  // Tanto mais precisa quanto o sistema de arquivos respeitar o padrão POSIX
  // (statvfs).
  class DiskInfoFacility
  {
  public:
    // Unidade de medida para espaço em disco.
    enum class Unit
    {
      Bytes,
      KB,
      MB,
      GB,
      TB,
      PB
    };

    // Constrói um objeto que olha para o sistema de arquivos onde o caminho
    // está montado.
    explicit DiskInfoFacility(
      const std::filesystem::path& path = std::filesystem::current_path());

    // Retorna o tamanho total do sistema de arquivos (capacity) convertido para
    // a unidade. Se o caminho não apontar para um fs válido, ou houver erro,
    // retorna nullopt.
    [[nodiscard]] std::optional<std::int64_t> totalSpace(
      Unit unit = Unit::Bytes) const;

    // Retorna espaço livre disponível para o usuário (available, similar a
    // f_bavail). Se o caminho não estiver disponível, retorna nullopt.
    [[nodiscard]] std::optional<std::int64_t> freeSpace(
      Unit unit = Unit::Bytes) const;

    // Calcula o espaço usado como total - free, convertido para a unidade.
    // Se total ou free não forem válidos, retorna nullopt.
    [[nodiscard]] std::optional<std::int64_t> usedSpace(
      Unit unit = Unit::Bytes) const;

  private:
    // Caminho usado para descobrir em qual sistema de arquivos estamos.
    std::filesystem::path m_path;
  };
};
#endif // GLOBAL_TOOLS_H
