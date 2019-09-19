#include "include/mainwidget.h"
#include "ui_mainwidget.h"

#include "include/graph/lgraphrepresentation.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>

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
    ui->graphRepresentationComboBox->addItem(tr("L-Graph via extensions"), static_cast<int>(Representation::L_GRAPH_EXT));

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

    switch (repr)
    {
    case Representation::L_GRAPH:
        representation = std::unique_ptr<LGraphRepresentationManager>(new LGraphRepresentationManager());
        connect(representation.get(), SIGNAL(saveState(std::vector<vertex>, std::vector<vertex>)), this, SLOT(saveLGraphCalculationState(std::vector<vertex>, std::vector<vertex>)));
        break;
    case Representation::L_GRAPH_EXT:
        representation = std::unique_ptr<LGraphExtRepresentationManager>(new LGraphExtRepresentationManager());
        break;
    };
    connect(representation.get(), SIGNAL(calculationStarted(int)), this, SLOT(startCalc(int)));
    connect(representation.get(), SIGNAL(calculationTick(int)), this, SLOT(tick(int)));
    connect(representation.get(), SIGNAL(calculationFinished(int)), this, SLOT(stopCalc(int)));
    connect(representation.get(), SIGNAL(calculationEnded(bool)), this, SLOT(calculationEnded(bool)));
    connect(this, SIGNAL(abortedCalculation()), representation.get(), SLOT(stopCalculation()));
    representation->generateFromGraph(currentGraph);

}

void MainWidget::calculationEnded(bool status)
{
    if (status)
    {
        representation->draw(*(ui->outputGraphicsView));
    }
    else
    {
        QMessageBox::warning(this, tr("No representation exists"), tr("No representation was found for this graph"));
    }

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

void MainWidget::tick(int ticksElapsed)
{
    if(progress == nullptr)
        return;

    progress->setValue(progress->value()+ticksElapsed);
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

void MainWidget::saveLGraphCalculationState(std::vector<vertex> x_vertices, std::vector<vertex> y_vertices)
{
    QFile out(QFileDialog::getSaveFileName(nullptr, "Enter the filename"));
    if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream os(&out);
    os << "LGraph\n";
    for(auto x: x_vertices)
        os << x << " ";
    os << "\n";
    for(auto y: y_vertices)
        os << y << " ";
    os << "\n";

    auto edges = currentGraph.getEdges();

    std::size_t size = 0;
    for(auto neig : edges)
    {
        size += neig.second.size();
    }
    size /= 2;

    os << size << "\n";
    for(auto neighbours: edges)
    {
        for(auto neighbour: neighbours.second)
        {
            if (neighbour > neighbours.first)
                os << neighbours.first << " " << neighbour << "\n";
        }

    }

    out.close();
}

void MainWidget::loadCalculationState()
{
    QFile in(QFileDialog::getOpenFileName(nullptr, "Enter the filename"));

    if(!in.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString line = in.readLine();
    if(line != "LGraph\n")
        return;

    std::vector<vertex> x_vertices, y_vertices;
    line = in.readLine().split('\n')[0];
    QStringList x = line.split(" ");
    for(std::size_t i = 0; i < x.size()-1; ++i)
    {
        x_vertices.push_back(x[i].toInt());
    }
    line = in.readLine().split('\n')[0];
    QStringList y = line.split(" ");
    for(std::size_t i = 0; i < y.size()-1; ++i)
    {
        y_vertices.push_back(y[i].toInt());
    }
    line = in.readLine().split('\n')[0];
    Graph g;
    for(auto vertex: x_vertices)
    {
        g.addVertex(vertex);
    }
    std::size_t edgeNr = line.toInt();
    for(std::size_t i = 0; i < edgeNr; i++)
    {
        line = in.readLine().split('\n')[0];
        QStringList edge = line.split(" ");
        g.addEdge(edge[0].toInt(), edge[1].toInt());
    }
    calculateFrom(Representation::L_GRAPH, x_vertices, y_vertices, g);
}

void MainWidget::calculateFrom(Representation repr, std::vector<vertex> x_vertices, std::vector<vertex> y_vertices, Graph & g)
{
    if(repr != Representation::L_GRAPH)
        return;

    currentGraph = g;

    representation = std::unique_ptr<LGraphRepresentationManager>(new LGraphRepresentationManager());
    connect(representation.get(), SIGNAL(calculationStarted(int)), this, SLOT(startCalc(int)));
    connect(representation.get(), SIGNAL(calculationTick(int)), this, SLOT(tick(int)));
    connect(representation.get(), SIGNAL(calculationFinished(int)), this, SLOT(stopCalc(int)));
    connect(representation.get(), SIGNAL(saveState(std::vector<vertex>, std::vector<vertex>)), this, SLOT(saveLGraphCalculationState(std::vector<vertex>, std::vector<vertex>)));
    connect(this, SIGNAL(abortedCalculation()), representation.get(), SLOT(stopCalculation()));
    // TODO: this isn't an ideal solution and should be fixed later
    ((LGraphRepresentationManager*)representation.get())->generateFromGraphStateBF(currentGraph, x_vertices, y_vertices);
}

MainWidget::~MainWidget()
{
    delete ui;
}
