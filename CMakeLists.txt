# This file is part of qawno.
#
# qawno is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# qawno is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with qawno. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(Qawno VERSION 1.0.0 LANGUAGES CXX)

list(APPEND
     CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

configure_file(qawno.h.in qawno.h @ONLY)

set(HEADERS
  src/AboutDialog.h
  src/Compiler.h
  src/CompilerSettingsDialog.h
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
  src/CompilerSettingsDialog.cpp
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
  src/CompilerSettingsDialog.ui
  src/FindDialog.ui
  src/GoToDialog.ui
  src/MainWindow.ui
  src/ReplaceDialog.ui
)

qt5_wrap_ui(UI_HEADERS ${UI_SOURCES})
qt5_add_resources(EMBEDDED_RESOURCES qawno.qrc)

if(APPLE)
  set(ICON_IMAGES_PATTERN
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/icon_*x*.png")
  set(ICONSET_DIR "${CMAKE_CURRENT_BINARY_DIR}/qawno.iconset")
  set(ICNS_FILE "${CMAKE_CURRENT_BINARY_DIR}/qawno.icns")
  set(RESORUCES "${ICNS_FILE}")
  file(GLOB ICON_FILES "${ICON_IMAGES_PATTERN}")
  add_custom_command(
    OUTPUT ${ICNS_FILE}
    COMMAND mkdir -p "${ICONSET_DIR}"
    COMMAND cp ${ICON_FILES} "${ICONSET_DIR}/"
    COMMAND iconutil --convert icns "${ICONSET_DIR}"
    DEPENDS ${ICON_FILES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  )
else()
  set(RESOURCES assets/images/qawno.ico)
endif()

add_executable(qawno
  ${SOURCES}
  ${HEADERS}
  ${UI_HEADERS}
  ${EMBEDDED_RESOURCES}
  ${RESORUCES}
)

if(APPLE)
  set_target_properties(qawno PROPERTIES
    MACOSX_BUNDLE TRUE
    RESOURCE "${ICNS_FILE}"
  )
endif()

target_link_libraries(qawno
  Qt5::Core
  Qt5::Widgets
)

if(WIN32)
  if(MSVC)
    set_property(TARGET qawno APPEND_STRING PROPERTY
                 LINK_FLAGS " /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  elseif(MINGW)
    set_property(TARGET qawno APPEND_STRING PROPERTY
                 LINK_FLAGS " -Wl,-subsystem,windows")
  endif()
endif()

if(UNIX AND NOT APPLE)
  set(INSTALL_BINARY_DIR bin)
  set(INSTALL_LIBRARY_DIR lib)
else()
  set(INSTALL_BINARY_DIR .)
  set(INSTALL_LIBRARY_DIR .)
endif()

install(
  TARGETS qawno
  RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
  BUNDLE DESTINATION ${INSTALL_BINARY_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
)

if(UNIX AND NOT APPLE)
  install(FILES ${RESOURCES} DESTINATION share)
endif()

if(WIN32 OR APPLE)
  get_target_property(QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
  get_filename_component(QT5_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
endif()

if(WIN32)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
  include(InstallRequiredSystemLibraries)
  install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION .)

  find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT5_BIN_DIR}")
  if(NOT WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "Could not determine location of windeployqt")
  endif()

  set(INSTALL_APP_DIR "${CMAKE_CURRENT_BINARY_DIR}/app")
  install(CODE "
      file(REMOVE_RECURSE \"${INSTALL_APP_DIR}\")
      file(MAKE_DIRECTORY \"${INSTALL_APP_DIR}\")
      file(COPY \"${CMAKE_CURRENT_BINARY_DIR}/qawno.exe\"
           DESTINATION \"${INSTALL_APP_DIR}\")
  ")
  install(CODE "
    execute_process(
      COMMAND ${WINDEPLOYQT_EXECUTABLE}
              --no-opengl-sw
              --no-quick-import
              --no-svg
              --no-translations
              --no-webkit2
              \"${INSTALL_APP_DIR}\")
  ")
  install(DIRECTORY "${INSTALL_APP_DIR}/" DESTINATION .)
endif()

if(APPLE)
  set_target_properties(qawno PROPERTIES OUTPUT_NAME Qawno)

  find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${QT5_BIN_DIR}")
  if(NOT MACDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "Could not determine location of macdeployqt")
  endif()

  set(INSTALL_APP_DIR "${CMAKE_CURRENT_BINARY_DIR}/qawno.app")
  install(CODE "
    execute_process(
      COMMAND ${MACDEPLOYQT_EXECUTABLE} \"${INSTALL_APP_DIR}\")
  ")
  install(DIRECTORY "${INSTALL_APP_DIR}"
          DESTINATION .
          USE_SOURCE_PERMISSIONS)
endif()

set(CPACK_PACKAGE_NAME Qawno)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR Zeex)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple Pawn editor")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY Qawno)

if(WIN32)
  if(NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR WIX)
  endif()
  set(CPACK_WIX_PRODUCT_ICON
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/qawno.ico")
  set(CPACK_WIX_PRODUCT_GUID "E5FCD5B0-D4F3-4F4C-B7E1-89BB99706556")
  set(CPACK_WIX_UPGRADE_GUID "1CB3E5B2-C719-4A65-8AF1-B5C19783C779")
  set(CPACK_WIX_UI_DIALOG
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/msi_dialog.bmp")
  set(CPACK_WIX_UI_BANNER
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/msi_banner.bmp")
elseif(CMAKE_SYSTEM_NAME EQUAL Linux)
  if(NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR TGZ)
  endif()
elseif(APPLE)
  if(NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR DragNDrop)
  endif()
  set(CPACK_BUNDLE_ICON qawno.icns)
endif()

include(CPack)
