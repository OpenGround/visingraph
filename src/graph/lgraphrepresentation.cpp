#include "include/graph/lgraphrepresentation.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QProgressDialog>

std::size_t factorial(std::size_t n)
{
    return n>1 ? n*factorial(n-1) : 1;
}


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
 * (n!)^2 permutations
 * \param graph The graph which is to be represented
 */
bool LGraphRepresentation::generateFromGraphBF(Graph& graph)
{
    std::vector<vertex> vertices_x = graph.getVertices();
    std::vector<vertex> vertices_y = graph.getVertices();
    std::map<vertex, std::set<vertex>> edges = graph.getEdges();

    bool viable = false;

    std::size_t size = vertices_x.size();

    // Large graphs aren't permitted yet
    if(size > 20)
    {
        // 21! > 2^64, although I would estimate that at most 11-12 is feasible
        emit graphTooBig();
        return false;
    }

    std::size_t permutations = factorial(size);
    std::size_t permutationsPerTick = (permutations / INT_MAX) + 1;
    int ticks = static_cast<int>(permutations / permutationsPerTick);
    std::size_t counter = 0;

    aborted = false;

    emit calculationStarted(ticks);

    do
    {
        do
        {
            // Let other events process so that the program doesn't stop responding
            QCoreApplication::processEvents();

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
        while(!viable && !aborted && std::next_permutation(vertices_y.begin(), vertices_y.end()));

        // Update the progress dialog
        counter++;
        if(counter == permutationsPerTick)
        {
            emit calculationTick();
            counter = 0;
        }


    }
    while(!viable && !aborted && std::next_permutation(vertices_x.begin(), vertices_x.end()));

    emit calculationFinished(ticks);

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
    QGraphicsScene* scene = view.scene();
    std::size_t node_nr = representation.size();

    scene->clear();
    QRectF rect = scene->sceneRect();

    // No vertices - there is nothing to draw
    if(node_nr == 0)
        return;


    constexpr qreal BORDER = 10;
    qreal top = rect.top() + BORDER, left = rect.left() + BORDER;
    qreal width = view.size().width() - 2 * BORDER, height = view.size().height() - 2 * BORDER;
    qreal unit_width = width / (node_nr);
    qreal unit_height = height / (node_nr);

    // Draw the L for every node
    for (auto node: representation)
    {
        qreal node_x = node.x * unit_width + left;
        qreal node_y = (node_nr - node.y - 0.25) * unit_height + top;
        qreal node_width = (node.width + 0.75) * unit_width;
        qreal node_height = (node.height + 0.75) * unit_height;

        scene->addLine(node_x, node_y, node_x + node_width, node_y);
        scene->addLine(node_x, node_y - node_height, node_x, node_y);
    }

}
