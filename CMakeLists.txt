cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0043 OLD)

project(qawno VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
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
