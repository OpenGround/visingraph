#include "include/testposet.h"
#include "include/poset.h"

#include <iostream>

TestPoset::TestPoset(QObject *parent) : QObject(parent)
{

}


void TestPoset::test_constructor()
{
    Poset p(5);

    QCOMPARE(p.lte(2,4), false);
    QCOMPARE(p.lte(2,5), false);
}

void TestPoset::test_add_inequality()
{
    Poset p(3);

    QCOMPARE(p.lte(1, 2), false);
    p.addInequality(1, 2);
    QCOMPARE(p.lte(1, 2), true);
    p.addInequality(1, 2);
    QCOMPARE(p.lte(1, 2), true);
}


void TestPoset::test_inferred_transitivity()
{
    Poset p(5);
    QCOMPARE(p.lte(1,3), false);
    p.addInequality(1,2);
    p.addInequality(2,3);
    QCOMPARE(p.lte(1,3), true);
    p.addInequality(1,0);
    QCOMPARE(p.lte(1,0), true);
    QCOMPARE(p.lte(2,0), false);
    QCOMPARE(p.lte(3,0), false);
}

void TestPoset::test_linear_extension_generation()
{
    Poset p(5); // independent set
    std::queue<std::vector<std::size_t>> q;
    std::vector<std::size_t> v = {0,1,2,3,4};
    p.findAllLinearExtensions(v, q);
    std::size_t count = 0;
    while(!q.empty())
    {
        count++;
        q.pop();
    }
    QCOMPARE(count, 120);

    Poset p2(5); // total order
    for(std::size_t i=0; i<4; i++)
    {
        p2.addInequality(i, i+1);
    }
    v = {0,1,2,3,4};
    p2.findAllLinearExtensions(v, q);
    count = 0;
    while(!q.empty())
    {
        count++;
        q.pop();
    }
    QCOMPARE(count, 1);
}
