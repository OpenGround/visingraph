#include "testgraph.h"
#include "include/graph.h"

TestGraph::TestGraph(QObject *parent) : QObject(parent)
{

}


void TestGraph::test_add_vertex()
{
    Graph g;
    QCOMPARE(g.addVertex(), (vertex)0);
    QCOMPARE(g.addVertex(), (vertex)1);
    QCOMPARE(g.addVertex(), (vertex)2);
    QCOMPARE(g.addVertex(), (vertex)3);
}


void TestGraph::test_add_remove_vertex()
{
    Graph g;
    g.addVertex();
    g.addVertex();
    g.addVertex();

    g.removeVertex(1);
    QCOMPARE(g.addVertex(), (vertex)1);
}

void TestGraph::test_remove_vertex()
{
    Graph g;
    g.removeVertex(0);
    g.addVertex();
    g.addVertex();
    QCOMPARE(g.getVertices().size(), (std::size_t)2);
    g.removeVertex(0);
    QCOMPARE(g.getVertices().size(), (std::size_t)1);
    g.removeVertex(0);
    QCOMPARE(g.getVertices().size(), (std::size_t)1);

}


