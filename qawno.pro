QT += core gui widgets

SOURCES += \
    src/main.cpp \
    src/OutputWidget.cpp \
    src/MenuBar.cpp \
    src/MainWindow.cpp \
    src/EditorWidget.cpp \
    src/Compiler.cpp \
    src/CompilerOptionsDialog.cpp \
    src/SyntaxHighlighter.cpp \
    src/GoToDialog.cpp \
    src/FindDialog.cpp \
    src/ReplaceDialog.cpp \
    src/AboutDialog.cpp

HEADERS += \
    src/OutputWidget.h \
    src/MenuBar.h \
    src/MainWindow.h \
    src/EditorWidget.h \
    src/Compiler.h \
    src/CompilerOptionsDialog.h \
    src/SyntaxHighlighter.h \
    src/GoToDialog.h \
    src/FindDialog.h \
    src/ReplaceDialog.h \
    src/AboutDialog.h

RC_FILE = qawno.rc

win32:QMAKE_LFLAGS_RELEASE += -static-libgcc

OTHER_FILES += \
    icons/pawn.ico \
    qawno.rc

FORMS += \
    src/CompilerOptionsDialog.ui \
    src/GoToDialog.ui \
    src/FindDialog.ui \
    src/ReplaceDialog.ui \
    src/AboutDialog.ui

