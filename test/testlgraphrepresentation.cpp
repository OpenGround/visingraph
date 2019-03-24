#include "test/testlgraphrepresentation.h"
#include "include/graph.h"
#include "include/graph/lgraphrepresentation.h"

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

    LGraphRepresentation repr;
    QCOMPARE(repr.generateFromGraph(g), true);

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

    LGraphRepresentation repr;
    QCOMPARE(repr.generateFromGraph(g), true);

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

    LGraphRepresentation repr;
    QCOMPARE(repr.generateFromGraph(g), true);

}
