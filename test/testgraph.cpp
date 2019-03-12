#include "testgraph.h"
#include "include/graph.h"

TestGraph::TestGraph(QObject *parent) : QObject(parent)
{

}


void TestGraph::test_add_vertex()
{
    Graph g;
    QCOMPARE(g.add_vertex(), (vertex)0);
    QCOMPARE(g.add_vertex(), (vertex)1);
    QCOMPARE(g.add_vertex(), (vertex)2);
    QCOMPARE(g.add_vertex(), (vertex)3);
}


void TestGraph::test_add_remove_vertex()
{
    Graph g;
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();

    g.remove_vertex(1);
    QCOMPARE(g.add_vertex(), (vertex)1);
}

void TestGraph::test_remove_vertex()
{
    Graph g;
    g.remove_vertex(0);
    g.add_vertex();
    g.add_vertex();
    QCOMPARE(g.get_vertices().size(), (std::size_t)2);
    g.remove_vertex(0);
    QCOMPARE(g.get_vertices().size(), (std::size_t)1);
    g.remove_vertex(0);
    QCOMPARE(g.get_vertices().size(), (std::size_t)1);

}


