cmake_minimum_required(VERSION 2.8.12)

project(qawno)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

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