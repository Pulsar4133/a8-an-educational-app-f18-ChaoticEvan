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
    int sugar	= 0;
    int ice		= 0;
    int lemons	= 0;
    float pricePerCup	= 1.00;
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
    bool  purchased [UPGRADE_ENUM::NUM_UPGRADES];
    float cost      [UPGRADE_ENUM::NUM_UPGRADES];
};

/**
 * @brief The StandStats struct contains statistics for the current
 * 		  state of the player's stand, such as the stand's reputation,
 */
struct Stand
{
    int reputation	= 0;
    int marketing	= 0;

    int cupsPerPitcher = 8;

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
    int currentDay   = 0;
    int currentLevel = this->currentDay / 5;
    Stand stand;
    World world;
    Day   day[GAME_LENGTH];
    Day   today = this->day[this->currentDay];
    LemonadeRecipe currentLemonade = this->today.lemonade;
    LemonadeRecipe perfectLemonade;
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

    void sigCost();

public slots:
    /**
     * @brief onNewDay is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonStats
     */
    void onNewDayStats(LemonadeRecipe newLemonadeRecipe);

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

    float calculateProfit(float cost, float income);

};

#endif // ECONENGINE_H
