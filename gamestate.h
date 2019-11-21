#ifndef GAMESTATE_H
#define GAMESTATE_H

#define DEFAULT_GAME_LENGTH 15
#define STARTING_PLAYER_MONEY 100.00

#include "lemonade.h"
#include <functional>

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

struct Upgrade;
/**
 * @brief The UPGRADE_ENUM enum provides an enumeration of all
 * 		  purchasable upgrades.
 */
enum UPGRADE_ENUM
{
    NEON_SIGN,
    WHALE_UMBRELLA,
    SUGAR_DEALER,
    ORGANIC_LEMONS,

    // Maximum enum value. Keep at end of enum.
    // TECHNICALLY UNSAFE.
    NUM_UPGRADES,
};

class Upgrades
{

private:

    Upgrade* m_upgradeList[UPGRADE_ENUM::NUM_UPGRADES];

public:

    Upgrades();

    Upgrade*& operator[](int i) { return m_upgradeList[i]; }

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
     * @brief The current amount of money in the player's wallet.
     */
    float wallet = STARTING_PLAYER_MONEY;

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
    /**
     * @brief Describes whether or not this day has been simulated
     */
    bool  complete = false;

    /**
     * @brief Temperature of the day
     */
    int   temperature = 70;

    /**
     * @brief Number of sales for this day. 0 prior to simulation.
     */
    int   sales	= 0;

    /**
     * @brief Number of units demanded for this day. 0 prior to simulation.
     */
    int   demanded = 0;

    /**
     * @brief Describes if the player sold out of lemonade that day
     */
    bool  soldOut = false;

    /**
     * @brief Total cost of lemonade materials that day. 0 prior to simulation
     */
    float cost	= 0;

    /**
     * @brief Total income from sales for that day. 0 prior to simulation.
     */
    float income = 0;

    /**
     * @brief Total profit for that day. 0 prior to simulation.
     */
    float profit = 0;

    /**
     * @brief Recipe for lemonade that day.
     */
    LemonadeRecipe lemonade;

};

/**
 * @brief The WorldStats struct contains statistics for the current
 * 		  state of the game world, such as weather.
 */
struct World
{
    int weatherSeverity	= 0;

    /**
     * @brief Current price per unit of lemons
     */
    float priceLemons	= basePriceLemons * multiplierPriceLemons;

    /**
     * @brief Current price per unit of sugar
     */
    float priceSugar	= basePriceSugar * multiplierPriceSugar;

    /**
     * @brief Current price per unit of ice
     */
    float priceIce		= basePriceIce * multiplierPriceIce;

    /**
     * @brief baseLemonPrice is the base price of a lemon unit.
     */
    float basePriceLemons = 0.50;

    /**
     * @brief baseSugarPrice is the base price of a sugar unit.
     */
    float basePriceSugar = 0.40;

    /**
     * @brief baseIcePrice is the base price of an ice unit
     */
    float basePriceIce = 0.10;

    /**
     * @brief priceLemonMultiplier is the multiplier for lemon prices
     */
    float multiplierPriceLemons = 1.00;

    /**
     * @brief priceSugarMultiplier is the multiplier for sugar prices.
     */
    float multiplierPriceSugar = 1.00;

    /**
     * @brief priceIceMultiplier is the multiplier for ice prices.
     */
    float multiplierPriceIce = 1.00;
};

/**
 * @brief The Weights struct includes all of the weights that may be
 * 		  used in the demand model.
 */
struct Weights
{
    /**
     * @brief Demand increase per reputation point
     */
    float reputation = 1.00;

    /**
     * @brief Demand increase per marketing unit
     */
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
     * @brief gameLength represents the length of the game, in days
     */
    int gameLength = DEFAULT_GAME_LENGTH;

    /**
     * @brief statistics for the player's stand
     */
    Stand stand;

    /**
     * @brief The current amount of money in the player's wallet.
     */
    float& wallet = this->stand.wallet;

    /**
     * @brief statistics for the current state of the world.
     */
    World world;

    /**
     * @brief an array of all the stats for every in-game day
     */
    Day days[DEFAULT_GAME_LENGTH];

    /**
     * @brief the currentDate's statistics.
     */
    Day& today = this->days[this->currentDate];

    /**
     * @brief the current day's recipe for lemonade.
     */
    LemonadeRecipe& currentLemonade = this->today.lemonade;

    /**
     * @brief the ideal recipe for lemonade
     */
    LemonadeRecipe perfectLemonade;

    /**
     * @brief the weights used in the demand calculations during the simulation
     */
    Weights weights;

};

#endif // GAMESTATE_H