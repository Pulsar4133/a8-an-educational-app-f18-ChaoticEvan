#ifndef ECONENGINE_H
#define ECONENGINE_H

#include <QObject>
#include "lemonade.h"

#define GAME_LENGTH 15

/**
 * @brief The LemonadeStats struct contains statistics for one day's
 * 		  lemonade composition, such as ingredient amounts and price
 * 		  per cup.
 */
struct LemonadeRecipe
{
    /**
     * @brief the amount of sugar in one pitcher of lemonade
     */
    int sugar	= 0;

    /**
     * @brief the amount of ice in one pitcher of lemonade
     */
    int ice		= 0;

    /**
     * @brief the amount of lemons in one pitcher of lemonade
     */
    int lemons	= 0;

    /**
     * @brief the price of one cup of lemonade
     */
    float pricePerCup	= 1.00;

    /**
     * @brief the number of pitchers of lemonade the player chose to create
     */
    int pitchers	= 0;

    /**
     * @brief LemonadeRecipe() constructs a default LemonadeRecipe.
     */
    LemonadeRecipe(){}

    /**
     * @brief LemonadeStats(Lemonade) converts a Lemonade object into a
     *        LemonadeStats struct.
     * @param newLemonade
     */
    LemonadeRecipe(Lemonade newLemonade){
        this->ice			= newLemonade.getIce();
        this->sugar			= newLemonade.getSugar();
        this->lemons		= newLemonade.getLemon();
        this->pricePerCup 	= newLemonade.getPricePerCup();
        this->pitchers		= newLemonade.getNumPitchers();
    }

};

/**
 * @brief The UPGRADE_ENUM enum provides an enumeration of all
 * 		  purchasable upgrades.
 */
enum UPGRADE_ENUM
{
    NEON_SIGN,
    WHALE_CANNON,
    LEMON_DEALER,

    // Maximum enum value. Keep at end of enum.
    // TECHNICALLY UNSAFE.
    NUM_UPGRADES,
};

struct Upgrades
{
    /**
     * @brief A list of ownership status for all upgrades
     */
    bool  purchased [UPGRADE_ENUM::NUM_UPGRADES];

    /**
     * @brief A list of costs for all upgrades
     */
    float cost      [UPGRADE_ENUM::NUM_UPGRADES];
};

/**
 * @brief The StandStats struct contains statistics for the current
 * 		  state of the player's stand, such as the stand's reputation,
 */
struct Stand
{
    /**
     * @brief reputation points to represent the current reputation of
     * 		  the player's stand
     */
    int reputation	= 0;

    /**
     * @brief marketing points to represent how much draw the player has
     * 		  to new customers
     */
    int marketing	= 0;

    /**
     * @brief the number of cups the player can make out of one pitcher
     * 		  of lemonade.
     */
    int cupsPerPitcher = 8;

    /**
     * @brief the upgrades the player has bought for their stand.
     */
    Upgrades upgrades;
};

/**
 * @brief The DayStats struct contains the statistics for one day of
 * 		  a lemonade stand.
 */
struct Day
{
    int   sales	= 0;
    int   demanded = 0;
    bool  soldOut = false;
    float cost	= 0;
    float income = 0;
    float profit = 0;

    LemonadeRecipe lemonade;

};

/**
 * @brief The WorldStats struct contains statistics for the current
 * 		  state of the game world, such as weather.
 */
struct World
{
    int weatherSeverity	= 0;

    float priceLemons	= 0.50;
    float priceSugar	= 0.40;
    float priceIce		= 0.10;
};

/**
 * @brief The Weights struct includes all of the weights that may be
 * 		  used in the demand model.
 */
struct Weights
{
    float reputation = 1.00;
    float marketing = 1.00;
};

/**
 * @brief The GameStats struct is the top-level struct for all game info
 * 		  and statistics.
 */
struct GameState
{
    /**
     * @brief current in-game date, starting at 0.
     */
    int currentDate   = 0;

    /**
     * @brief current in-game level, starting at 0 and increasing with
     *        every 5 days.
     */
    int currentLevel = this->currentDate / 5;

    /**
     * @brief statistics for the player's stand
     */
    Stand stand;

    /**
     * @brief statistics for the current state of the world.
     */
    World world;

    /**
     * @brief an array of all the stats for every in-game day
     */
    Day day[GAME_LENGTH];

    /**
     * @brief the currentDate's statistics.
     */
    Day today = this->day[this->currentDate];

    /**
     * @brief the current day's recipe for lemonade.
     */
    LemonadeRecipe currentLemonade = this->today.lemonade;

    /**
     * @brief the ideal recipe for lemonade
     */
    LemonadeRecipe perfectLemonade;

    /**
     * @brief the weights used in the demand calculations during the simulation
     */
    Weights weights;
};


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
