#ifndef MPTGRAPHREPRESENTATION_H
#define MPTGRAPHREPRESENTATION_H

#include <QObject>
#include "include/graph/graphrepresentation.h"
#include "include/graph/lgraphrepresentation.h"

class MPTGraphRepresentation : public GraphRepresentation
{
    Q_OBJECT
public:
    MPTGraphRepresentation();
    ~MPTGraphRepresentation() override {}
    void checkPermutation(std::vector<vertex>& vertices_x, std::map<vertex, std::set<vertex>>& edges);
    bool isRepresentationViable(std::map<vertex, std::set<vertex>> edges);
    void draw(QGraphicsView& view) override;

private:
    std::vector<LNode> representation;
};


class MPTGraphRepresentationManager : public GraphRepresentationManager
{
    Q_OBJECT
public:
    MPTGraphRepresentationManager();
    ~MPTGraphRepresentationManager() override {}
    void generateFromGraph(Graph&) override;
    void draw(QGraphicsView &view) override;

public slots:
    void stopCalculation() override { aborted = true; }

private:
    MPTGraphRepresentation representation;
    bool checkPermutation(std::vector<vertex>&, Graph&);
    void generateFromGraphBF(Graph&);
    bool aborted = false;
    bool viable = false;
    std::size_t stopped = 0;
    std::size_t foundSolution = 0;
    std::size_t permutations, permutationsPerTick, currentPermutation, counter;
    std::map<vertex, std::set<vertex>> edges;
    std::vector<vertex> xVertices;
    int ticks;

};


#endif // MPTGRAPHREPRESENTATION_H
