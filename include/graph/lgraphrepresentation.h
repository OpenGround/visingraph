#ifndef LGRAPHREPRESENTATION_H
#define LGRAPHREPRESENTATION_H

#include "include/graph/graphrepresentation.h"
#include <memory>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

std::size_t factorial(std::size_t);
std::size_t rankPermutation(std::vector<vertex>, std::size_t, std::size_t);

struct LNode
{
    // The corresponding vertex
    vertex v;

    // The coordinates of the L-midpoint on the (x,y)-grid
    // It is expected no two LNodes share the same x-coordinate or the same y-coordinate
    std::size_t x,y;

    // The height and width of the L-shape
    std::size_t height, width;

    static bool singleIntersection(LNode& up, LNode& right)
    {
        return right.x < up.x && up.x <= right.x + right.width && up.y < right.y && right.y <= up.y + up.height;
    }

    static bool intersection(LNode& l, LNode r)
    {
        return singleIntersection(l,r) || singleIntersection(r, l);
    }
};

class LGraphQThread;

class LGraphRepresentationManager : public GraphRepresentationManager
{
    Q_OBJECT
public:
    LGraphRepresentationManager();
    ~LGraphRepresentationManager() override {}
    void generateFromGraph(Graph&) override;
    void generateFromGraphStateBF(Graph&, std::vector<vertex>, std::vector<vertex>);
    void draw(QGraphicsView &view) override;


public slots:
    void stopCalculation() override { aborted = true; }
    void threadFinished(bool status, int id);

signals:
    void saveState(std::vector<vertex>, std::vector<vertex>);

private:
    void generateFromGraphBF(Graph&);
    bool aborted = false;
    bool viable = false;
    std::size_t stopped = 0;
    std::size_t foundSolution = 0;
    std::size_t permutations, permutationsPerTick, currentPermutation, counter;
    std::map<vertex, std::set<vertex>> edges;
    std::vector<vertex> xVertices, yVertices;
    int ticks;
    std::vector<std::unique_ptr<LGraphQThread>> threads;

};


class LGraphRepresentation: public GraphRepresentation
{
    Q_OBJECT
public:
    LGraphRepresentation();
    ~LGraphRepresentation() override {}
    void checkBFPermutation(std::vector<vertex>&, std::vector<vertex>&, std::map<vertex, std::set<vertex>>&);
    bool checkBFPermutations(std::vector<vertex>&, std::vector<vertex>&, std::map<vertex, std::set<vertex>>&);
    bool isRepresentationViable(std::map<vertex, std::set<vertex>> edges);
    void draw(QGraphicsView &) override;

private:
    std::vector<LNode> representation;

};

class LGraphQThread: public QThread
{
    Q_OBJECT
public:
    LGraphQThread(QObject *parent = nullptr);
    ~LGraphQThread() override;


    void check(std::vector<vertex>&, std::vector<vertex>&, std::map<vertex, std::set<vertex>>&, std::size_t);
    void draw(QGraphicsView &view) {repr.draw(view);}

protected:
    void run() override;

signals:
    void finishedCalculation(bool status, int id);

private:
    LGraphRepresentation repr;
    QMutex mutex, reprMutex;
    QWaitCondition condition;
    std::vector<vertex> vertices_x, vertices_y;
    std::map<vertex, std::set<vertex>> edges;
    int id;
    bool abort=false;
};


#endif // LGRAPHREPRESENTATION_H
