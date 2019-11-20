/**
  * This class is mimicing a "model".
  */
#include "econengine.h"
#include <iostream>

EconEngine::EconEngine(QObject *parent) : QObject(parent)
{

    // SETTLE: Do we want to calculate/randomize game conditions
    //		   at the beginning of the game, in order to give forecasts
    // 		   through the calendar and foreshadow news events?

}

void EconEngine::onNewDayRecipe(LemonadeRecipe newLemonadeRecipe)
{

    // Increment the day, and set today's lemonade recipe.
    game.currentDate++;
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
    std::cout<< "inNewDay"<< std::endl;
    // Convert Lemonade to a LemonadeStats struct
    LemonadeRecipe stats(newLemonade);


    // Use onNewDayStats to give the converted LemonadeStats
    // to the simulator.
    this->onNewDayRecipe(stats);

    return;
}

void EconEngine::onGameStatePushRequest()
{
    emit this->sigPushGameState(game);
}

void EconEngine::runSimulation()
{
    game.currentDate++;

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

    float cost = totalCostOfLemonade();
    calculateProfit(cost, game.today.income);

    return;
}

float EconEngine::calculateProfit(float cost, float income){
    return income - cost;
}

int EconEngine::calculateDemand()
{
    // TODO: Calculate the demand according to the weights and values of internal variables.

    return 100;
}

float EconEngine::totalCostOfLemonade(){
    float cost_of_lemons;
    float cost_of_sugar;
    //if organic upgrade is made, change the prices of lemons (and sugar).
    //if organic{
//    float cost_of_lemons = lemon * .8;
//    float cost_of_sugar = sugar * .1;
    //}
    //else{
    cost_of_lemons = game.today.lemonade.lemons * .4;
    cost_of_sugar = game.today.lemonade.sugar * .5;
    //}
    float cost_of_ice = game.today.lemonade.ice * .1;
    float total_cost_of_ingredients = cost_of_ice+ cost_of_sugar+ cost_of_lemons;
    //calculate cost in relation to number of pitchers.
    total_cost_of_ingredients = game.today.lemonade.pitchers * total_cost_of_ingredients;
    return total_cost_of_ingredients;
}

void EconEngine::plotGraph(){
//    QLineSeries *series = new QLineSeries();
//    series ->append();
//    QChart *chart = new QChart();
//    chart ->legend() ->show();
//    chart ->addSeries
}
