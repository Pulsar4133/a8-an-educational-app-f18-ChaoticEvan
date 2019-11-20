#include "testeconengine.h"
#include "econengine.h"

TestEconEngine::TestEconEngine() : QObject()
{
}
    EconEngine* model = new EconEngine();
    void testCalculateProfit(){
        QCOMPARE(model -> calculateProfit(20.0,100.0), 80.0);
    }
    void testCostOfLemonade(){
        QCOMPARE(model -> totalCostOfLemonade(),0);
    }
    void testCalculateDemand(){
        QCOMPARE(model -> calculateDemand(), 100);
    }
