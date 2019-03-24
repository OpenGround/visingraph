#include "include/graph/lgraphrepresentation.h"

LGraphRepresentation::LGraphRepresentation()
{

}

bool LGraphRepresentation::generateFromGraph(Graph& graph)
{
    return generateFromGraphBF(graph);
}

/*!
 * \brief LGraphRepresentation::generate_from_graph_BF
 * Attempt to create the representation by checking all possible
 * (n!)^2 combinations
 * \param graph The graph which is to be represented
 */
bool LGraphRepresentation::generateFromGraphBF(Graph& graph)
{
    std::vector<vertex> vertices_x = graph.getVertices();
    std::vector<vertex> vertices_y = graph.getVertices();
    std::map<vertex, std::set<vertex>> edges = graph.getEdges();

    std::map<vertex, std::vector<vertex>> satisfied_edges;
    bool viable = false;

    do
    {
        do
        {
            // Get the coordinates of the midpoints in O(n)
            std::map<vertex, std::pair<std::size_t, std::size_t>> coordinates;
            for(std::size_t i = 0; i < vertices_x.size(); ++i)
            {
                coordinates.insert(std::make_pair(vertices_x[i], std::make_pair(i, 0)));
            }
            for(std::size_t i = 0; i < vertices_x.size(); ++i)
            {
                coordinates.at(vertices_y[i]).second = i;
            }

            representation.clear();

            // Create the representation without any height or length at first
            for(auto it=coordinates.begin(); it != coordinates.end(); ++it)
            {
                LNode n;
                n.v = it->first;
                n.x = it->second.first;
                n.y = it->second.second;
                representation.push_back(n);
            }

            // Add the height and length based on the edges in O(n^2*log^2(n))
            // TODO: the runtime of this could be improved rather easily
            // Sort by x coordinates and calculate width by simply extending for
            // as long as possible.
            std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.x < b.x;});
            for(auto it=representation.begin(); it!=representation.end(); ++it)
            {
                std::size_t width = 0;
                // If the vertex has no edges, there is no need to extend
                if(edges.at(it->v).size() > 0)
                {
                    for(auto it2=it+1; it2 != representation.end(); ++it2)
                    {
                        // If the next node could intersect and they don't have a common
                        // edge, don't continue with the extension
                        if(it2->y < it->y && it2->x > it->x && edges.at(it->v).count(it2->v) == 0)
                        {
                            break;
                        }
                        width++;
                    }
                }
                it->width = width;
            }

            // Sort by y coordinates and calculate height
            std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.y < b.y;});
            for(auto it=representation.begin(); it!=representation.end(); ++it)
            {
                std::size_t height = 0;
                // If the vertex has no edges, there is no need to extend
                if(edges.at(it->v).size() > 0)
                {
                    for(auto it2=it+1; it2 != representation.end(); ++it2)
                    {
                        // If the next node's L would intersect and they don't have a common
                        // edge, don't continue with the extension
                        if(it2->y < it->y && it2->x < it->x && (it2->x + it2->width) >= it->x && edges.at(it->v).count(it2->v) == 0)
                        {
                            break;
                        }
                        height++;
                    }
                }
                it->height = height;
            }


            viable = isRepresentationViable(edges);

        }
        while(!viable && std::next_permutation(vertices_y.begin(), vertices_y.end()));
    }
    while(!viable && std::next_permutation(vertices_x.begin(), vertices_x.end()));

    return viable;
}

bool LGraphRepresentation::isRepresentationViable(std::map<vertex, std::set<vertex>> edges)
{
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.v < b.v;});
    for(auto it=representation.begin(); it != representation.end(); ++it)
    {
        for(auto it2=it+1; it2 != representation.end(); ++it2)
        {
            if((LNode::intersection(*it, *it2) && edges.at(it->v).count(it2->v) == 0) || (!LNode::intersection(*it, *it2) && edges.at(it->v).count(it2->v) > 0))
            {
                return false;
            }
        }
    }

    return true;
}

void LGraphRepresentation::draw(QGraphicsView& view)
{
    // TODO
}
