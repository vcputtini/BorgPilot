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
#ifndef BASHHIGHLIGHTERLITE_H
#define BASHHIGHLIGHTERLITE_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>

/*!
 * \brief Implements syntax highlighting for Bash scripts in Qt 6.
 * * This class extends QSyntaxHighlighter to provide parsing and highlighting
 * of common Bash elements such as keywords, variables, embedded commands, and
 * comments, supporting light and dark themes.
 */
class BashHighlighterLite : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  /**
   * \enum Theme
   * \brief Defines the available color schemes for highlighting.
   */
  enum class Theme
  {
    Light,
    Dark
  };

  /*!
   * \brief Class constructor.
   * \param parent The text document that will receive the highlighting.
   * \param theme The initial theme (default: Theme::Dark).
   */
  explicit BashHighlighterLite(QTextDocument* parent = nullptr,
                               Theme theme = Theme::Dark);
  ~BashHighlighterLite() = default;

  /*!
   * \brief Changes the color theme at runtime.
   * \param theme The new theme to be applied.
   */
  void setTheme(Theme theme);

protected:
  /*!
   * \brief Analyzes and formats a block of text.
   * \param text The content of the current line to be processed.
   * This method is automatically called by the Qt framework when the
   * text document is changed or rendered.
   */
  void highlightBlock(const QString& text) override;

private:
  /*!
   * \struct HighlightingRule
   * \brief Structure for mapping regex patterns to character formats.
   */
  struct HighlightingRule
  {
    QRegularExpression pattern; ///< Regular expression to identify the element.
    QTextCharFormat format;     ///< Visual formatting (color, bold, etc.).
  };
  QVector<HighlightingRule>
    m_highlightingRules_; ///< Stores the current state of the theme.

  QTextCharFormat m_keywordFormat_; ///< Keyword format (if, for, while).
  QTextCharFormat
    m_builtinFormat_; ///< Format for internal commands (echo, cd).
  QTextCharFormat m_variableFormat_; ///< Format for variables ($VAR, ${VAR}).
  QTextCharFormat
    m_singleQuoteFormat_; ///< Format for strings in single quotes.
  QTextCharFormat
    m_doubleQuoteFormat_;           ///< Format for strings in double quotes.
  QTextCharFormat m_commentFormat_; ///< Format for comments (#).

  Theme m_currentTheme_;

  /*!
   * \brief Initializes the basic format properties (bold, italic).
   */
  void setupFormats();

  /*!
   * \brief Defines and compiles the regular expression rules.
   */
  void setupRules();

  /*!
   * \brief Updates the format colors based on the selected theme.
   */
  void applyThemeColors();
};

#endif // BASHHIGHLIGHTERLITE_H