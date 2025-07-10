/***************************************************************************
 * Copyright (c) 2025                                                 *
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
#include "messages.h"

namespace MessagesNS {

/*!
 * \brief Messages::Messages
 */
Messages::Messages()
  : msg_{}
{
  fillMap();
}

/*!
 * \brief Messages::~Messages
 */
Messages::~Messages() {}

/*!
 * \brief Messages::instance
 * \return
 */
Messages&
Messages::instance()
{
  static Messages m_;
  return m_;
}

/*!
 * \brief Messages::set
 * \param token_
 * \return *this
 */
Messages&
Messages::set(Tokens&& token_)
{
  msg_ = messages_hash_.value(token_);
  return *this;
}

/*!
 * \brief Messages::text
 * \return
 */
const QString
Messages::text() noexcept
{
  return msg_;
}

/*!
 * \brief Messages::fillMap
 */
void
Messages::fillMap()
{
  messages_hash_ = {
    { Tokens::SYS_INTERNAL_ERROR, QObject::tr(SYS_INTERNAL_ERROR) },
    { Tokens::HELP_000, QObject::tr(HELP_000) },
    { Tokens::HELP_001, QObject::tr(HELP_001) },
    { Tokens::HELP_002, QObject::tr(HELP_002) },
    { Tokens::HELP_003, QObject::tr(HELP_003) },
    { Tokens::HELP_004, QObject::tr(HELP_004) },
    { Tokens::HELP_005, QObject::tr(HELP_005) },
    { Tokens::HELP_006, QObject::tr(HELP_006) },
    { Tokens::HELP_GEN_000, QObject::tr(HELP_GEN_000) },
    { Tokens::HELP_GEN_001, QObject::tr(HELP_GEN_001) },
    { Tokens::HELP_GEN_002, QObject::tr(HELP_GEN_002) },
    { Tokens::HELP_GEN_003, QObject::tr(HELP_GEN_003) },
    { Tokens::HELP_GEN_004, QObject::tr(HELP_GEN_004) },
    { Tokens::HELP_GEN_005, QObject::tr(HELP_GEN_005) },
    { Tokens::HELP_GEN_006, QObject::tr(HELP_GEN_006) },
    { Tokens::HELP_GEN_007, QObject::tr(HELP_GEN_007) },
    { Tokens::HELP_GEN_008, QObject::tr(HELP_GEN_008) },
    { Tokens::HELP_GEN_009, QObject::tr(HELP_GEN_009) },
    { Tokens::HELP_GEN_010, QObject::tr(HELP_GEN_010) },
    { Tokens::HELP_GEN_011, QObject::tr(HELP_GEN_011) },
    { Tokens::HELP_GEN_012, QObject::tr(HELP_GEN_012) },
    { Tokens::HELP_GEN_013, QObject::tr(HELP_GEN_013) },
    { Tokens::HELP_GEN_014, QObject::tr(HELP_GEN_014) },
    { Tokens::HELP_GEN_015, QObject::tr(HELP_GEN_015) },
    { Tokens::HELP_GEN_016, QObject::tr(HELP_GEN_016) },
    { Tokens::HELP_GEN_017, QObject::tr(HELP_GEN_017) },
    { Tokens::HELP_GEN_018, QObject::tr(HELP_GEN_018) },
    { Tokens::GEN_WARN_00001, QObject::tr(GEN_WARN_00001) },
  };
}

} // namespace MessagesNS
