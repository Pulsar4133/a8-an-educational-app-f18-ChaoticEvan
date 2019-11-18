#include "econengine.h"

EconEngine::EconEngine(QObject *parent) : QObject(parent)
{

}

void EconEngine::onNewDay(LemonadeStats newLemonadeStats)
{
    // Stores the new lemonade stats in the model.
    this->_currentLemonadeStats = newLemonadeStats;

    this->runSimulation();

    emit this->sigSimulationComplete(this->_currentDayStats);
}

void EconEngine::runSimulation()
{

    // Reset current day defaults
    this->_currentDayStats.reset();

    // TODO: Recalculate ideal lemonade stats,
    //		 e.g. different ice cubes based on
    //		 temperature or something.
    this->_perfectLemonadeStats = this->_perfectLemonadeStats;

    // TODO: Recalculate weights of internal structures based
    //		 on any conditions that may have changed.
    this->_standStats   = this->_standStats;
    this->_worldStats   = this->_worldStats;
    this->_upgradeStats = this->_upgradeStats;

    // Calculates demand based on internal states
    int cupsDemanded = this->calculateDemand();

    // Set the number of cups sold to the number demanded
    int cupsMade = this->_currentLemonadeStats.pitchers * this->_standStats.cupsPerPitcher;
    int cupsSold = cupsDemanded;

    // If more cups were demanded than made,
    // cap the demand to number made and flag
    // that the player sold out.
    if (cupsDemanded > cupsMade)
    {
        cupsSold = cupsMade;
        this->_currentDayStats.soldOut = true;
    }

    this->_currentDayStats.sales = cupsSold;
    this->_currentDayStats.demanded = cupsDemanded;
    this->_currentDayStats.income = cupsSold * this->_currentLemonadeStats.pricePerCup;

    // TODO: Calculate cost and profit.

    return;

}

int EconEngine::calculateDemand()
{
    // TODO: Calculate the demand according to the weights and values of all internal variables.

    return 100;
}
