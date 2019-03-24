#include "include/ui/outputgraphicsview.h"

OutputGraphicsView::OutputGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setScene(&scene);
}

void OutputGraphicsView::resizeEvent(QResizeEvent* event)
{
    setSceneRect(0, 0, size().width(), size().height());

    QGraphicsView::resizeEvent(event);
}
