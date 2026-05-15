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
#ifndef BASHSCRIPTGENERATOR_H
#define BASHSCRIPTGENERATOR_H

#include <QObject>

#include <QFile>
#include <QFileDevice>
#include <QIODevice>
#include <QMessageBox>
#include <QMultiHash>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QTextStream>

#include "command2.h"
#include "types_global.h"

/*! \note MENSAGEM DE COMMIT - APAGAR DEPOIS DE FAZER O COMMIT
 * Adiciona suporte a variáveis ​​de ambiente gerais e atualiza os modos de
 * criptografia
 *
 * Implementa a geração dinâmica de variáveis ​​de ambiente gerais e de
 * resposta automática usando uma função lambda altamente otimizada com
 *QStringBuilder. Atualiza os argumentos de inicialização do repositório para
 * usar a flag explícita --encryption e introduz o modo de criptografia
 * 'Authenticated'.
 *
 *- Adiciona a função lambda mountEnvVars, aproveitando QStringBuilder (%) e
 *reserve() para concatenação de strings otimizada.
 *
 *- Inclui <QStringBuilder> e corrige a capitalização do cabeçalho
 *<QStringLiteral>.
 *
 *- Introduz Command::EncryptModes::Authenticated como uma nova opção de
 *inicialização.
 *
 *- Substitui a flag de inicialização legada '-e' pela flag explícita
 *'--encryption='.
 *
 *- Traduz os caminhos de documentação codificados para inglês.
 */

class BashScriptGenerator : public QObject
{
  Q_OBJECT
public:
  explicit BashScriptGenerator(const QString& fileName_,
                               QObject* parent = nullptr);
  explicit BashScriptGenerator(const QString& fileName_,
                               AppTypes::RepoistoryCreateDataMap data_create_m_,
                               QObject* parent = nullptr);

  void setRepositoryPath(const QString& path_) noexcept;
  BashScriptGenerator& append(int index_, AppTypes::InitRepositoryTuple data_);
  void setCommons(const QString& value_);
  void writeScript() noexcept;

  enum class [[nodiscard]] MainModel : int
  {
    Create = 0x00,
    Initialization = 0x01,
    None = 0xff
  };
  void setMainModel(MainModel model_ = MainModel::None);
  MainModel mainModel();

private:
  QTextStream m_out_ts_{};
  QFile m_out_file_{};
  QSettings m_setting_{};
  AppTypes::InitRepositoryMap m_data_map_{};
  AppTypes::RepoistoryCreateDataMap m_data_map_create_{};
  QString m_fname_{};
  QString m_repoPath_{};
  Command m_command_;
  QString m_commons_; // Common options
  MainModel m_mainModel_;

  void commandForInitialization() noexcept;
  void commandForBackup() noexcept;

signals:
  void fileError(const QString& text_);
};

#endif // BASHSCRIPTGENERATOR_H
