#include "include/mainwidget.h"
#include "ui_mainwidget.h"

#include "include/graph/lgraphrepresentation.h"

#include <QMessageBox>

#include <iostream>


MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    progress = nullptr;

    initChoices();
    initConnections();
}

void MainWidget::initChoices()
{
    ui->graphRepresentationComboBox->addItem(tr("L-Graph"), static_cast<int>(Representation::L_GRAPH));
}

void MainWidget::initConnections()
{
    connect(ui->conversionButton, SIGNAL(clicked()), this, SLOT(convert()));

    connect(ui->inputGraphicsView, SIGNAL(nodeAdded(vertex)), this, SLOT(addNode(vertex)));
    connect(ui->inputGraphicsView, SIGNAL(nodeDeleted(vertex)), this, SLOT(deleteNode(vertex)));
    connect(ui->inputGraphicsView, SIGNAL(edgeAdded(vertex, vertex)), this, SLOT(addEdge(vertex, vertex)));
    connect(ui->inputGraphicsView, SIGNAL(edgeDeleted(vertex, vertex)), this, SLOT(deleteEdge(vertex, vertex)));
}

void MainWidget::convert()
{
    Representation repr = static_cast<Representation>(ui->graphRepresentationComboBox->currentData().toInt());

    switch (repr) {
    case Representation::L_GRAPH:
    LGraphRepresentation representation;
    connect(&representation, SIGNAL(calculationStarted(int)), this, SLOT(startCalc(int)));
    connect(&representation, SIGNAL(calculationTick()), this, SLOT(tick()));
    connect(&representation, SIGNAL(calculationFinished(int)), this, SLOT(stopCalc(int)));
    connect(this, SIGNAL(abortedCalculation()), &representation, SLOT(stopCalculation()));
    if (representation.generateFromGraph(currentGraph))
    {
        representation.draw(*(ui->outputGraphicsView));
    }
    else
    {
        QMessageBox::warning(this, tr("No representation exists"), tr("No representation was found for this graph"));
    }

    break;
    };
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

void MainWidget::startCalc(int maxTicks)
{
    if(progress != nullptr)
        delete progress;

    progress = new QProgressDialog("", "Cancel", 0, maxTicks, this);
    connect(progress, SIGNAL(canceled()), this, SLOT(abortCalc()));
    progress->setValue(0);
    QApplication::processEvents();
}

void MainWidget::tick()
{
    if(progress == nullptr)
        return;

    progress->setValue(progress->value()+1);
    QApplication::processEvents();
}

void MainWidget::stopCalc(int maxTicks)
{
    if(progress == nullptr)
        return;

    progress->setValue(maxTicks);
    delete progress;
    progress = nullptr;
}

MainWidget::~MainWidget()
{
    delete ui;
}
