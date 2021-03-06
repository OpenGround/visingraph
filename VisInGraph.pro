#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T18:04:30
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = VisInGraph
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/graph.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mainwidget.cpp \
    src/ui/inputgraphicsview.cpp \
    src/graph/graphrepresentation.cpp \
    src/graph/lgraphrepresentation.cpp \
    src/ui/outputgraphicsview.cpp \
    src/poset.cpp

HEADERS += \
    include/graph.h \
    include/mainwindow.h \
    include/mainwidget.h \
    include/ui/inputgraphicsview.h \
    include/graph/graphrepresentation.h \
    include/graph/lgraphrepresentation.h \
    include/ui/outputgraphicsview.h \
    include/poset.h

FORMS += \
    ui/mainwindow.ui \
    ui/mainwidget.ui

QMAKE_CXXFLAGS += \
    -Wall \
    -Wextra \
    -Wpedantic \
    --std=c++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
