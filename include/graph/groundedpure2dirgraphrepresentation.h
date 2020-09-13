#ifndef GROUNDEDPURE2DIRGRAPHREPRESENTATION_H
#define GROUNDEDPURE2DIRGRAPHREPRESENTATION_H

#include <QObject>
#include "include/graph/graphrepresentation.h"
#include "include/graph/lgraphrepresentation.h"

class GroundedPure2DirGraphRepresentation : public GraphRepresentation
{
    Q_OBJECT
public:
    GroundedPure2DirGraphRepresentation();
    ~GroundedPure2DirGraphRepresentation() override {}
    void checkPermutation(std::vector<vertex>& vertices_x, std::map<vertex, std::set<vertex>>& edges);
    bool isRepresentationViable(std::map<vertex, std::set<vertex>> edges);
    void draw(QGraphicsView& view) override;

private:
    std::vector<LNode> representation;
};


class GroundedPure2DirGraphRepresentationManager : public GraphRepresentationManager
{
    Q_OBJECT
public:
    GroundedPure2DirGraphRepresentationManager();
    ~GroundedPure2DirGraphRepresentationManager() override {}
    bool generateFromGraph(Graph&) override;
    void draw(QGraphicsView &view) override;

public slots:
    void stopCalculation() override { aborted = true; }

private:
    GroundedPure2DirGraphRepresentation representation;
    bool checkPermutation(std::vector<vertex>&, Graph&);
    bool generateFromGraphBF(Graph&);
    bool aborted = false;
    bool viable = false;
    std::size_t stopped = 0;
    std::size_t foundSolution = 0;
    std::size_t permutations, permutationsPerTick, currentPermutation, counter;
    std::map<vertex, std::set<vertex>> edges;
    std::vector<vertex> xVertices;
    int ticks;

};


#endif // GROUNDEDPURE2DIRGRAPHREPRESENTATION_H
