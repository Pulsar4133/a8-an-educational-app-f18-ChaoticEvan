#ifndef TESTECONENGINE_H
#define TESTECONENGINE_H

#include <QtTest/QtTest>

class TestEconEngine:public QObject
{
    Q_OBJECT
public:
    TestEconEngine();
private slots:
    void testCalculateProfit();
    void testCostOfLemonade();
    void testCalculateDemand();
    void testSetWeatherPattern();
    void testOnNewDayRecipe();
};

#endif // TESTECONENGINE_H
