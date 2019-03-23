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

    do
    {
        do
        {




        }
        while(std::next_permutation(vertices_y.begin(), vertices_y.end()));
    }
    while(std::next_permutation(vertices_x.begin(), vertices_x.end()));

    return true;
}

void LGraphRepresentation::draw(QGraphicsView& view)
{
    // TODO
}
