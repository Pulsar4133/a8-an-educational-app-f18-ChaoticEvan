/**
  * This class is mimicing a "model".
  */
#include "econengine.h"
#include "upgrades.h"

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
    this->setWeatherPattern(game.days, game.gameLength);
}


void EconEngine::onNewDayRecipe(LemonadeRecipe newLemonadeRecipe)
{
    // Set today's lemonade recipe from the provided recipe.
    game.today().lemonade = newLemonadeRecipe;

    // Runs the simulation using the new LemonadeStats provided
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
    // Convert Lemonade to a LemonadeStats struct
    LemonadeRecipe stats(newLemonade);

    // Use onNewDayStats to give the converted LemonadeStats
    // to the simulator.
    this->onNewDayRecipe(stats);

    return;
}

void EconEngine::onUpgradePurchased(int upgradeId)
{

    // Get the upgrade from the stand Upgrades
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

    // Calculate the total cost of the lemonade, and the profit
    game.today().cost   = totalCostOfLemonade();
    game.today().profit = calculateProfit(game.today().cost, game.today().income);

    // Add the profit (whether it be positive or negative) to the player's
    // wallet.
    game.stand.wallet += game.today().profit;

    // SETTLE: Do we want bankruptcy to be a Game Over condition?

    return;
}

float EconEngine::calculateProfit(float cost, float income)
{
    return income - cost;
}

int EconEngine::calculateDemand()
{
    // TODO: Calculate the demand according to the weights and values of internal variables.
    //       currently
    int result = 100;

    return result;
}

void EconEngine::setWeatherPattern(Day* days, int numDays)
{
    // TODO: Determine how we want to "randomize" weather patterns.
    for (int i = 0; i < numDays; i++)
    {
        // Skip day if it has already been simulated.
        if (days[i].complete)
        {
            continue;
        }

        days[i].temperature = 65;
    }

    return;

}

float EconEngine::totalCostOfLemonade()
{
    float costOfLemons = game.today().lemonade.lemons * game.world.priceLemons;
    float costOfSugar  = game.today().lemonade.sugar * game.world.priceSugar;
    float costOfIce    = game.today().lemonade.ice * game.world.priceIce;

    float totalCostOfIngredients = costOfIce + costOfSugar + costOfLemons;

    //calculate cost in relation to number of pitchers.
    totalCostOfIngredients = game.today().lemonade.pitchers * totalCostOfIngredients;
    return totalCostOfIngredients;
}
