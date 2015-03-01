# This file is part of qawno.
#
# qawno is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# qawno is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with qawno.  If not, see <http:#www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0043 OLD)

project(qawno VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

configure_file(qawno.h.in qawno.h @ONLY)

set(HEADERS
  src/AboutDialog.h
  src/Compiler.h
  src/CompilerOptionsDialog.h
  src/EditorWidget.h
  src/FindDialog.h
  src/GoToDialog.h
  src/MainWindow.h
  src/OutputWidget.h
  src/ReplaceDialog.h
  src/StatusBar.h
  src/SyntaxHighlighter.h
)

set(SOURCES
  src/AboutDialog.cpp
  src/Compiler.cpp
  src/CompilerOptionsDialog.cpp
  src/EditorWidget.cpp
  src/FindDialog.cpp
  src/GoToDialog.cpp
  src/main.cpp
  src/MainWindow.cpp
  src/OutputWidget.cpp
  src/ReplaceDialog.cpp
  src/StatusBar.cpp
  src/SyntaxHighlighter.cpp
  qawno.rc
)

set(UI_SOURCES
  src/AboutDialog.ui
  src/CompilerOptionsDialog.ui
  src/FindDialog.ui
  src/GoToDialog.ui
  src/MainWindow.ui
  src/ReplaceDialog.ui
)

qt5_wrap_ui(UI_HEADERS ${UI_SOURCES})
qt5_add_resources(RESOURCES qawno.qrc)

add_executable(qawno ${HEADERS} ${SOURCES} ${UI_HEADERS} ${RESOURCES})
qt5_use_modules(qawno Core Widgets)

if(WIN32)
  if(MSVC)
    set_property(TARGET qawno APPEND_STRING PROPERTY
                 LINK_FLAGS " /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  elseif(MINGW)
    set_property(TARGET qawno APPEND_STRING PROPERTY
                 LINK_FLAGS " -Wl,-subsystem,windows")
  endif()
endif()
