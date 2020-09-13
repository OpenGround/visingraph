#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QProgressDialog>
#include <memory>
#include <queue>
#include "include/graph.h"
#include "include/graph/graphrepresentation.h"
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
    L_GRAPH = 0,
    L_GRAPH_EXT = 1,
    MPT_GRAPH = 2,
    GROUNDED_PURE_2_DIR = 3,
    BIPARTITE = 4
};

enum class CalcState : uint
{
    FAILED = 0,
    SUCCESS = 1,
    IN_PROGRESS = 2
};

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void calculateFrom(Representation, std::vector<vertex>, std::vector<vertex>, Graph&);

signals:
    void abortedCalculation();
    void nextGraph();
    void searchEnded();


public slots:
    void addNode(vertex);
    void deleteNode(vertex);
    void addEdge(vertex, vertex);
    void deleteEdge(vertex, vertex);
    void convert();
    void startCalc(int);
    void tick(int);
    void stopCalc(int);
    void calculationEnded(bool);
    void abortCalc() {emit abortedCalculation(); aborted=true;}
    void saveLGraphCalculationState(std::vector<vertex>, std::vector<vertex>);
    void loadCalculationState();

    void startSearch();
    void calculationEnded(int, bool);
    void chooseFile();
    void generateNextGraph();
    void searchEnd();


private:
    void initChoices();
    void initConnections();

    Ui::MainWidget *ui;
    Graph currentGraph;
    QProgressDialog *progress;
    std::unique_ptr<GraphRepresentationManager> representation;

    void prepareGraphQueue();
    bool loadGraphFromQueue();
    CalcState calcstatus[3];
    std::unique_ptr<GraphRepresentationManager> positive1, positive2, negative;
    std::queue<std::string> graphQueue;
    bool aborted=false;

};

#endif // MAINWIDGET_H
