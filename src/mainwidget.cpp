#include "include/mainwidget.h"
#include "ui_mainwidget.h"

#include "include/graph/lgraphrepresentation.h"
#include "include/graph/mptgraphrepresentation.h"
#include "include/graph/groundedpure2dirgraphrepresentation.h"
#include "include/graph/bipartitegraphrepresentation.h"

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
    ui->graphRepresentationComboBox->addItem(tr("L-graph"), static_cast<int>(Representation::L_GRAPH));
    ui->graphRepresentationComboBox->addItem(tr("L-graph via extensions"), static_cast<int>(Representation::L_GRAPH_EXT));
    ui->graphRepresentationComboBox->addItem(tr("Max-point-tolerance graph"), static_cast<int>(Representation::MPT_GRAPH));
    ui->graphRepresentationComboBox->addItem(tr("Grounded PURE-2-DIR"), static_cast<int>(Representation::GROUNDED_PURE_2_DIR));

    ui->positive1ComboBox->addItem(tr("L-graph via extensions"), static_cast<int>(Representation::L_GRAPH_EXT));
    ui->positive1ComboBox->addItem(tr("Max-point-tolerance graph"), static_cast<int>(Representation::MPT_GRAPH));
    ui->positive1ComboBox->addItem(tr("Grounded PURE-2-DIR"), static_cast<int>(Representation::GROUNDED_PURE_2_DIR));
    ui->positive1ComboBox->addItem(tr("Bipartite graph"), static_cast<int>(Representation::BIPARTITE));

    ui->positive2ComboBox->addItem(tr("L-graph via extensions"), static_cast<int>(Representation::L_GRAPH_EXT));
    ui->positive2ComboBox->addItem(tr("Max-point-tolerance graph"), static_cast<int>(Representation::MPT_GRAPH));
    ui->positive2ComboBox->addItem(tr("Grounded PURE-2-DIR"), static_cast<int>(Representation::GROUNDED_PURE_2_DIR));
    ui->positive2ComboBox->addItem(tr("Bipartite graph"), static_cast<int>(Representation::BIPARTITE));

    ui->negativeComboBox->addItem(tr("L-graph via extensions"), static_cast<int>(Representation::L_GRAPH_EXT));
    ui->negativeComboBox->addItem(tr("Max-point-tolerance graph"), static_cast<int>(Representation::MPT_GRAPH));
    ui->negativeComboBox->addItem(tr("Grounded PURE-2-DIR"), static_cast<int>(Representation::GROUNDED_PURE_2_DIR));
    ui->negativeComboBox->addItem(tr("Bipartite graph"), static_cast<int>(Representation::BIPARTITE));

}

void MainWidget::initConnections()
{
    connect(ui->conversionButton, SIGNAL(clicked()), this, SLOT(convert()));
    connect(ui->startSearchButton, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

    connect(ui->inputGraphicsView, SIGNAL(nodeAdded(vertex)), this, SLOT(addNode(vertex)));
    connect(ui->inputGraphicsView, SIGNAL(nodeDeleted(vertex)), this, SLOT(deleteNode(vertex)));
    connect(ui->inputGraphicsView, SIGNAL(edgeAdded(vertex, vertex)), this, SLOT(addEdge(vertex, vertex)));
    connect(ui->inputGraphicsView, SIGNAL(edgeDeleted(vertex, vertex)), this, SLOT(deleteEdge(vertex, vertex)));

    connect(this, SIGNAL(searchEnded()), this, SLOT(searchEnd()));
    connect(this, SIGNAL(nextGraph()), this, SLOT(generateNextGraph()));
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
    case Representation::MPT_GRAPH:
        representation = std::unique_ptr<MPTGraphRepresentationManager>(new MPTGraphRepresentationManager());
        break;
    case Representation::GROUNDED_PURE_2_DIR:
        representation = std::unique_ptr<GroundedPure2DirGraphRepresentationManager>(new GroundedPure2DirGraphRepresentationManager());
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

void MainWidget::calculationEnded(int checkid, bool status)
{
    calcstatus[checkid] = static_cast<CalcState>(status);

    if (calcstatus[0] == CalcState::SUCCESS && calcstatus[1] == CalcState::SUCCESS && calcstatus[2] == CalcState::FAILED)
    {
        QMessageBox::warning(this, tr("Counterexample found"), QString::fromStdString(currentGraph.getG6()));
    }
    else if(calcstatus[0] != CalcState::IN_PROGRESS && calcstatus[1] != CalcState::IN_PROGRESS && calcstatus[2] != CalcState::IN_PROGRESS)
    {
        emit nextGraph();
    }

}

void MainWidget::chooseFile()
{
     ui->fileLabel->setText(QFileDialog::getOpenFileName(nullptr, "Enter the filename"));
}

void MainWidget::prepareGraphQueue()
{
    // clear the queue
    std::queue<std::string> empty;
    std::swap(graphQueue, empty);

    QFile in(ui->fileLabel->text());

    if (in.open(QIODevice::ReadOnly|QIODevice::Text))
    {
       QTextStream instream(&in);
       while (!instream.atEnd())
       {
          QString line = instream.readLine();
          graphQueue.push(line.toStdString());
       }
       in.close();
    }
}

void MainWidget::generateNextGraph()
{
    std::size_t graphs = graphQueue.size(), remaining = graphs, counter=0;
    std::size_t graphsPerTick = (graphs / INT_MAX) + 1;
    int ticks = static_cast<int>(graphs / graphsPerTick);

    startCalc(ticks);

    while(!aborted && !graphQueue.empty())
    {
        QApplication::processEvents();
        bool loaded = loadGraphFromQueue();

        std::cout << graphQueue.size() << " " << loaded << std::endl;

        for(std::size_t i = 0; i < 3; i++)
        {
            calcstatus[i] = CalcState::IN_PROGRESS;
        }

        if (!loaded)
            break;

        bool status = positive1->generateFromGraph(currentGraph);

        if (status)
        {
            status = positive2->generateFromGraph(currentGraph);

            if (status)
            {
                status = negative->generateFromGraph(currentGraph);

                if (!status)
                {
                    QMessageBox::warning(this, tr("Counterexample found"), QString::fromStdString(currentGraph.getG6()));
                }
            }
        }

        counter += remaining - graphQueue.size();
        remaining -= counter;
        if (counter > graphsPerTick)
        {
            tick(static_cast<int>(counter/graphsPerTick));
            counter = counter % graphsPerTick;
        }

    }

    if(!aborted)
    {
        stopCalc(ticks);
        searchEnd();
    }
    else
    {
        QMessageBox::warning(this, tr("Calculation aborted"), tr("The calculation was aborted"));
    }

}

bool MainWidget::loadGraphFromQueue()
{
    // TODO: There might be a bug with the last graph in the queue.
    while (!graphQueue.empty() && !currentGraph.loadFromGraph6(graphQueue.front()))
    {
        graphQueue.pop();
    }
    if (graphQueue.empty())
    {
        emit searchEnded();
        return false;
    }
    else
    {
        graphQueue.pop();
    }

    return true;
}

void MainWidget::startSearch()
{
    prepareGraphQueue();
    aborted = true;

    // TODO: this has to be doable more efficiently
    Representation repr = static_cast<Representation>(ui->positive1ComboBox->currentData().toInt());

    switch (repr)
    {
    case Representation::L_GRAPH:
        positive1 = std::unique_ptr<LGraphRepresentationManager>(new LGraphRepresentationManager());
        break;
    case Representation::L_GRAPH_EXT:
        positive1 = std::unique_ptr<LGraphExtRepresentationManager>(new LGraphExtRepresentationManager());
        break;
    case Representation::MPT_GRAPH:
        positive1 = std::unique_ptr<MPTGraphRepresentationManager>(new MPTGraphRepresentationManager());
        break;
    case Representation::GROUNDED_PURE_2_DIR:
        positive1 = std::unique_ptr<GroundedPure2DirGraphRepresentationManager>(new GroundedPure2DirGraphRepresentationManager());
        break;
    case Representation::BIPARTITE:
        positive1 = std::unique_ptr<BipartiteGraphRepresentationManager>(new BipartiteGraphRepresentationManager());
        break;
    };
    repr = static_cast<Representation>(ui->positive2ComboBox->currentData().toInt());

    switch (repr)
    {
    case Representation::L_GRAPH:
        positive2 = std::unique_ptr<LGraphRepresentationManager>(new LGraphRepresentationManager());
        break;
    case Representation::L_GRAPH_EXT:
        positive2 = std::unique_ptr<LGraphExtRepresentationManager>(new LGraphExtRepresentationManager());
        break;
    case Representation::MPT_GRAPH:
        positive2 = std::unique_ptr<MPTGraphRepresentationManager>(new MPTGraphRepresentationManager());
        break;
    case Representation::GROUNDED_PURE_2_DIR:
        positive2 = std::unique_ptr<GroundedPure2DirGraphRepresentationManager>(new GroundedPure2DirGraphRepresentationManager());
        break;
    case Representation::BIPARTITE:
        positive2 = std::unique_ptr<BipartiteGraphRepresentationManager>(new BipartiteGraphRepresentationManager());
        break;
    };
    repr = static_cast<Representation>(ui->negativeComboBox->currentData().toInt());

    switch (repr)
    {
    case Representation::L_GRAPH:
        negative = std::unique_ptr<LGraphRepresentationManager>(new LGraphRepresentationManager());
        break;
    case Representation::L_GRAPH_EXT:
        negative = std::unique_ptr<LGraphExtRepresentationManager>(new LGraphExtRepresentationManager());
        break;
    case Representation::MPT_GRAPH:
        negative = std::unique_ptr<MPTGraphRepresentationManager>(new MPTGraphRepresentationManager());
        break;
    case Representation::GROUNDED_PURE_2_DIR:
        negative = std::unique_ptr<GroundedPure2DirGraphRepresentationManager>(new GroundedPure2DirGraphRepresentationManager());
        break;
    case Representation::BIPARTITE:
        negative = std::unique_ptr<BipartiteGraphRepresentationManager>(new BipartiteGraphRepresentationManager());
        break;

    };

    generateNextGraph();
}

void MainWidget::searchEnd()
{
    QMessageBox::warning(this, tr("Counterexample not found"), tr("Try bigger graphs I guess."));
}


MainWidget::~MainWidget()
{
    delete ui;
}
