#include "include/graph/groundedpure2dirgraphrepresentation.h"
#include "include/poset.h"

#include <QCoreApplication>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>
#include <QProgressDialog>


GroundedPure2DirGraphRepresentationManager::GroundedPure2DirGraphRepresentationManager()
{

}

bool GroundedPure2DirGraphRepresentationManager::generateFromGraph(Graph& graph)
{
    return generateFromGraphBF(graph);
}

/*!
 * \brief GroundedPure2DirGraphRepresentationManager::generate_from_graph_BF
 * Attempt to create the representation by checking all possible
 * n! permutations
 * This can be done in polynomial time using
 * \param graph The graph which is to be represented
 */
bool GroundedPure2DirGraphRepresentationManager::generateFromGraphBF(Graph& graph)
{
    std::vector<vertex> vertices_x = graph.getVertices();

    edges = graph.getEdges();

    viable = false;
    aborted = false;
    stopped = 0;
    xVertices = vertices_x;

    std::size_t size = vertices_x.size();

    // Large graphs aren't permitted yet
    if(size > 20)
    {
        // 21! > 2^64, although I would estimate that at most 11-12 is feasible
        // to be run in a decent timeframe (given the graph is ugly w.r.t. the algorithm)
        emit graphTooBig();
        return false;
    }

    permutations = factorial(size);
    permutationsPerTick = (permutations / INT_MAX) + 1;
    currentPermutation = rankPermutation(vertices_x, 0, size);
    ticks = static_cast<int>(permutations / permutationsPerTick);
    counter = 0;

    emit calculationStarted(ticks);

    // If the calculation has already been started, tick the required number of ticks
    emit calculationTick(static_cast<int>(currentPermutation/permutationsPerTick));

    do {
        // Process events so that the application doesn't stop responding
        QCoreApplication::processEvents();

        if(checkPermutation(vertices_x, graph))
        {
            emit calculationFinished(ticks);
            emit calculationEnded(true);
            return true;
        }

        counter++;
        if(counter == permutationsPerTick)
        {
            emit calculationTick(1);
            counter = 0;
        }
    } while(std::next_permutation(vertices_x.begin(), vertices_x.end()) && !aborted);

    emit calculationFinished(ticks);
    emit calculationEnded(false);

    return false;
}


void GroundedPure2DirGraphRepresentationManager::draw(QGraphicsView& view)
{
    representation.draw(view);
}


bool GroundedPure2DirGraphRepresentationManager::checkPermutation(std::vector<vertex>& vertices_x, Graph &g)
{
    edges = g.getEdges();
    representation.checkPermutation(vertices_x, edges);
    return representation.isRepresentationViable(edges);
}


GroundedPure2DirGraphRepresentation::GroundedPure2DirGraphRepresentation()
{

}


void GroundedPure2DirGraphRepresentation::checkPermutation(std::vector<vertex>& vertices_x, std::map<vertex, std::set<vertex>>& edges)
{
    representation.clear();

    // Create the representation without any height or length at first
    for(std::size_t i = 0; i < vertices_x.size(); ++i)
    {
        LNode n;
        n.v = vertices_x[i];
        n.x = i;
        n.y = vertices_x.size() - i - 1;
        representation.push_back(n);
    }

    // Add the height and length based on the edges in O(n^2*log^2(n))
    // TODO: the runtime of this could be improved rather easily
    // Sort by x coordinates and calculate width by simply extending for
    // as long as possible.
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.x < b.x;});
    for(auto it=representation.begin(); it!=representation.end(); ++it)
    {
        if(it == representation.begin())
        {
            it->width = representation.size()-1;
            continue;
        }

        std::size_t current_width = 0, max_width = 0;
        // If the vertex has no edges, there is no need to extend
        if(edges.at(it->v).size() > 0)
        {
            for(auto it2=it+1; it2 != representation.end(); ++it2)
            {
                current_width++;
                // If the next node could intersect and they don't have a common
                // edge, don't continue with the extension
                if(edges.at(it->v).count(it2->v) > 0)
                {
                    max_width = current_width;
                }

            }
        }
        it->width = max_width;
    }

    // Sort by y coordinates and calculate height
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.y < b.y;});
    for(auto it=representation.begin(); it!=representation.end(); ++it)
    {
        std::size_t max_height = 0, current_height = 0;
        // If the vertex has no edges, there is no need to extend
        if(edges.at(it->v).size() > 0)
        {
            for(auto it2=it+1; it2 != representation.end(); ++it2)
            {
                current_height++;
                // If the next node's L would intersect and they don't have a common
                // edge, extend farther
                if(edges.at(it->v).count(it2->v) > 0)
                {
                    max_height = current_height;
                }
            }
        }
        it->height = max_height;
    }

}

bool GroundedPure2DirGraphRepresentation::isRepresentationViable(std::map<vertex, std::set<vertex>> edges)
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

        // We want a 2-DIR representation despite using MPT as backend
        if(it->width > 0 && it->height > 0)
        {
            return false;
        }

    }

    return true;
}

void GroundedPure2DirGraphRepresentation::draw(QGraphicsView& view)
{
    QGraphicsScene* scene = view.scene();
    std::size_t node_nr = representation.size();

    scene->clear();
    QRectF rect = scene->sceneRect();

    // No vertices - there is nothing to draw
    if(node_nr == 0)
        return;

    constexpr qreal BORDER = 10;
    constexpr qreal FONT_SIZE = 10;
    qreal top = rect.top() + BORDER, left = rect.left() + BORDER;
    qreal width = view.size().width() - 2 * BORDER, height = view.size().height() - 2 * BORDER;
    qreal unit_width = width / (node_nr);
    qreal unit_height = height / (node_nr);

    // Draw the line segment for every node
    for (auto node: representation)
    {
        qreal node_x = node.x * unit_width + left;
        qreal node_y = (node_nr - node.y - 0.25) * unit_height + top;
        qreal node_width = (node.width + 0.75) * unit_width;
        qreal node_height = (node.height + 0.75) * unit_height;

        if (node.width > 0)
        {
            scene->addLine(node_x, node_y, node_x + node_width, node_y);
        }

        if (node.height > 0 || node.width == 0) // The second part of the condition fixes "invisible" line segments with no neighbors
        {
            scene->addLine(node_x, node_y - node_height, node_x, node_y);
        }

        QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(node.v));
        text->setX(node_x+0.5*FONT_SIZE);
        text->setY(node_y-1.5*FONT_SIZE);

    }

}
