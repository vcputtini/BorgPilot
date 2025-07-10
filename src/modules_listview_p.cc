/***************************************************************************
 * Copyright (c) 2025                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase6 project, a images/documents           *
 * database program.                                                       *
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

#include "modules_listview_p.h"

/*!
 * \brief modules_listview_p::modules_listview_p
 * \param parent
 */
modules_listview_p::modules_listview_p(QWidget* parent)
  : QListView(parent)
{

  sListModNames_ << tr("Script Gerenetor") << tr("Search & Listing")
                 << tr("Utilities");

  const QList<QPixmap> listImages_ = { QPixmap(":/images/borg_icon_1.png"),
                                       QPixmap(":/images/borg_icon_2.png"),
                                       QPixmap(":/images/borg_icon_4.png") };

  setViewMode(QListView::IconMode);
  setSpacing(10);
  setIconSize(QSize(128, 128));

  mdlThumbs_ = new QStandardItemModel;

  for (uint8_t i = 0; i < sListModNames_.count(); ++i) {
    QStandardItem* item = new QStandardItem;
    item->setText(sListModNames_.at(i));
    item->setIcon(listImages_.at(i));
    item->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setData(i, Qt::UserRole); // Define internal code for module
    mdlThumbs_->setItem(i, 0, item);
  }
  setModel(mdlThumbs_);
  show();
}

/*!
 * \brief modules_listview_p::~modules_listview_p
 */
modules_listview_p::~modules_listview_p()
{
  delete mdlThumbs_;
}
