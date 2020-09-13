#ifndef GRAPH_H
#define GRAPH_H

#include <utility> //std::pair
#include <vector> //std::vector
#include <set> //std::set
#include <map> //std::map
#include <queue>

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

    bool loadFromGraph6(std::string&);

    std::vector<vertex> getVertices() {return vertices;}
    std::map<vertex, std::set<vertex>> getEdges() {return edges;}
    std::string getG6() {return g6;}

private:
    std::vector<vertex> vertices;
    std::map<vertex, std::set<vertex>> edges; //as a set of neighbours

    std::queue<bool> getBits(std::string&);
    std::string g6;
};

#endif // GRAPH_H
