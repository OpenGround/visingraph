#include "include/graph/lgraphrepresentation.h"

LGraphRepresentation::LGraphRepresentation()
{

}

bool LGraphRepresentation::generate_from_graph(Graph& graph)
{
    return generate_from_graph_BF(graph);
}

/*!
 * \brief LGraphRepresentation::generate_from_graph_BF
 * Attempt to create graph by brute-forcing all possible
 * (n!)^2 combinations
 * \param graph The graph which is to be represented
 */
bool LGraphRepresentation::generate_from_graph_BF(Graph& graph)
{
    std::vector<vertex> vertices_x = graph.get_vertices();
    std::vector<vertex> vertices_y = graph.get_vertices();
    std::set<std::pair<vertex, vertex>> edges = graph.get_edges();

    do {
        // TODO
    } while(std::next_permutation(vertices_x.begin(), vertices_x.end()));

    return true;
}

void LGraphRepresentation::draw(QGraphicsView& view)
{
    // TODO
}
