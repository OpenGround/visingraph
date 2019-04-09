#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QObject>
#include <QtTest/QTest>

class TestUtils : public QObject
{
    Q_OBJECT
public:
    explicit TestUtils(QObject *parent = nullptr);

signals:

private slots:
    void testFactorial();
    void testFactorial_data();
    void testRankPermutation();
    void testRankPermutation_data();
};



#endif // TESTUTILS_H
