#ifndef TEST_H
#define TEST_H
#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT
private slots:
    void case1_testcase();
    void case2_testcase();
    void case3_testcase();
    void case4_testcase();
    void case5_testcase();
    void case6_testcase();
    void case7_testcase();
    void case8_testcase();
    void case9_testcase();
    void case10_testcase();


public:
    Test();
};

#endif // SIMPLETEST_H
