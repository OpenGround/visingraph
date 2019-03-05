include(VisInGraph.pri)

QT += testlib

SOURCES += test/main.cpp \
    test/main.cpp

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# qmake defaults to adding -O2 when compiling, which can cause gcov to miss that some lines were optimised out.
QMAKE_CXXFLAGS_RELEASE -= -O2
