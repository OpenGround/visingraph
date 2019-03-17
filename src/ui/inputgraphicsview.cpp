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
    auto intersecting_node = std::find_if(nodes.begin(), nodes.end(), [&event](GraphicsNode i){return i.first->contains(event->localPos());});
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

    QGraphicsEllipseItem* node;

    if(nodes.size() == 0)
    {
        node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
        nodes.push_back(std::make_pair(node, (std::size_t)0));
        scene.addItem(nodes[0].first);
        emit nodeAdded(0);
    }
    else if(nodes[nodes.size()-1].second == nodes.size() - 1)
    {
        std::size_t id = nodes.size();
        node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
        nodes.push_back(std::make_pair(node, id));
        scene.addItem(nodes[nodes.size()-1].first);
        emit nodeAdded(id);
    }
    else
    {
        for(std::size_t i = 0; i < nodes.size(); i++)
        {
            if(nodes[i].second > i)
            {
                node = new QGraphicsEllipseItem(x, y, 2*NODE_RADIUS, 2*NODE_RADIUS);
                nodes.insert(nodes.begin() + i, std::make_pair(node, i));
                scene.addItem(nodes[i].first);
                emit nodeAdded(i);
                break;
            }
        }
    }
}

void InputGraphicsView::addEdge(GraphicsNode* node1,  GraphicsNode* node2)
{
    qreal x1 = node1->first->rect().x() + NODE_RADIUS;
    qreal y1 = node1->first->rect().y() + NODE_RADIUS;
    qreal x2 = node2->first->rect().x() + NODE_RADIUS;
    qreal y2 = node2->first->rect().y() + NODE_RADIUS;

    QGraphicsLineItem* edge = new QGraphicsLineItem(x1,y1,x2,y2);
    edge->setPen(QPen(QBrush(Qt::black), 1));
    edges.push_back(std::make_tuple(edge, node1->second, node2->second));

    scene.addItem(std::get<0>(edges[edges.size()-1]));

    auto db = scene.items();

    emit edgeAdded(node1->second, node2->second);
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
    std::size_t first = it->second;
    scene.removeItem(it->first);

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
    node->first->setPen(QPen(Qt::red));
}

void InputGraphicsView::dehighlightNode()
{
    if(highlightedNode != nullptr)
    {
        highlightedNode->first->setPen(QPen(Qt::black));
        highlightedNode = nullptr;
    }
}
