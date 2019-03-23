#ifndef LGRAPHREPRESENTATION_H
#define LGRAPHREPRESENTATION_H

#include "include/graph/graphrepresentation.h"

class LGraphRepresentation: public GraphRepresentation
{
public:
    LGraphRepresentation();
    bool generateFromGraph(Graph&) override;
    void draw(QGraphicsView &) override;

private:
    bool generateFromGraphBF(Graph&);
};

#endif // LGRAPHREPRESENTATION_H
