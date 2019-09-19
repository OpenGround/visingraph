#include "testgraph.h"
#include "testlgraphrepresentation.h"
#include "testutils.h"
#include "include/testposet.h"

#include <QTest>

#include <algorithm> // std::max

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    int retval = 0;

    TestGraph testgraph;
    retval = std::max(QTest::qExec(&testgraph), retval);

    TestLGraphRepresentation testlgraphrepresentation;
    retval = std::max(QTest::qExec(&testlgraphrepresentation), retval);

    TestUtils testutils;
    retval = std::max(QTest::qExec(&testutils), retval);

    TestPoset testposet;
    retval = std::max(QTest::qExec(&testposet), retval);

    return retval;
}
