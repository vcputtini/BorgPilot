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
#ifndef TREEWIDGETREPOSITORY_P_H
#define TREEWIDGETREPOSITORY_P_H

#include <QButtonGroup>
#include <QObject>
#include <QRadioButton>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

/*!
 * \brief QTreeWidget de três colunas para repositórios Borg.
 *
 * Layout visual:
 * \code
 *  REPOSITORY      | ARCHIVE
 *  ────────────────────────────────────
 *  ▶ BKP01         |
 *      ·           | ◉ A01
 *      ·           | ○ A02
 *      ·           | ○ A{n}
 * \endcode
 *
 * Uso típico:
 * \code
 *  tree->addRepository("/mnt/repo1");
 *  tree->addArchive("2025-01-15T00:00:00");
 *  tree->addArchive("2025-01-16T00:00:00");
 *
 *  tree->addRepository("user\@host:/repo2", "{hostname}-{now}");
 *  tree->addArchive("backup-001");
 * \endcode
 */
class TreeWidgetRepository_P : public QTreeWidget
{
  Q_OBJECT

public:
  // Column indexes - I don't want magic numbers here
  enum class Column : int
  {
    Repository = 0,
    Archive = 1,
    /* Filters = 2, */
    COUNT = 2
  };

  explicit TreeWidgetRepository_P(QWidget* parent = nullptr);
  ~TreeWidgetRepository_P() = default;

  /*!
   * \brief Adds a repository as a root (top-level) item.
   * \param repoPath Repository path (local or ssh user @host:/path).
   * \note After this call, subsequent calls to \c addArchive() append
   * files to **this** repository, until \c addRepository() is
   * called again.
   */
  void addRepository(const QString& repoPath);

  /*!
   * \brief Adds a file to the current repository.
   * \param archiveName Borg filename (e.g., \c 2025-01-15T00:00:00).
   * \pre \c addRepository() must have been called at least once.
   * If not, a \c "Default" repository is created automatically.
   */
  void addArchive(const QString& archiveName);

  /*!
   * \brief Removes all items and resets the internal state.
   */
  void clearTree();

signals:
  /*!
   * \brief Issued when the user selects a file via QRadioButton.
   *
   * The format of the issued brief is:
   * - Local repository: \c "/path/repo::archive --format=\"{NL}\""
   * - SSH repository: \c "ssh://user\@host:/repo::archive
   * --format=\"{NL}\""
   */
  void sig_setPattern(const QString& pattern);

private:
  // status
  QTreeWidgetItem* currentRoot_ = nullptr; ///< Active root for addArchive()
  QButtonGroup* buttonGroup_ = nullptr;    ///< Global radio exclusivity

  // helpers

  /*!
   * \brief Builds and returns the correct Borg pattern for (repoPath,
   * archiveName).
   */
  [[nodiscard]] static QString buildPattern(const QString& repoPath,
                                            const QString& archiveName);

  /*!
   * \briefInitializes headers and appearance — called only by the constructor.
   */
  void setupHeader();
};

#endif // TREEWIDGETREPOSITORY_P_H
