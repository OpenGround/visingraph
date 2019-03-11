#include "include/graph.h"

#include <algorithm>

Graph::Graph()
{

}

/*!
 * \brief Graph::add_vertex
 * \return The added vertex
 */
vertex Graph::add_vertex()
{
    if(vertices.empty())
    {
        vertices.push_back(0);
        return 0;
    }

    if(vertices.size() - 1 == vertices[vertices.size() - 1])
    {
        vertices.push_back(vertices.size());
        return vertices.size();
    }

    for(vertex i = 0; i < vertices.size(); i++)
    {
        if(vertices[i] > i)
        {
            vertices.insert(vertices.begin() + i, i);
            return i;
        }
    }

    return -1;
}

/*!
 * \brief Graph::remove_vertex
 * \param v The vertex to be removed
 */
void Graph::remove_vertex(vertex v)
{
    std::remove_if(vertices.begin(), vertices.end(), [v](vertex& t) {return t==v;});
    for(auto edge = edges.begin(); edge != edges.end(); ++edge)
    {
        if(edge->first == v || edge->second == v)
        {
            edges.erase(edge);
            edge--;
        }
    }
}

/*!
 * \brief Graph::add_edge
 * \param u A possible vertex
 * \param v A possible vertex
 * \return true if both vertices exist in the graph and the edge didn't already exist
 */
bool Graph::add_edge(vertex u, vertex v)
{
    if(std::binary_search(vertices.begin(), vertices.end(), u) &&
       std::binary_search(vertices.begin(), vertices.end(), v))
    {
        edges.insert(std::minmax(u,v));
        return true;
    }
    return false;
}
