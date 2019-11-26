#ifndef ECONENGINE_H
#define ECONENGINE_H

#include "gamestate.h"
#include "lemonade.h"
#include <QObject>

/**
 * @brief The EconEngine class has two functions:
 * 			- Maintain the state of the game.
 * 			- Run the calculations for the economic simulation.
 * 		  It is a Singleton; the program's EconEngine singleton
 * 		  can be accessed at any time via:
 *
 * 		  EconEngine::instance();
 *
 * 		  Additionally, the state of the game can be accessed via:
 *
 * 		  EconEngine::gameState();
 */
class GameModel : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief getInstance Will return the program's EconModel instance.
     */
    static GameModel* instance();

    /**
     * @brief getState Will provide a read-only reference to the GameState
     * 		  structure of the program's EconModel singleton.
     */
    static GameState* state();

signals:
    /**
     * @brief sigSimulationComplete Signals that a simulation has been
     * 		  completed.
     * @param gameState, Updated to contain data regarding the recently
     * 		  completed day.
     */
    void sigSimulationComplete(const GameState& game);

public slots:
    /**
     * @brief onNewDay Is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonStats
     */
    void onNewDayRecipe(LemonadeRecipe newLemonadeRecipe);

    /**
     * @brief onNewDay Is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonade Is a Lemonade object with the stats necessary
     * 		  to run a simulation.
     */
    void onNewDayLemonade(Lemonade newLemonade);

    /**
     * @brief onUpgradePurchased Will purchase the upgrade at the given upgradeId
     * @param upgradeId, the ID of the stand upgrade.
     */
    void onUpgradePurchased(int upgradeId);

private:

    /**
     * @brief Creates a new EconEngine able to run economic simulations
     * 		  of a lemonade stand.
     */
    explicit GameModel(QObject *parent = nullptr);

    // TODO: Assignment Operator
    // TODO: Copy Constructor

    static GameModel* m_engineInstance;

    /**
     * @brief Game contains all of the current game state data, such as
     * 		  stand, upgrade, and world data.
     */
    GameState game;

    /**
     * @brief runSimulation Runs a one-day simulation according to the
     * 		  internal states of the EconEngine.
     */
    void runSimulation();

    /**
     * @brief calculateDemand Calculates one day's lemonade demand according
     * 		  to the internal state of the EconModel's member structs.
     */
    int calculateDemand();

    /**
     * @brief generateDays Will set the weather pattern of every unsimulated
     *		  day in the game.
     * @param days, The array of all days in the game.
     * @param numDays, The number of days in the game.
     */
    void  generateDays(Day* days, int numDays);

    /**
     * @brief setDisasterLevel3 A method that will generate the disaster that occurs on the 3rd level.
     */
    void setDisasterLevel3();

    /**
     * @brief calculateProfit A method to calculate the profit by subracting the cost from income.
     * @param cost
     * @param income
     * @return profit
     */
    float calculateProfit(float cost, float income);

    /**
     * @brief totalCostOfLemonade A method that calculates the cost of lemonade by analyzing the
     * inputs of number of lemons, sugar, and ice.
     * @return cost
     */
    float totalCostOfLemonade();

};

#endif // ECONENGINE_H
