#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <QObject>
#include <QtTest/QTest>

class TestGraph : public QObject
{
    Q_OBJECT
public:
    explicit TestGraph(QObject *parent = nullptr);

signals:

private slots:
    void test_add_vertex();
    void test_remove_vertex();
    void test_add_remove_vertex();
};

#endif // TESTGRAPH_H
