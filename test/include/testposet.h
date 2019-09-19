#ifndef TESTPOSET_H
#define TESTPOSET_H

#include <QObject>
#include <QtTest/QTest>

class TestPoset : public QObject
{
    Q_OBJECT
public:
    explicit TestPoset(QObject *parent = nullptr);

private slots:
    void test_constructor();
    void test_add_inequality();
    void test_inferred_transitivity();
    void test_linear_extension_generation();
};

#endif // TESTPOSET_H
