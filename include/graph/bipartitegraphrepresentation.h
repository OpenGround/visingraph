#ifndef BIPARTITEGRAPHREPRESENTATION_H
#define BIPARTITEGRAPHREPRESENTATION_H

#include <QObject>
#include "include/graph/graphrepresentation.h"

class BipartiteGraphRepresentation : public GraphRepresentation
{
    Q_OBJECT
public:
    BipartiteGraphRepresentation();
    ~BipartiteGraphRepresentation() override {}
    void draw(QGraphicsView& view) override;

};


class BipartiteGraphRepresentationManager : public GraphRepresentationManager
{
    Q_OBJECT
public:
    BipartiteGraphRepresentationManager();
    ~BipartiteGraphRepresentationManager() override {}
    bool generateFromGraph(Graph&) override;
    void draw(QGraphicsView &view) override;

public slots:
    void stopCalculation() override { aborted = true; }

private:
    BipartiteGraphRepresentation representation;
    bool checkPermutation(std::vector<vertex>&, Graph&);
    bool checkBipartiteGraph(Graph&);
    bool aborted = false;
    std::map<vertex, std::set<vertex>> edges;
    std::vector<vertex> vertices;

};

#endif // BIPARTITEGRAPHREPRESENTATION_H
