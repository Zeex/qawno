SOURCES += \
    main.cpp \
    OutputWidget.cpp \
    MenuBar.cpp \
    MainWindow.cpp \
    IssueList.cpp \
    EditorWidget.cpp \
    Compiler.cpp \
    Issue.cpp \
    CompilerOptionsDialog.cpp \
    SyntaxHighlighter.cpp

HEADERS += \
    OutputWidget.h \
    MenuBar.h \
    MainWindow.h \
    IssueList.h \
    EditorWidget.h \
    Compiler.h \
    Issue.h \
    CompilerOptionsDialog.h \
    SyntaxHighlighter.h

RC_FILE = qawno.rc

win32:QMAKE_LFLAGS_RELEASE += -static-libgcc

OTHER_FILES += \
    README \
    qawno.rc \
    pawn.ico

FORMS += \
    CompilerOptionsDialog.ui
