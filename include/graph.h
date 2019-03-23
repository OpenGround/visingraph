#ifndef GRAPH_H
#define GRAPH_H

#include <utility> //std::pair
#include <vector> //std::vector
#include <set> //std::set
#include <map> //std::map

using vertex = std::size_t;


class Graph
{
public:
    Graph();
    vertex addVertex();
    bool addVertex(vertex);
    void removeVertex(vertex v);
    bool addEdge(vertex u, vertex v);
    void removeEdge(vertex u, vertex v);

    std::vector<vertex> getVertices() {return vertices;}
    std::map<vertex, std::set<vertex>> getEdges() {return edges;}


private:
    std::vector<vertex> vertices;
    std::map<vertex, std::set<vertex>> edges;
};

#endif // GRAPH_H
