INCLUDEPATH ''=
DEPENDSPATH ''=

QT       += core gui widgets

SOURCES += \
    src/graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mainwidget.cpp \
    src/ui/inputgraphicsview.cpp \
    src/ui/outputgraphicsview.cpp \
    src/graph/graphrepresentation.cpp \
    src/graph/lgraphrepresentation.cpp \
    src/poset.cpp \
    $$PWD/src/graph/mptgraphrepresentation.cpp

HEADERS += \
    include/graph.h \
    include/mainwindow.h \
    include/mainwidget.h \
    include/ui/inputgraphicsview.h \
    include/ui/outputgraphicsview.h \
    include/graph/graphrepresentation.h \
    include/graph/lgraphrepresentation.h \
    include/poset.h \
    $$PWD/include/graph/mptgraphrepresentation.h

FORMS += \
    ui/mainwindow.ui \
    ui/mainwidget.ui
