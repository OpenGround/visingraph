#ifndef GRAPH_H
#define GRAPH_H

#include <utility> //std::pair
#include <vector> //std::vector
#include <set> //std::set

using vertex = std::size_t;


class Graph
{
public:
    Graph();
    vertex add_vertex();
    void remove_vertex(vertex v);
    bool add_edge(vertex u, vertex v);


private:
    std::vector<vertex> vertices;
    std::set<std::pair<vertex, vertex>> edges;
};

#endif // GRAPH_H
