#include "include/graph.h"

#include <algorithm>

Graph::Graph()
{

}

/*!
 * \brief Graph::addVertex
 * \return The added vertex
 */
vertex Graph::addVertex()
{
    if(vertices.empty())
    {
        vertices.push_back(0);
        edges.insert(std::make_pair(0, std::set<vertex>()));
        return 0;
    }

    if(vertices.size() - 1 == vertices[vertices.size() - 1])
    {
        vertex v = vertices.size();
        vertices.push_back(v);
        edges.insert(std::make_pair(v, std::set<vertex>()));
        return v;
    }

    for(vertex i = 0; i < vertices.size(); i++)
    {
        if(vertices[i] > i)
        {
            vertices.insert(vertices.begin() + i, i);
            edges.insert(std::make_pair(i, std::set<vertex>()));
            return i;
        }
    }

    return -1;
}

bool Graph::addVertex(vertex v)
{
    for(vertex i = 0; i < vertices.size(); i++)
    {
        if(vertices[i] == v)
        {
            return false;
        }
        if(vertices[i] > v)
        {
            vertices.insert(vertices.begin() + i, v);
            edges.insert(std::make_pair(v, std::set<vertex>()));
            return true;
        }
    }

    vertices.push_back(v);
    edges.insert(std::make_pair(v, std::set<vertex>()));
    return true;
}

/*!
 * \brief Graph::removeVertex
 * \param v The vertex to be removed
 */
void Graph::removeVertex(vertex v)
{
    if(std::find(vertices.begin(), vertices.end(), v) == vertices.end())
    {
        return;
    }

    // Remove the vertex and shrink the container
    vertices.erase(std::remove(vertices.begin(), vertices.end(), v), vertices.end());

    if(edges.count(v) == 0)
    {
        return;
    }

    auto neighbours = edges.at(v);
    for(vertex n: neighbours)
    {
        edges.at(n).erase(v);
    }

    edges.erase(v);
}

/*!
 * \brief Graph::addEdge
 * \param u A possible vertex
 * \param v A possible vertex
 * \return true if both vertices exist in the graph and the edge didn't already exist
 */
bool Graph::addEdge(vertex u, vertex v)
{
    if(std::binary_search(vertices.begin(), vertices.end(), u) &&
       std::binary_search(vertices.begin(), vertices.end(), v))
    {
        edges.at(u).insert(v);
        edges.at(v).insert(u);
        return true;
    }
    return false;
}

void Graph::removeEdge(vertex u, vertex v)
{
    try {
        edges.at(u).erase(v);
        edges.at(v).erase(u);
    } catch (std::out_of_range) {

    }
}
