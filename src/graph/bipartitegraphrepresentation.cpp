#include "include/graph/bipartitegraphrepresentation.h"
#include "include/poset.h"

#include <QCoreApplication>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>
#include <QProgressDialog>

BipartiteGraphRepresentationManager::BipartiteGraphRepresentationManager()
{

}

bool BipartiteGraphRepresentationManager::generateFromGraph(Graph& graph)
{

    return checkBipartiteGraph(graph);
}



void BipartiteGraphRepresentationManager::draw(QGraphicsView& view)
{
    representation.draw(view);
}


bool BipartiteGraphRepresentationManager::checkBipartiteGraph(Graph &g)
{
    std::queue<vertex> q;
    vertices = g.getVertices();
    edges = g.getEdges();

    std::vector<bool> colors(vertices.size(), false), visited(vertices.size(), false);

    q.push(0);
    vertex current;

    while (!q.empty())
    {
        current = q.front();
        if(!visited[current])
        {
            visited[current] = true;
            auto neighbours = edges.at(vertices[current]);
            for(auto vx: neighbours)
            {
                vertex index = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), vx));
                if (visited[index] && colors[index] == colors[current])
                {
                    emit calculationEnded(false);
                    return false;
                }
                colors[index] = !colors[current];
                q.push(index);
            }

        }

        q.pop();
    }

    emit calculationEnded(true);
    return true;
}


BipartiteGraphRepresentation::BipartiteGraphRepresentation()
{

}


void BipartiteGraphRepresentation::draw(QGraphicsView& view)
{
    // TODO: there's currently no reason to draw bipartite graphs.
}
