#ifndef LGRAPHREPRESENTATION_H
#define LGRAPHREPRESENTATION_H

#include "include/graph/graphrepresentation.h"

std::size_t factorial(std::size_t);
std::size_t rankPermutation(std::vector<vertex>, std::size_t, std::size_t);

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
    Q_OBJECT
public:
    LGraphRepresentation();
    bool generateFromGraph(Graph&) override;
    bool generateFromGraphStateBF(Graph&, std::vector<vertex>, std::vector<vertex>);
    void draw(QGraphicsView &) override;

public slots:
    void stopCalculation() override { aborted = true; }

signals:
    void saveState(std::vector<vertex>, std::vector<vertex>);

private:
    bool generateFromGraphBF(Graph&);
    void checkBFPermutation(std::vector<vertex>&, std::vector<vertex>&, std::map<vertex, std::set<vertex>>);
    bool isRepresentationViable(std::map<vertex, std::set<vertex>> edges);
    std::vector<LNode> representation;
    bool aborted = false;


};

#endif // LGRAPHREPRESENTATION_H
