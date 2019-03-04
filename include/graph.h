#ifndef GRAPH_H
#define GRAPH_H

#include <utility> //std::pair
#include <vector> //std::vector


class Graph
{
public:
    Graph();

private:
    std::vector<std::size_t> vertices;
    std::vector<std::pair<std::size_t, std::size_t>> edges;
};

#endif // GRAPH_H
