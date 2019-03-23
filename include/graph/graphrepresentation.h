#ifndef GRAPHREPRESENTATION_H
#define GRAPHREPRESENTATION_H

#include "include/graph.h"
#include <QGraphicsView>

class GraphRepresentation
{
public:
    GraphRepresentation();
    virtual ~GraphRepresentation() {}
    virtual bool generateFromGraph(Graph&) = 0;
    virtual void draw(QGraphicsView&) = 0;
};

#endif // GRAPHREPRESENTATION_H
