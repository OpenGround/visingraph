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


bool Graph::loadFromGraph6(std::string& gr)
{
    std::size_t vertex_no;
    std::string edgestring;

    if (gr[0] < 126)
    {
        vertex_no = gr[0] - 63;
        edgestring = gr.substr(1);
    }
    else if (gr[1] < 126)
    {
        vertex_no = (gr[3] - 63) + (gr[2] - 63) * 64 + (gr[1] - 63) * 64 * 64;
        edgestring = gr.substr(5);
    }
    else
    {
        vertex_no = (gr[7] - 63) + (gr[6] - 63) * 64 + (gr[5] - 63) * 64 * 64 + (gr[4] - 63) * 64 * 64 * 64 + (gr[3] - 63) * 64 * 64 * 64 * 64 + (gr[2] - 63) * 64 * 64 * 64 * 64 * 64;
        edgestring = gr.substr(9);
    }

    vertices.clear();
    edges.clear();

    for(vertex i = 0; i < vertex_no; i++)
    {
        addVertex(i);
    }


    std::queue<bool> edgeBits = getBits(edgestring);
    bool isEdge;

    for(vertex i = 0; i < vertex_no; i++)
    {
        for(vertex j = 0; j < i; j++)
        {
            if(edgeBits.front())
            {
                addEdge(i, j);
            }

            edgeBits.pop();
        }
    }

    g6 = gr;
    return true;
}


std::queue<bool> Graph::getBits(std::string& str)
{
    std::queue<bool> q;
    std::vector<uint8_t> POWERS = {32, 16, 8, 4, 2, 1};
    for(auto c: str)
    {
        uint8_t b = c - 63;
        for(std::size_t i = 0; i < 6; i++)
        {
            q.push((b / POWERS[i]) % 2);
        }
    }

    return q;
}
