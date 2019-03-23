#include "include/mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

void MainWidget::addNode(vertex v)
{
    currentGraph.addVertex(v);
}

void MainWidget::deleteNode(vertex v)
{
    currentGraph.removeVertex(v);
}

void MainWidget::addEdge(vertex u, vertex v)
{
    currentGraph.addEdge(u, v);
}

void MainWidget::deleteEdge(vertex u, vertex v)
{
    currentGraph.removeEdge(u, v);
}

MainWidget::~MainWidget()
{
    delete ui;
}
