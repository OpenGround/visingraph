#ifndef GRAPHREPRESENTATION_H
#define GRAPHREPRESENTATION_H

#include "include/graph.h"
#include <QGraphicsView>
#include <QObject>

class GraphRepresentation : public QObject
{
    Q_OBJECT
public:
    GraphRepresentation();
    virtual ~GraphRepresentation() {}
    virtual void draw(QGraphicsView&) = 0;

};

class GraphRepresentationManager : public QObject
{
    Q_OBJECT
public:
    GraphRepresentationManager();
    virtual ~GraphRepresentationManager() {}
    virtual bool generateFromGraph(Graph&) = 0;
    virtual void draw(QGraphicsView&) = 0;

signals:
    void graphTooBig();
    void calculationStarted(int maxTicks);
    void calculationTick(int ticksElapsed);
    void calculationFinished(int maxTicks);
    void calculationEnded(bool status);
    void calculationEnded(int calcId, bool status);

public slots:
    virtual void stopCalculation() = 0;
    void calculationIdEnd(bool);

protected:
    int calculationId = 4;

};

#endif // GRAPHREPRESENTATION_H
