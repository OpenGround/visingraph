#ifndef LGRAPHREPRESENTATION_H
#define LGRAPHREPRESENTATION_H

#include "include/graph/graphrepresentation.h"

class LGraphRepresentation: public GraphRepresentation
{
public:
    LGraphRepresentation();
    bool generate_from_graph(Graph&) override;
    void draw(QGraphicsView &) override;

private:
    bool generate_from_graph_BF(Graph&);
};

#endif // LGRAPHREPRESENTATION_H
