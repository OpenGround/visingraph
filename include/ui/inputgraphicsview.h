#ifndef INPUTGRAPHICSVIEW_H
#define INPUTGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>

#include <vector>
#include <utility>

#include "include/graph.h"

using GraphicsEdge = std::tuple<QGraphicsLineItem*, std::size_t, std::size_t>;
using GraphicsNode = std::tuple<QGraphicsEllipseItem*, QGraphicsSimpleTextItem*, std::size_t>;


class InputGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    InputGraphicsView(QWidget*);

protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void nodeAdded(vertex);
    void edgeAdded(vertex, vertex);
    void nodeDeleted(vertex);
    void edgeDeleted(vertex, vertex);

private: // methods
    void addNode(QPointF);
    void deleteNode(std::vector<GraphicsNode>::iterator);
    void addEdge(GraphicsNode*, GraphicsNode*);
    void deleteEdge(std::vector<GraphicsEdge>::iterator);
    void highlightNode(GraphicsNode*);
    void dehighlightNode();
    bool hasHighlightedNode() {return highlightedNode != nullptr;}


private: // members
    std::vector<GraphicsNode> nodes;
    std::vector<GraphicsEdge> edges;
    QGraphicsScene scene;
    GraphicsNode *highlightedNode = nullptr;

    // The radius of displayed nodes
    static constexpr qreal NODE_RADIUS = 10;

    // As it's difficult to click precisely on an edge, there is a small radius
    // around the edge in which the clicks count as if the user clicked on the edge
    static constexpr qreal EDGE_RANGE = 3;

};

#endif // INPUTGRAPHICSVIEW_H
