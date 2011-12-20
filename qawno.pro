SOURCES += \
    main.cpp \
    mainwindow.cpp \
    compiler.cpp \
    menubar.cpp \
    outputwidget.cpp \
    highlighter.cpp \
    editor.cpp \
    issuelist.cpp

HEADERS += \
    mainwindow.h \
    compiler.h \
    menubar.h \
    outputwidget.h \
    highlighter.h \
    editor.h \
    issuelist.h

RC_FILE = qawno.rc

win32:QMAKE_LFLAGS_RELEASE += -static-libgcc


























