/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase2 project, a images/documents           *
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

#include "dialogabout.h"
#include "progid.h"

/*!
 * \brief DialogAbout::DialogAbout
 * \param parent
 */
DialogAbout::DialogAbout(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  graphicsViewAbout->resetTransform();

  QFont fontArial15("Arial", 15, QFont::Bold);
  QFont fontArial10("Arial", 10, QFont::Normal);
  QFontMetricsF fmArial10(fontArial10);
  QFontMetricsF fmArial15(fontArial15);

  scene_ = new QGraphicsScene;
  scene_->setSceneRect(-290, -175, 580, 350);
  graphicsViewAbout->setScene(scene_);
  scene_->clear();

  pixItemLogo_ = new QGraphicsPixmapItem;

  QColor color_;
  color_.setCmyk(60, 40, 0, 40);
  scene_->setBackgroundBrush(QBrush(color_));

  QImage img_;
  QPixmap pixTmp_;

  QImageReader reader(":/images/borgpilot_64x64.png");
  reader.read(&img_);
  pixTmp_.convertFromImage(img_);

  [[maybe_unused]] QRectF rect = scene_->sceneRect();

  QTransform tf_;
  tf_.translate(-290, -200); // move (0,0) to the top left corner

  QString sText_{};

  qreal posH_{};
  // posH_ = (rect.width() - pixTmp_.width());

  pixItemLogo_->setTransform(tf_);
  pixItemLogo_->setPixmap(pixTmp_);
  pixItemLogo_->setPos(15, pixTmp_.height() / 4);
  scene_->addItem(pixItemLogo_);

  qreal line_ = pixTmp_.height() + 20;
  textItem_ = new QGraphicsTextItem;
  sText_ =
    tr("%0: Just an interface for the %1").arg(PROGRAM_NICKNAME, "BorgBackup");
  textItem_->setTransform(tf_);
  textItem_->setFont(fontArial15);
  textItem_->setDefaultTextColor(Qt::white);
  posH_ = 10;
  textItem_->setPos(posH_, line_);
  textItem_->setPlainText(sText_);
  scene_->addItem(textItem_);

  const QString buildTimeStamp = QString("Compiled on: %0").arg(__TIMESTAMP__);

  QGraphicsTextItem* text_[15];
  text_[0] = new QGraphicsTextItem;
  /*!
   *  \attention PLEASE, DO NOT REMOVE THIS COPYRIGTH. Just add yours!
   */
  sText_ = QString("Copyrigh(C)'%0, Volnei Cervi Puttini <vcputtini@gmail.com>")
             .arg(QDate::currentDate().year());
  text_[0]->setTransform(tf_);
  text_[0]->setFont(fontArial10);
  text_[0]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text_[0]->setPos(posH_, line_ += fmArial15.lineSpacing());
  text_[0]->setPlainText(sText_);
  scene_->addItem(text_[0]);

  text_[1] = new QGraphicsTextItem;
  sText_ = "All rights reserved";
  text_[1]->setTransform(tf_);
  text_[1]->setFont(fontArial10);
  text_[1]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text_[1]->setPos(posH_, line_ += fmArial15.lineSpacing());
  text_[1]->setPlainText(sText_);
  scene_->addItem(text_[1]);

  text_[2] = new QGraphicsTextItem;
  sText_ = tr("Version: %0 %1").arg(ProgId::strVersion(), buildTimeStamp);
  text_[2]->setTransform(tf_);
  text_[2]->setFont(fontArial10);
  text_[2]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text_[2]->setPos(posH_, line_ += fmArial10.lineSpacing());
  text_[2]->setPlainText(sText_);
  scene_->addItem(text_[2]);

  QString verGCC{};
#ifdef Q_OS_LINUX
  verGCC = QString("Compiler: GNU GCC %1.%2.%3")
             .arg(__GNUC__)
             .arg(__GNUC_MINOR__)
             .arg(__GNUC_PATCHLEVEL__);
#else
  verGCC = QString("Compiler: MINGW GCC %1.%2.%3")
             .arg(__GNUC__)
             .arg(__GNUC_MINOR__)
             .arg(__GNUC_PATCHLEVEL__);
#endif

  QString archsys = "Arch: x86_64;";
#ifdef Q_PROCESSOR_X86_32
  archsys = "Arch: x86_32";
#endif

  QStringList lText_;
  lText_ << verGCC << archsys << QSysInfo::prettyProductName() << "" << ""
         << "BorgBackup: Copyright (C) 2015-2023 The Borg Collective (see "
            "AUTHORS file)"
         << "Copyright (C) 2010-2014 Jonas Borgström <jonas@borgstrom.se>"
         << "All rights reserved."
         << "URL: https://borgbackup.readthedocs.io/en/stable/"
         << ""
         << "All icons and images used in this program were"
         << "created by Artificial Intelligence."
         << ""
         << "The program is provided AS IS whith NO WARRANTY OF ANY KIND"
         << "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND"
         << "FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.";

  int i{ 2 };
  line_ += fmArial10.lineSpacing() * 2;

  for (const QString& s : std::as_const(lText_)) {
    text_[i] = new QGraphicsTextItem;
    text_[i]->setTransform(tf_);
    text_[i]->setFont(fontArial10);
    text_[i]->setDefaultTextColor(Qt::black);
    posH_ = 10;
    text_[i]->setPos(posH_, line_ += fmArial10.lineSpacing());
    text_[i]->setPlainText(s);
    scene_->addItem(text_[i]);
    ++i;
  }

  graphicsViewAbout->setScene(scene_);
}

/*!
 * \brief DialogAbout::~DialogAbout
 */
DialogAbout::~DialogAbout()
{
  delete pixItemLogo_;
  delete textItem_;
  delete scene_;
}
