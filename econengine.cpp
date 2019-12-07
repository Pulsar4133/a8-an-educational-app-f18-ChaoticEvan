/**
  * This class is mimicing a "model".
  */
#include "econengine.h"
#include <math.h>
#include "upgrades.h"
#include <QDebug>

#define TEMPERATURE_AVERAGE 85
#define TEMPERATURE_VARIANCE 5
#define BASE_RECIPE_BONUS 50
#define REPUTATION_POINTS_PER_SALE 0.3
#define MAX_DEMAND 300
#define MIN_DEMAND 0
#define PERFECT_LEMONS 6
#define PERFECT_SUGAR  4
#define PERFECT_PRICE 1.5

// Initialize the instance pointer to null.
EconEngine* EconEngine::m_engineInstance = NULL;

EconEngine* EconEngine::instance()
{
    // If an engine instance has not been created yet, do so.
    if (!m_engineInstance)
    {
        m_engineInstance = new EconEngine();
    }

    // Return the pointer for the singleton instance of the engine.
    return m_engineInstance;
}

GameState* EconEngine::gameState()
{
    return &EconEngine::instance()->game;
}

EconEngine::EconEngine(QObject *parent) : QObject(parent)
{
    // SETTLE: Do we want to calculate/randomize game conditions
    //		   at the beginning of the game, in order to give forecasts
    // 		   through the calendar and foreshadow news events?

    // Set the future weather for all days in the game.
    this->generateDays(game.days, game.gameLength);

    game.perfectLemonade = LemonadeRecipe();
    game.perfectLemonade.lemons = PERFECT_LEMONS;
    game.perfectLemonade.sugar = PERFECT_SUGAR;
    game.perfectLemonade.pricePerCup = PERFECT_PRICE;
}


void EconEngine::onNewDayRecipe(LemonadeRecipe newLemonadeRecipe)
{
    // Set today's lemonade recipe from the provided recipe.
    game.today().lemonade = newLemonadeRecipe;

    // Runs the simulation using the new LemonadeStats provided.
    this->runSimulation();

    // Mark the day as completed.
    game.today().complete = true;

    // Increment the day.
    game.currentDate++;

    // Signals that a simuation has been completed, providing
    // the stats for the current day.
    emit this->sigSimulationComplete(game);

    return;
}

void EconEngine::onNewDayLemonade(Lemonade newLemonade)
{
    // Convert Lemonade to a LemonadeStats struct.
    LemonadeRecipe stats(newLemonade);

    // Use onNewDayStats to give the converted LemonadeStats
    // to the simulator.
    this->onNewDayRecipe(stats);

    return;
}

void EconEngine::onUpgradePurchased(int upgradeId)
{

    // Get the upgrade from the stand Upgrades.
    Upgrade* upgrade = game.stand.upgrades[upgradeId];

    // Deduct the cost of the upgrade from the player's wallet.
    game.stand.wallet -= upgrade->cost;

    // Designate that this upgrade has now been purchased.
    upgrade->purchased = true;

    // Execute the upgrade's effect.
    upgrade->effect(game);

    return;
}

void EconEngine::runSimulation()
{

    // Perfect lemonade: One ice cube per 5 degrees over 70 degrees
    game.perfectLemonade.ice = (game.today().temperature - 70) / 5;
    if (game.perfectLemonade.ice < 0)
    {
        game.perfectLemonade.ice = 0;
    }

    // Calculates demand based on internal states.
    int cupsDemanded = this->calculateDemand();

    // Set the number of cups sold to the number demanded.
    int cupsMade = game.today().lemonade.pitchers * game.stand.cupsPerPitcher;
    int cupsSold = cupsDemanded;

    // If more cups were demanded than made,
    // cap the demand to number made and flag
    // that the player sold out.
    if (cupsDemanded > cupsMade)
    {
        cupsSold = cupsMade;
        game.today().soldOut = true;
    }

    // Set the status of today's cups sold, cups demanded, and income.
    game.today().sales = cupsSold;
    game.today().demanded = cupsDemanded;
    game.today().income = cupsSold * game.today().lemonade.pricePerCup;

    // Calculate the total cost of the lemonade, and the profit.
    game.today().cost   = totalCostOfLemonade();
    game.today().profit = calculateProfit(game.today().cost, game.today().income);

    // Add the profit (whether it be positive or negative) to the player's
    // wallet.
    game.stand.wallet += game.today().profit;

    this->balanceWeights();

    return;
}

float EconEngine::calculateProfit(float cost, float income)
{
    return income - cost;
}

int EconEngine::calculateDemand()
{
    int result = 0; //Default have it max demand (without upgrades).

    int temp = game.today().temperature;

    //Switched based off of the temperature.
    int baseDemand = 0;
    switch(temp){
    case 55:
        baseDemand = 10;
        break;
    case 25:
        baseDemand = 5;
        break;
    case 65:
        baseDemand = 15;
        break;
    case 72:
        baseDemand = 20;
        break;
    }

    float priceMult = this->determinePriceWeight();

    result = baseDemand;
    int sumReputation = int(game.stand.reputation  * game.weights.reputation);

    result = (baseDemand
            + sumReputation)
            * priceMult;

    if (result > MAX_DEMAND)
    {
        result = MAX_DEMAND;
    }
    else if (result < MIN_DEMAND)
    {
        result = MIN_DEMAND;
    }

    qDebug() << baseDemand
             << "\t"
             << game.stand.reputation
             << "\t"
             << sumReputation
             << "\t"
             << priceMult
             << "\t"
             << result;

    return result;
}

void EconEngine::balanceWeights()
{
    game.stand.reputation += int(game.today().sales * REPUTATION_POINTS_PER_SALE);
    game.stand.reputation += determineRecipeBonus();
    // TODO: Incorporate price into reputation
}

int EconEngine::determineRecipeBonus()
{
    int result = BASE_RECIPE_BONUS;

    int lemonsDiff = 	game.today().lemonade.lemons -
                        game.perfectLemonade.lemons;

    int sugarDiff = 	game.today().lemonade.sugar -
                        game.perfectLemonade.sugar;

    int iceDiff = 		game.today().lemonade.ice -
                        game.perfectLemonade.ice;

    int squaredDiffSum = lemonsDiff * lemonsDiff +
                         sugarDiff * sugarDiff +
                         iceDiff * iceDiff;

    result -= squaredDiffSum;

    return result;

}

float EconEngine::determinePriceWeight()
{
    float idealPrice = game.perfectLemonade.pricePerCup;

    float diffRatio = game.today().lemonade.pricePerCup / idealPrice;

    return 1 / (diffRatio);
}

void EconEngine::generateDays(Day* days, int numDays)
{
    for (int i = 0; i < numDays; i++)
    {
        // Skip day if it has already been simulated.
        if (days[i].complete)
        {
            continue;
        }
        if(i == 14){
            setDisasterLevel3();
        }
        else if(i == 9){ //Tornado disaster.
            game.days ->disaster = 1;
        }
        else{ //No Disaster
            game.days -> disaster = 0;
        }
        int random = 0 + ( std::rand() % ( 3 - 0 + 1 ) );
        game.days[i].weatherState = random;
        switch(random){
        case 0:
            //Rainy weather.
            days[i].temperature = 55;
            break;
        case 1:
            //Snowy weather.
            days[i].temperature = 25;
            break;
        case 2:
            //Cloudy weather.
            days[i].temperature = 65;
            break;
        case 3:
            //Sunny weather.
            days[i].temperature = 72;
            break;
        }
    }
    return;
}

void EconEngine::setDisasterLevel3(){
    int random = 2 + ( std::rand() % ( 3 - 2 + 1 ) );
    switch (random){
    case 1:
        //Duck Disaster
        game.days->disaster = 2;
        break;
    case 2:
        //Whale Disaster
        game.days->disaster = 3;
        break;
    }
}

float EconEngine::totalCostOfLemonade()
{
    float costOfLemons = game.today().lemonade.lemons * game.world.priceLemons();
    float costOfSugar  = game.today().lemonade.sugar * game.world.priceSugar();
    float costOfIce    = game.today().lemonade.ice * game.world.priceIce();

    float totalCostOfIngredients = costOfIce + costOfSugar + costOfLemons;

    //Calculate cost in relation to number of pitchers.
    totalCostOfIngredients = game.today().lemonade.pitchers * totalCostOfIngredients;
    return totalCostOfIngredients;
}
