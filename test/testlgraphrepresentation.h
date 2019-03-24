#ifndef TESTLGRAPHREPRESENTATION_H
#define TESTLGRAPHREPRESENTATION_H

#include <QObject>
#include <QtTest/QTest>

class TestLGraphRepresentation : public QObject
{
    Q_OBJECT
public:
    explicit TestLGraphRepresentation(QObject *parent = nullptr);

private slots:
    void testGenerateI4Representation();
    void testGenerateK4Representation();
    void testGenerateC4Representation();
};

#endif // TESTLGRAPHREPRESENTATION_H
