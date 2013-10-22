QT += core gui widgets

SOURCES += \
  src/main.cpp \
  src/AboutDialog.cpp \
  src/Compiler.cpp \
  src/CompilerOptionsDialog.cpp \
  src/EditorWidget.cpp \
  src/FindDialog.cpp \
  src/GoToDialog.cpp \
  src/MainWindow.cpp \
  src/OutputWidget.cpp \
  src/ReplaceDialog.cpp \
  src/SyntaxHighlighter.cpp

HEADERS += \
  src/AboutDialog.h \
  src/Compiler.h \
  src/CompilerOptionsDialog.h \
  src/EditorWidget.h \
  src/FindDialog.h \
  src/GoToDialog.h \
  src/MainWindow.h \
  src/OutputWidget.h \
  src/ReplaceDialog.h \
  src/SyntaxHighlighter.h

FORMS += \
  src/AboutDialog.ui \
  src/CompilerOptionsDialog.ui \
  src/FindDialog.ui \
  src/GoToDialog.ui \
  src/ReplaceDialog.ui

RESOURCES = qawno.qrc

win32:QMAKE_LFLAGS_RELEASE += -static-libgcc