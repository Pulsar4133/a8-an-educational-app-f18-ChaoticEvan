/**
  * This class is mimicing a "model".
  */
#include "econengine.h"

EconEngine::EconEngine(QObject *parent) : QObject(parent)
{

}

void EconEngine::onNewDay(LemonadeStats newLemonadeStats)
{
    // Stores the new lemonade stats in the model.
    this->currentLemonadeStats = newLemonadeStats;

    this->runSimulation();

    emit this->sigSimulationComplete(this->currentDayStats);
}

void EconEngine::runSimulation()
{

    // Reset current day defaults
    this->currentDayStats.reset();

    // TODO: Recalculate ideal lemonade stats,
    //		 e.g. different ice cubes based on
    //		 temperature or something.
    this->perfectLemonadeStats = this->perfectLemonadeStats;

    // TODO: Recalculate weights of internal structures based
    //		 on any conditions that may have changed.
    this->standStats   = this->standStats;
    this->worldStats   = this->worldStats;
    this->upgradeStats = this->upgradeStats;

    // Calculates demand based on internal states
    int cupsDemanded = this->calculateDemand();

    // Set the number of cups sold to the number demanded
    int cupsMade = this->currentLemonadeStats.pitchers * this->standStats.cupsPerPitcher;
    int cupsSold = cupsDemanded;

    // If more cups were demanded than made,
    // cap the demand to number made and flag
    // that the player sold out.
    if (cupsDemanded > cupsMade)
    {
        cupsSold = cupsMade;
        this->currentDayStats.soldOut = true;
    }

    this->currentDayStats.sales = cupsSold;
    this->currentDayStats.demanded = cupsDemanded;
    this->currentDayStats.income = cupsSold * this->currentLemonadeStats.pricePerCup;

    emit sigCost();
    // TODO change placeholder of 0 to the actual cost.
    calculateProfit(0,currentDayStats.income);

    // TODO: Calculate profit.

    return;
}

float EconEngine::calculateProfit(float cost, float income){
    return income - cost;
}

int EconEngine::calculateDemand()
{
    // TODO: Calculate the demand according to the weights and values of all internal variables.

    return 100;
}
