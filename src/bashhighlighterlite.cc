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
#include "bashhighlighterlite.h"

BashHighlighterLite::BashHighlighterLite(QTextDocument* parent, Theme theme)
  : QSyntaxHighlighter(parent)
  , m_currentTheme_(theme)
{
  setupFormats();
  applyThemeColors(); // Apply initial theme
  setupRules();
}

void
BashHighlighterLite::setupFormats()
{
  m_keywordFormat_.setFontWeight(QFont::Bold);
  m_commentFormat_.setFontItalic(true);
}

void
BashHighlighterLite::applyThemeColors()
{
  if (m_currentTheme_ == Theme::Dark) {
    m_keywordFormat_.setForeground(QColor(QStringLiteral("#0033A0")));
    m_builtinFormat_.setForeground(QColor(QStringLiteral("#007A78")));
    m_variableFormat_.setForeground(QColor(QStringLiteral("#800000")));
    m_singleQuoteFormat_.setForeground(QColor(QStringLiteral("#A31515")));
    m_doubleQuoteFormat_.setForeground(QColor(QStringLiteral("#A31515")));
    m_commentFormat_.setForeground(QColor(QStringLiteral("#008000")));
  } else {
    m_keywordFormat_.setForeground(QColor(QStringLiteral("#569CD6")));
    m_builtinFormat_.setForeground(QColor(QStringLiteral("#4EC9B0")));
    m_variableFormat_.setForeground(QColor(QStringLiteral("#9CDCFE")));
    m_singleQuoteFormat_.setForeground(QColor(QStringLiteral("#CE9178")));
    m_doubleQuoteFormat_.setForeground(QColor(QStringLiteral("#CE9178")));
    m_commentFormat_.setForeground(QColor(QStringLiteral("#6A9955")));
  }
}

void
BashHighlighterLite::setTheme(Theme theme)
{
  if (m_currentTheme_ != theme) {
    m_currentTheme_ = theme;
    applyThemeColors();
    rehighlight(); // Força a atualização de todo o documento com as novas cores
  }
}

void
BashHighlighterLite::setupRules()
{
  const QStringList keywordPatterns_ = {
    QStringLiteral(R"(\bif\b)"),      QStringLiteral(R"(\bthen\b)"),
    QStringLiteral(R"(\belse\b)"),    QStringLiteral(R"(\belif\b)"),
    QStringLiteral(R"(\bfi\b)"),      QStringLiteral(R"(\bcase\b)"),
    QStringLiteral(R"(\besac\b)"),    QStringLiteral(R"(\bfor\b)"),
    QStringLiteral(R"(\bselect\b)"),  QStringLiteral(R"(\bwhile\b)"),
    QStringLiteral(R"(\buntil\b)"),   QStringLiteral(R"(\bdo\b)"),
    QStringLiteral(R"(\bdone\b)"),    QStringLiteral(R"(\bin\b)"),
    QStringLiteral(R"(\bfunction\b)")
  };

  for (const QString& pattern : keywordPatterns_) {
    m_highlightingRules_.append(
      { QRegularExpression(pattern), m_keywordFormat_ });
  }

  const QStringList builtinPatterns_ = {
    QStringLiteral(R"(\becho\b)"),   QStringLiteral(R"(\bprintf\b)"),
    QStringLiteral(R"(\bread\b)"),   QStringLiteral(R"(\bcd\b)"),
    QStringLiteral(R"(\bpwd\b)"),    QStringLiteral(R"(\bpushd\b)"),
    QStringLiteral(R"(\bpopd\b)"),   QStringLiteral(R"(\bdirs\b)"),
    QStringLiteral(R"(\blet\b)"),    QStringLiteral(R"(\beval\b)"),
    QStringLiteral(R"(\bset\b)"),    QStringLiteral(R"(\bunset\b)"),
    QStringLiteral(R"(\bexport\b)"), QStringLiteral(R"(\bexit\b)"),
    QStringLiteral(R"(\breturn\b)")
  };

  for (const QString& pattern : builtinPatterns_) {
    m_highlightingRules_.append(
      { QRegularExpression(pattern), m_builtinFormat_ });
  }

  m_highlightingRules_.append({ QRegularExpression(QStringLiteral(
                                  R"(\$[a-zA-Z_][a-zA-Z0-9_]*|\$\{[^}]+\})")),
                                m_variableFormat_ });

  m_highlightingRules_.append(
    { QRegularExpression(QStringLiteral(R"("[^"\\]*(\\.[^"\\]*)*")")),
      m_doubleQuoteFormat_ });

  m_highlightingRules_.append(
    { QRegularExpression(QStringLiteral(R"('[^']*')")), m_singleQuoteFormat_ });

  m_highlightingRules_.append(
    { QRegularExpression(QStringLiteral(R"(#.*)")), m_commentFormat_ });
}

void
BashHighlighterLite::highlightBlock(const QString& text)
{
  for (const HighlightingRule& rule_ : std::as_const(m_highlightingRules_)) {
    QRegularExpressionMatchIterator matchIterator_ =
      rule_.pattern.globalMatch(text);
    while (matchIterator_.hasNext()) {
      QRegularExpressionMatch match = matchIterator_.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule_.format);
    }
  }
}
