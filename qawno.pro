SOURCES += \
    main.cpp \
    OutputWidget.cpp \
    MenuBar.cpp \
    MainWindow.cpp \
    EditorWidget.cpp \
    Compiler.cpp \
    Issue.cpp \
    CompilerOptionsDialog.cpp \
    SyntaxHighlighter.cpp \
    GoToDialog.cpp \
    FindDialog.cpp \
    FindReplaceDialog.cpp

HEADERS += \
    OutputWidget.h \
    MenuBar.h \
    MainWindow.h \
    IssueList.h \
    EditorWidget.h \
    Compiler.h \
    Issue.h \
    CompilerOptionsDialog.h \
    SyntaxHighlighter.h \
    GoToDialog.h \
    FindDialog.h \
    FindReplaceDialog.h

RC_FILE = qawno.rc

win32:QMAKE_LFLAGS_RELEASE += -static-libgcc

OTHER_FILES += \
    README \
    qawno.rc \
    pawn.ico

FORMS += \
    CompilerOptionsDialog.ui \
    GoToDialog.ui \
    FindDialog.ui \
    FindReplaceDialog.ui

