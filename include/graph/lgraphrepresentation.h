#ifndef LGRAPHREPRESENTATION_H
#define LGRAPHREPRESENTATION_H

#include "include/graph/graphrepresentation.h"

struct LNode
{
    // The corresponding vertex
    vertex v;

    // The coordinates of the L-midpoint on the (x,y)-grid
    // It is expected no two LNodes share the same x-coordinate or the same y-coordinate
    std::size_t x,y;

    // The height and width of the L-shape
    std::size_t height, width;

    static bool singleIntersection(LNode& up, LNode& right)
    {
        return right.x < up.x && up.x <= right.x + right.width && up.y < right.y && right.y <= up.y + up.height;
    }

    static bool intersection(LNode& l, LNode r)
    {
        return singleIntersection(l,r) || singleIntersection(r, l);
    }
};

class LGraphRepresentation: public GraphRepresentation
{
public:
    LGraphRepresentation();
    bool generateFromGraph(Graph&) override;
    void draw(QGraphicsView &) override;

private:
    bool generateFromGraphBF(Graph&);
    bool isRepresentationViable(std::map<vertex, std::set<vertex>> edges);
    std::vector<LNode> representation;


};

#endif // LGRAPHREPRESENTATION_H
