#ifndef ECONENGINE_H
#define ECONENGINE_H

#include <QObject>
#include "lemonade.h"
#include "gamestate.h"

class EconEngine : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new EconEngine able to run economic simulations
     * 		  of a lemonade stand.
     * @param parent
     */
    explicit EconEngine(QObject *parent = nullptr);

signals:
    /**
     * @brief sigSimulationComplete signals that a simulation has been
     * 		  completed
     * @param gameState, updated to contain data regarding the recently
     * 		  completed day
     */
    void sigSimulationComplete(GameState game);

    /**
     * @brief sigPushWorldStats signals out the current game state upon
     * 		  request.
     * @param game
     */
    void sigPushGameState(GameState game);

public slots:
    /**
     * @brief onNewDay is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonStats
     */
    void onNewDayRecipe(LemonadeRecipe newLemonadeRecipe);

    /**
     * @brief onNewDay is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonade is a Lemonade object with the stats necessary
     * 		  to run a simulation.
     */
    void onNewDayLemonade(Lemonade newLemonade);

    /**
     * @brief onWorldStatPushRequest sends out the current state of the
     * 		  world via sigPushWorldStats when signalled.
     */
    void onGameStatePushRequest();

private:

    /**
     * @brief game contains all of the current game state data, such as
     * 		  stand, upgrade, and world data.
     */
    GameState game;

    /**
     * @brief runSimulation runs a one-day simulation according to the
     * 		  internal states of the EconEngine.
     */
    void runSimulation();

    /**
     * @brief calculateDemand calculates one day's lemonade demand according
     * 		  to the internal state of the EconModel's member structs.
     */
    int calculateDemand();

    /**
     * @brief setWeatherPattern will set the weather pattern of every unsimulated
     *		  day in the game.
     * @param days, the array of all days in the game.
     * @param numDays, the number of days in the game.
     */
    void  setWeatherPattern(Day* days, int numDays);

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
