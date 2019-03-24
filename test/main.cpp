#include "testgraph.h"
#include "testlgraphrepresentation.h"

#include <QTest>

#include <algorithm> // std::max

int main()
{
    int retval = 0;

    TestGraph testgraph;
    retval = std::max(QTest::qExec(&testgraph), retval);

    TestLGraphRepresentation testlgraphrepresentation;
    retval = std::max(QTest::qExec(&testlgraphrepresentation), retval);

    return retval;
}
