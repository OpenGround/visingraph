#include "testgraph.h"
#include "include/graph.h"

TestGraph::TestGraph(QObject *parent) : QObject(parent)
{

}


void TestGraph::test_add_vertex()
{
    Graph g;
    QCOMPARE(g.addVertex(), static_cast<vertex>(0));
    QCOMPARE(g.addVertex(), static_cast<vertex>(1));
    QCOMPARE(g.addVertex(), static_cast<vertex>(2));
    QCOMPARE(g.addVertex(), static_cast<vertex>(3));

    QCOMPARE(g.addVertex(2), false);
    QCOMPARE(g.addVertex(5), true);
    QCOMPARE(g.addVertex(), static_cast<vertex>(4));
    QCOMPARE(g.addVertex(), static_cast<vertex>(6));

}


void TestGraph::test_add_remove_vertex()
{
    Graph g;
    g.addVertex();
    g.addVertex();
    g.addVertex();

    g.removeVertex(1);
    QCOMPARE(g.addVertex(), static_cast<vertex>(1));
}

void TestGraph::test_remove_vertex()
{
    Graph g;
    g.removeVertex(0);
    g.addVertex();
    g.addVertex();
    QCOMPARE(g.getVertices().size(), static_cast<std::size_t>(2));
    g.removeVertex(0);
    QCOMPARE(g.getVertices().size(), static_cast<std::size_t>(1));
    g.removeVertex(0);
    QCOMPARE(g.getVertices().size(), static_cast<std::size_t>(1));

}

void TestGraph::testAddRemoveEdge()
{
    Graph g;
    g.removeEdge(0,1);
    QCOMPARE(g.addEdge(0,1), false);
    g.addVertex(1234);
    g.addVertex(9876);
    QCOMPARE(g.addEdge(1234, 9876), true);
    QCOMPARE(g.getEdges().at(9876).size(), static_cast<std::size_t>(1));
    QCOMPARE(g.getEdges().at(1234).size(), static_cast<std::size_t>(1));
    //The edge exists after the insertion, so it returns true
    QCOMPARE(g.addEdge(9876, 1234), true);
    QCOMPARE(g.getEdges().at(9876).size(), static_cast<std::size_t>(1));
    QCOMPARE(g.getEdges().at(1234).size(), static_cast<std::size_t>(1));
    g.removeEdge(9876, 1234);
    QCOMPARE(g.getEdges().at(9876).size(), static_cast<std::size_t>(0));
    QCOMPARE(g.getEdges().at(1234).size(), static_cast<std::size_t>(0));


}

