#include "include/ui/inputgraphicsview.h"

#include <algorithm>

InputGraphicsView::InputGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setScene(&scene);
}

void InputGraphicsView::resizeEvent(QResizeEvent* event)
{
    setSceneRect(0,0,width(),height());

    QGraphicsView::resizeEvent(event);
}


void InputGraphicsView::mousePressEvent(QMouseEvent *event)
{
    auto intersecting_node = std::find_if(nodes.begin(), nodes.end(), [&event](GraphicsNode i){return std::get<0>(i)->contains(event->localPos())||std::get<1>(i)->contains(event->localPos());});
    auto intersecting_edge = std::find_if(edges.begin(), edges.end(), [&event](GraphicsEdge i){return std::get<0>(i)->contains(event->localPos());});

    bool leftButton = event->button() == Qt::LeftButton;
    bool rightButton = event->button() == Qt::RightButton;

    if(intersecting_edge == edges.end() && intersecting_node == nodes.end())
    {
        if(!hasHighlightedNode() && leftButton)
            addNode(event->localPos());
        else
            dehighlightNode();
    }
    else if(intersecting_edge == edges.end())
    {
        if(rightButton)
        {
            dehighlightNode();
            deleteNode(intersecting_node);
            return;
        }

        if(highlightedNode == nullptr)
        {
            highlightNode(&(*intersecting_node));
        }
        else if(highlightedNode != &(*intersecting_node))
        {
            addEdge(highlightedNode, &(*intersecting_node));
            dehighlightNode();
        }
    }
    else if(intersecting_node == nodes.end() && rightButton)
    {
        deleteEdge(intersecting_edge);
        dehighlightNode();
    }

    scene.update();

    QGraphicsView::mousePressEvent(event);
}


void InputGraphicsView::addNode(QPointF coords)
{
    qreal x = coords.x() - NODE_RADIUS;
    qreal y = coords.y() - NODE_RADIUS;
    qreal textx = x + 0.5 * NODE_RADIUS;
    qreal texty = y;

    QGraphicsEllipseItem* node;
    QGraphicsSimpleTextItem* text;

    if(nodes.size() == 0)
    {
        node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
        node->setBrush(QBrush(QColor("white")));
        text = new QGraphicsSimpleTextItem(QString::number(0));
        text->setX(textx);
        text->setY(texty);

        nodes.push_back(std::make_tuple(node, text, static_cast<std::size_t>(0)));
        scene.addItem(std::get<0>(nodes[0]));
        scene.addItem(std::get<1>(nodes[0]));
        emit nodeAdded(0);
    }
    else if(std::get<2>(nodes[nodes.size()-1]) == nodes.size() - 1)
    {
        std::size_t id = nodes.size();
        node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
        node->setBrush(QBrush(QColor("white")));
        text = new QGraphicsSimpleTextItem(QString::number(id));
        text->setX(textx);
        text->setY(texty);

        nodes.push_back(std::make_tuple(node, text, id));
        scene.addItem(std::get<0>(nodes[nodes.size()-1]));
        scene.addItem(std::get<1>(nodes[nodes.size()-1]));
        emit nodeAdded(id);
    }
    else
    {
        for(std::size_t i = 0; i < nodes.size(); i++)
        {
            if(std::get<2>(nodes[i]) > i)
            {
                node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
                node->setBrush(QBrush(QColor("white")));
                text = new QGraphicsSimpleTextItem(QString::number(i));
                text->setX(textx);
                text->setY(texty);

                nodes.insert(nodes.begin() + i, std::make_tuple(node, text, i));
                scene.addItem(std::get<0>(nodes[i]));
                scene.addItem(std::get<1>(nodes[i]));
                emit nodeAdded(i);
                break;
            }
        }
    }
}

void InputGraphicsView::addEdge(GraphicsNode* node1,  GraphicsNode* node2)
{
    qreal x1 = std::get<0>(*node1)->rect().x() + NODE_RADIUS;
    qreal y1 = std::get<0>(*node1)->rect().y() + NODE_RADIUS;
    qreal x2 = std::get<0>(*node2)->rect().x() + NODE_RADIUS;
    qreal y2 = std::get<0>(*node2)->rect().y() + NODE_RADIUS;

    QGraphicsLineItem* edge = new QGraphicsLineItem(x1,y1,x2,y2);
    edge->setPen(QPen(QBrush(Qt::black), 1));
    // Edges are drawn below the vertices so that they don't intersect with the node's displayed id
    edge->setZValue(-1);
    edges.push_back(std::make_tuple(edge, std::get<2>(*node1), std::get<2>(*node2)));

    scene.addItem(std::get<0>(edges[edges.size()-1]));

    auto db = scene.items();

    emit edgeAdded(std::get<2>(*node1), std::get<2>(*node2));
}

void InputGraphicsView::deleteEdge(std::vector<GraphicsEdge>::iterator it)
{
    std::size_t first = std::get<1>(*it), second = std::get<2>(*it);
    scene.removeItem(std::get<0>(*it));
    edges.erase(it);
    emit edgeDeleted(first, second);
}

void InputGraphicsView::deleteNode(std::vector<GraphicsNode>::iterator it)
{
    std::size_t first = std::get<2>(*it);
    scene.removeItem(std::get<0>(*it));
    scene.removeItem(std::get<1>(*it));

    for(std::size_t i = 0; i < edges.size(); ++i)
    {
        if(std::get<1>(edges[i]) == first || std::get<2>(edges[i]) == first)
        {
            deleteEdge(edges.begin() + i);
            i--;
        }
    }
    nodes.erase(it);
    emit nodeDeleted(first);
}

void InputGraphicsView::highlightNode(GraphicsNode* node)
{
    dehighlightNode();
    highlightedNode = node;
    std::get<0>(*node)->setPen(QPen(Qt::red));
}

void InputGraphicsView::dehighlightNode()
{
    if(highlightedNode != nullptr)
    {
        std::get<0>(*highlightedNode)->setPen(QPen(Qt::black));
        highlightedNode = nullptr;
    }
}
