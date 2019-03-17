#include "include/mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

void MainWidget::addNode(vertex v)
{
    currentGraph.add_vertex(v);
}

void MainWidget::deleteNode(vertex v)
{
    currentGraph.remove_vertex(v);
}

void MainWidget::addEdge(vertex u, vertex v)
{
    currentGraph.add_edge(u, v);
}

void MainWidget::deleteEdge(vertex u, vertex v)
{
    currentGraph.remove_edge(u, v);
}

MainWidget::~MainWidget()
{
    delete ui;
}
