/**
  * This class is mimicing a "model".
  */
#include "econengine.h"

EconEngine::EconEngine(QObject *parent) : QObject(parent)
{

    // SETTLE: Do we want to calculate/randomize game conditions
    //		   at the beginning of the game, in order to give forecasts
    // 		   through the calendar and foreshadow news events?

}

void EconEngine::onNewDayStats(LemonadeRecipe newLemonadeRecipe)
{

    // Increment the day, and set today's lemonade recipe.
    game.currentDay++;
    game.today.lemonade = newLemonadeRecipe;

    // Runs the simulation using the new LemonadeStats provided
    this->runSimulation();

    // Signals that a simuation has been completed, providing
    // the stats for the current day.
    emit this->sigSimulationComplete(game);

    return;
}

void EconEngine::onNewDayLemonade(Lemonade newLemonade)
{
    // Convert Lemonade to a LemonadeStats struct
    LemonadeRecipe stats(newLemonade);


    // Use onNewDayStats to give the converted LemonadeStats
    // to the simulator.
    this->onNewDayStats(stats);

    return;
}

void EconEngine::onGameStatePushRequest()
{
    emit this->sigPushGameState(game);
}

void EconEngine::runSimulation()
{

    game.currentDay++;

    // TODO: Recalculate ideal lemonade stats,
    //		 e.g. different ice cubes based on
    //		 temperature or something.
    game.perfectLemonade.ice = 2;

    // TODO: Recalculate weights of internal structures based
    //		 on any conditions that may have changed.
    game.weights.reputation = 1.50;

    // Calculates demand based on internal states
    int cupsDemanded = this->calculateDemand();

    // Set the number of cups sold to the number demanded
    int cupsMade = game.today.lemonade.pitchers * game.stand.cupsPerPitcher;
    int cupsSold = cupsDemanded;

    // If more cups were demanded than made,
    // cap the demand to number made and flag
    // that the player sold out.
    if (cupsDemanded > cupsMade)
    {
        cupsSold = cupsMade;
        game.today.soldOut = true;
    }

    // Set the status of today's cups sold, cups demanded, and income.
    game.today.sales = cupsSold;
    game.today.demanded = cupsDemanded;
    game.today.income = cupsSold * game.today.lemonade.pricePerCup;

    // SETTLE: Would we like the cost to be contained here, and sent to the Ingredients
    // 		   team, or would we like to use this emit to poll the Ingredients team for
    //		   their calculation?
    emit sigCost();

    // TODO change placeholder of 0 to the actual cost obtained from emit ^.
    calculateProfit(0, game.today.income);

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
