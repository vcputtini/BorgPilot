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
#include "qborgpilot.h"

#include <QApplication>
#include <QLocale>
#include <QString>
#include <QTranslator>

#include <unistd.h> // geteuid()

#include "createdefaultborgconffile.h"

#define ___ROOT_MODE___

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "qBorgPilot_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

#ifdef ___ROOT_MODE___
  if (geteuid() != 0) {
    QMessageBox box_;
    box_.setIcon(QMessageBox::Warning);
    box_.setText("WARNING");
    box_.setInformativeText(
      "We apologize, but at this time this program can only be run "
      "by the <B>root user</B>.<br><br>You can run the program via the "
      "<b>sudo</b> command.");
    box_.exec();
    return -1;
  }
#endif

  CreateDefaultBorgConfFile dflConf_;
  dflConf_.create();

  qBorgPilot w;
#ifdef ___ROOT_MODE___
  w.setWindowTitle(QString("%1 [root mode]").arg(w.windowTitle()));
#endif
  w.showMaximized();
  return a.exec();
}
