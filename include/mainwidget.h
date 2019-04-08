#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QProgressDialog>
#include "include/graph.h"
/*!
    \class MainWidget
    \brief The MainWidget class represents the main widget for the application.

    It consists of three main parts: the input box for "painting" the graph,
    the output box for the correct representation and a "list" of settings
    for the graphical representation.

 */

namespace Ui {
class MainWidget;
}

enum class Representation : uint
{
    L_GRAPH = 0
};

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void abortedCalculation();

public slots:
    void addNode(vertex);
    void deleteNode(vertex);
    void addEdge(vertex, vertex);
    void deleteEdge(vertex, vertex);
    void convert();
    void startCalc(int);
    void tick();
    void stopCalc(int);
    void abortCalc() {emit abortedCalculation();}

private:
    void initChoices();
    void initConnections();
    Ui::MainWidget *ui;
    Graph currentGraph;
    QProgressDialog *progress;
};

#endif // MAINWIDGET_H
