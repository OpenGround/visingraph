#ifndef OUTPUTGRAPHICSVIEW_H
#define OUTPUTGRAPHICSVIEW_H

#include <QGraphicsView>

class OutputGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    OutputGraphicsView(QWidget*);

protected:
    void resizeEvent(QResizeEvent *event);

private: // members
    QGraphicsScene scene;
};



#endif // OUTPUTGRAPHICSVIEW_H
