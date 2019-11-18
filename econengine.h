#ifndef ECONENGINE_H
#define ECONENGINE_H

#include <QObject>

/**
 * @brief The LemonadeStats struct contains statistics for one day's
 * 		  lemonade composition, such as ingredient amounts and price
 * 		  per cup.
 */
struct LemonadeStats
{
    int sugar	= 0;
    int ice		= 0;
    int lemons	= 0;
    float pricePerCup	= 1.00;
    int pitchers	= 0;
};

/**
 * @brief The StandStats struct contains statistics for the current
 * 		  state of the player's stand, such as the stand's reputation,
 * 		  and the weights for these statistics.
 */
struct StandStats
{
    float reputation		= 0.00;
    float weightReputation	= 0.00;

    float marketing			= 0;
    float weightMarketing	= 0.00;

    int   cupsPerPitcher	= 8;

    int   sampleDummy		= 0;
    float weightSampleDummy	= 0.00;
};

/**
 * @brief The WorldStats struct contains statistics for the current
 * 		  state of the game world, such as weather, and the demand
 * 		  weights for these statistics.
 */
struct WorldStats
{
    int   weatherSeverity		= 0;
    float weightWeatherSeverity = 0.00;

    double priceLemons	= 0.50;
    double priceSugar	= 0.40;
    double priceIce		= 0.10;

    int   sampleDummy		= 0;
    float weightSampleDummy	= 0.00;
};

/**
 * @brief The UpgradeStats struct contains statistics for the current
 * 		  upgrade state of the lemonade stand, and the weights for
 * 		  these upgrades.
 */
struct UpgradeStats
{
    int   sampleDummy		= 0;
    float sampleDummyWeight	= 0.00;

    int   sampleDummy2			= 0;
    float sampleDummy2Weight	= 0.00;
};

/**
 * @brief The DayStats struct contains the statistics for one day of
 * 		  a lemonade stand.
 */
struct DayStats
{
    int   sales	= 0;
    int   demanded = 0;
    bool  soldOut = false;
    float cost	= 0;
    float income = 0;
    float profit = 0;

    void reset()
    {
        this->sales = 0;
        this->demanded = 0;
        this->soldOut = false;
        this->cost   = 0.00;
        this->income = 0.00;
        this->profit = 0.00;
    }

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
     * @param dayStats, statistics for the just-completed day.
     */
    void sigSimulationComplete(DayStats dayStats);
    void sigCost();

public slots:
    /**
     * @brief onNewDay is signalled whenever a new day is requested. In
     * 		  order to be initiated, a new set of of lemonade stats is
     * 		  required.
     * @param newLemonStats
     */
    void onNewDay(LemonadeStats newLemonadeStats);
    float calculateProfit(float cost, float income);

private:

    /**
     * @brief _standStats stats for current game stand
     */
    StandStats standStats;

    /**
     * @brief _worldStats stats for current world state
     */
    WorldStats worldStats;

    /**
     * @brief _upgradeStats stats for current stand upgrades
     */
    UpgradeStats upgradeStats;

    /**
     * @brief _currentDayStats stats for the results of the current day
     */
    DayStats currentDayStats;

    /**
     * @brief _currentLemonadeStats stats for the current day's lemonade
     */
    LemonadeStats currentLemonadeStats;

    /**
     * @brief _perfectLemonadeStats stats for the current day's ideal lemonade mix
     */
    LemonadeStats perfectLemonadeStats;

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

};

#endif // ECONENGINE_H
