include(VisInGraph.pri)

QT += testlib

SOURCES += test/main.cpp \
    test/testgraph.cpp \
    test/testlgraphrepresentation.cpp \
    test/testutils.cpp \
    test/testposet.cpp

SOURCES -= src/main.cpp

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# qmake defaults to adding -O2 when compiling, which can cause gcov to miss that some lines were optimised out.
QMAKE_CXXFLAGS_RELEASE -= -O2

HEADERS += \
    test/testgraph.h \
    test/testlgraphrepresentation.h \
    test/testutils.h \
    test/include/testposet.h
