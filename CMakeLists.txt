# ***************************************************************************
# * Copyright (c) 2025                                                 *
# *      Volnei Cervi Puttini.  All rights reserved.                        *
# *      vcputtini@gmail.com
# *                                                                         *
# * Redistribution and use in source and binary forms, with or without      *
# * modification, are permitted provided that the following conditions      *
# * are met:                                                                *
# * 1. Redistributions of source code must retain the above copyright       *
# *    notice, this list of conditions and the following disclaimer.        *
# * 2. Redistributions in binary form must reproduce the above copyright    *
# *    notice, this list of conditions and the following disclaimer in the  *
# *    documentation and/or other materials provided with the distribution. *
# * 4. Neither the name of the Author     nor the names of its contributors *
# *    may be used to endorse or promote products derived from this software*
# *    without specific prior written permission.                           *
# *                                                                         *
# * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
# * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
# * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
# * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
# * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
# * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
# * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
# * INTERRUPTION)                                                           *
# * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
# * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
# * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
# * POSSIBILITY OFSUCH DAMAGE.                                              *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************
cmake_minimum_required(VERSION 3.5)

string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")

set(PROJECT_MAJOR_VERSION 1)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION 0)
set(PROJECT_BUILD_VERSION ${BUILD_DATE}_${BUILD_TIME})


set(PROGRAM_NAME "qBorgPilot")
set(PROGRAM_NICKNAME "BorgPilot")

set(PROJECT_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")
set(PROJECT_COMPILATION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}.${PROJECT_BUILD_VERSION}")

# Generate config.h file from config.h.in
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

project(qBorgPilot VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FOLDER src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FOLDER})
#cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)


find_package(QT NAMES Qt6 REQUIRED COMPONENTS Gui Core Widgets LinguistTools
    Network Xml Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Core Widgets LinguistTools
    Network Xml Concurrent)

set(TS_FILES ${SRC_FOLDER}/qBorgPilot_en_US.ts)
set(TS_FILES ${SRC_FOLDER}/qBorgPilot_en_US.ts ${SRC_FOLDER}/qBorgPilot_pt_BR.ts)

set(PROJECT_SOURCES
         ${SRC_FOLDER}/main.cc
         ${SRC_FOLDER}/types_global.h
         ${SRC_FOLDER}/qborgpilot.cc
         ${SRC_FOLDER}/qborgpilot.h
         ${SRC_FOLDER}/qborgpilot.ui
         ${SRC_FOLDER}/modules_listview_p.cc
         ${SRC_FOLDER}/modules_listview_p.h
         ${SRC_FOLDER}/dialogabout.cc
         ${SRC_FOLDER}/dialogabout.h
         ${SRC_FOLDER}/dialogabout.ui
         ${SRC_FOLDER}/progid.cc
         ${SRC_FOLDER}/progid.h
         ${SRC_FOLDER}/formsettings.h
         ${SRC_FOLDER}/formsettings.cc
         ${SRC_FOLDER}/formsettings.ui
         ${SRC_FOLDER}/qBorgPilot.qrc
         ${SRC_FOLDER}/messages.h
         ${SRC_FOLDER}/messages.cc
         ${SRC_FOLDER}/tableview_general_p.h
         ${SRC_FOLDER}/tableview_general_p.cc
         ${SRC_FOLDER}/tableview_dirandfiles_p.cc
         ${SRC_FOLDER}/tableview_dirandfiles_p.h
         ${SRC_FOLDER}/tableview_autoanswerers_p.cc
         ${SRC_FOLDER}/tableview_autoanswerers_p.h
         ${SRC_FOLDER}/global_tools.h
         ${SRC_FOLDER}/global_tools.cc
         ${SRC_FOLDER}/abstracttablemodel_settings_p.h
         ${SRC_FOLDER}/abstracttablemodel_settings_p.cc
         ${SRC_FOLDER}/settingshandler.h
         ${SRC_FOLDER}/settingshandler.cc         
         ${SRC_FOLDER}/environmentvars.h
         ${SRC_FOLDER}/environmentvars.cc
         ${SRC_FOLDER}/findreqexternaltools.h
         ${SRC_FOLDER}/findreqexternaltools.cc
         ${SRC_FOLDER}/dialogfindreqexternaltools.h
         ${SRC_FOLDER}/dialogfindreqexternaltools.cc
         ${SRC_FOLDER}/dialogfindreqexternaltools.ui
         ${SRC_FOLDER}/formscriptgenerator.h
         ${SRC_FOLDER}/formscriptgenerator.cc
         ${SRC_FOLDER}/formscriptgenerator.ui
         ${SRC_FOLDER}/treeview_commonoptions_p.h
         ${SRC_FOLDER}/treeview_commonoptions_p.cc
         ${SRC_FOLDER}/tablewidget_reponamesitemdelegate.h
         ${SRC_FOLDER}/tablewidget_reponamesitemdelegate.cc
         ${SRC_FOLDER}/scriptgen.h
         ${SRC_FOLDER}/scriptgen.cc
         ${SRC_FOLDER}/scripttemplates.h
         ${SRC_FOLDER}/scripttemplates.cc
         ${SRC_FOLDER}/treewidget_createbackup_p.h
         ${SRC_FOLDER}/treewidget_createbackup_p.cc
         ${SRC_FOLDER}/command2.h
         ${SRC_FOLDER}/command2.cc
         ${SRC_FOLDER}/global_options.h
         ${SRC_FOLDER}/global_options.cc
         ${SRC_FOLDER}/dialogfindreqexternaltools_p.h
         ${SRC_FOLDER}/dialogfindreqexternaltools_p.cc
         ${SRC_FOLDER}/formlistings.h
         ${SRC_FOLDER}/formlistings.cc
         ${SRC_FOLDER}/formlistings.ui
         ${SRC_FOLDER}/treeviewpatdesc_p.h
         ${SRC_FOLDER}/treeviewpatdesc_p.cc
         ${SRC_FOLDER}/treewidgetrepo_p.h
         ${SRC_FOLDER}/treewidgetrepo_p.cc
         ${SRC_FOLDER}/borgrunnerthread.h
         ${SRC_FOLDER}/borgrunnerthread.cc
         ${SRC_FOLDER}/formutilities.h
         ${SRC_FOLDER}/formutilities.cc
         ${SRC_FOLDER}/formutilities.ui
         ${SRC_FOLDER}/comboboxscriptname_p.h
         ${SRC_FOLDER}/comboboxscriptname_p.cc
         ${SRC_FOLDER}/listviewrepos_p.h
         ${SRC_FOLDER}/listviewrepos_p.cc
         ${SRC_FOLDER}/comboboxcmdoptions_p.h
         ${SRC_FOLDER}/comboboxcmdoptions_p.cc
         ${SRC_FOLDER}/listwidget_delete_p.h
         ${SRC_FOLDER}/listwidget_delete_p.cc
         ${SRC_FOLDER}/listwidget_prune_p.h
         ${SRC_FOLDER}/listwidget_prune_p.cc
         ${SRC_FOLDER}/createdefaultborgconffile.h
         ${SRC_FOLDER}/createdefaultborgconffile.cc
         ${SRC_FOLDER}/dialogshowlogfile.h
         ${SRC_FOLDER}/dialogshowlogfile.cc
         ${SRC_FOLDER}/dialogshowlogfile.ui
         ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    qt_add_executable(qBorgPilot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        README.md
        ${SRC_FOLDER}/doc/README.md        
    )

    qt_create_translation(QM_FILES ${SRC_FOLDER}/${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# In order to use the config.h file
target_include_directories(qBorgPilot PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(qBorgPilot PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Concurrent
)
target_link_libraries(qBorgPilot PRIVATE Qt6::Core)


include(GNUInstallDirs)
install(TARGETS qBorgPilot
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qBorgPilot)
endif()

