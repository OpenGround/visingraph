#include "testutils.h"
#include "include/graph/lgraphrepresentation.h"

TestUtils::TestUtils(QObject *parent) : QObject(parent)
{

}



void TestUtils::testFactorial()
{
    QFETCH(std::size_t, n);
    QFETCH(std::size_t, result);
    QCOMPARE(factorial(n), result);
}

void TestUtils::testFactorial_data()
{
    QTest::addColumn<std::size_t>("n");
    QTest::addColumn<std::size_t>("result");

    QTest::newRow("1!") << static_cast<std::size_t>(1) << static_cast<std::size_t>(1);
    QTest::newRow("5!") << static_cast<std::size_t>(5) << static_cast<std::size_t>(120);
    QTest::newRow("0!") << static_cast<std::size_t>(0) << static_cast<std::size_t>(1);
}

void TestUtils::testRankPermutation()
{
    QFETCH(std::vector<vertex>, permutation);
    QFETCH(std::size_t, result);
    QCOMPARE(rankPermutation(permutation, 0, permutation.size()), result);
}

void TestUtils::testRankPermutation_data()
{
    QTest::addColumn<std::vector<vertex>>("permutation");
    QTest::addColumn<std::size_t>("result");

    QTest::newRow("0,1") << std::vector<vertex>({0, 1}) << static_cast<std::size_t>(0);
    QTest::newRow("0,1,2,3") << std::vector<vertex>({0, 1, 2, 3}) << static_cast<std::size_t>(0);
    QTest::newRow("3,2,1,0") << std::vector<vertex>({3, 2, 1, 0}) << static_cast<std::size_t>(23);
    QTest::newRow("0,1,3,2") << std::vector<vertex>({0, 1, 3, 2}) << static_cast<std::size_t>(1);
}
