#include "testgraph.h"

#include <QTest>

#include <algorithm> // std::max

int main()
{
    int retval = 0;

    TestGraph testgraph;
    retval = std::max(QTest::qExec(&testgraph), retval);

    return retval;
}
