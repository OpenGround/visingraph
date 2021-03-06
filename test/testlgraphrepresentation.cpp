#include "test/testlgraphrepresentation.h"
#include "include/graph.h"
#include "include/graph/lgraphrepresentation.h"

#include <QSignalSpy>

TestLGraphRepresentation::TestLGraphRepresentation(QObject *parent) : QObject(parent)
{

}

void TestLGraphRepresentation::testGenerateI4Representation()
{
    Graph g;
    for(int i=0; i<4; i++)
    {
        g.addVertex();
    }

    LGraphRepresentationManager repr;
    QSignalSpy start(&repr, SIGNAL(calculationStarted(int)));
    QSignalSpy update(&repr, SIGNAL(calculationTick(int)));
    QSignalSpy stop(&repr, SIGNAL(calculationFinished(int)));
    QSignalSpy result(&repr, SIGNAL(calculationEnded(bool)));

    repr.generateFromGraph(g);
    QVERIFY(result.wait());

    QCOMPARE(start.count(), 1);
    QCOMPARE(stop.count(), 1);
    QVERIFY(update.count() < INT_MAX);
    QCOMPARE(result.count(), 1);
    QVERIFY(result.takeFirst().at(0).toBool() == true);
}

void TestLGraphRepresentation::testGenerateK4Representation()
{
    Graph g;
    for(int i=0; i<4; i++)
    {
        g.addVertex();
    }
    for(int i=0; i<4; i++)
    {
        for(int j=i+1; j<4; j++)
        {
            g.addEdge(i, j);
        }
    }

    LGraphRepresentationManager repr;
    QSignalSpy start(&repr, SIGNAL(calculationStarted(int)));
    QSignalSpy update(&repr, SIGNAL(calculationTick(int)));
    QSignalSpy stop(&repr, SIGNAL(calculationFinished(int)));
    QSignalSpy result(&repr, SIGNAL(calculationEnded(bool)));

    repr.generateFromGraph(g);
    QVERIFY(result.wait());

    QCOMPARE(start.count(), 1);
    QCOMPARE(stop.count(), 1);
    QVERIFY(update.count() < INT_MAX);
}

void TestLGraphRepresentation::testGenerateC4Representation()
{
    Graph g;
    for(int i=0; i<4; i++)
    {
        g.addVertex();
    }
    for(int i=0; i<3; i++)
    {
        g.addEdge(i, i+1);
    }
    g.addEdge(3, 0);

    LGraphRepresentationManager repr;
    QSignalSpy start(&repr, SIGNAL(calculationStarted(int)));
    QSignalSpy update(&repr, SIGNAL(calculationTick(int)));
    QSignalSpy stop(&repr, SIGNAL(calculationFinished(int)));

    QSignalSpy result(&repr, SIGNAL(calculationEnded(bool)));

    repr.generateFromGraph(g);
    QVERIFY(result.wait());

    QCOMPARE(start.count(), 1);
    QCOMPARE(stop.count(), 1);
    QVERIFY(update.count() < INT_MAX);
    QCOMPARE(result.count(), 1);
    QVERIFY(result.takeFirst().at(0).toBool() == true);
}

void TestLGraphRepresentation::testFailToGenerateI21Representation()
{
    Graph g;
    for(int i=0; i<21; i++)
    {
        g.addVertex();
    }
    LGraphRepresentationManager repr;
    QSignalSpy start(&repr, SIGNAL(calculationStarted(int)));
    QSignalSpy update(&repr, SIGNAL(calculationTick(int)));
    QSignalSpy stop(&repr, SIGNAL(calculationFinished(int)));
    QSignalSpy tooBig(&repr, SIGNAL(graphTooBig()));

    QSignalSpy result(&repr, SIGNAL(calculationEnded(bool)));

    repr.generateFromGraph(g);

    QCOMPARE(tooBig.count(), 1);
    QCOMPARE(start.count(), 0);
    QCOMPARE(stop.count(), 0);
    QCOMPARE(update.count(), 0);
    QCOMPARE(result.count(), 0);
}
