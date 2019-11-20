#ifndef UPGRADE_H
#define UPGRADE_H

#include <QString>
#include "gamestate.h"
#include <functional>

/**
 * @brief The UPGRADE_ENUM enum provides an enumeration of all
 * 		  purchasable upgrades.
 */
enum UPGRADE_ENUM
{
    NEON_SIGN,
    WHALE_CANNON,
    SUGAR_DEALER,
    ORGANIC_LEMONS,

    // Maximum enum value. Keep at end of enum.
    // TECHNICALLY UNSAFE.
    NUM_UPGRADES,
};

struct Upgrade
{
    /**
     * @brief name of the upgrade
     */
    QString name;

    /**
     * @brief fun description about the upgrade
     */
    QString description = "";

    /**
     * @brief cost of the upgrade, in dollars
     */
    float   cost;

    /**
     * @brief enumerable id of the upgrade, as listed in UPGRADE_ENUM
     */
    int id;

    /**
     * @brief purchased status of the uprade
     */
    bool purchased = false;

    /**
     * @brief An effect function, stored as a variable, to run when the Upgrade's
     * 		  effect is applied.
     * @note  Remember delegates from CS3500? This is one of those, but in C++!
     *
     * 		  Hurray!
     *
     * 		  What you'll need to do is create a lambda function that takes the
     * 		  signature "void method(GameState&)". This function should go in,
     * 	      mess around with the GameState (such as adjusting weights, variables,
     * 		  prices, etc.) as necessary.
     *
     * 		  Read the C++ documentation on how to create lambda functions, as that
     * 		  is the most obvious way to create this. However, a brief synopsis below:
     *
     * 		  LAMBDA FUNCTION:
     *
     * 		  	  [] (GameState game) -> void
     * 			  {
     * 			      ***FUNCTION STUFF HERE ***
     * 			  }
     *
     * 		  Ask Spencer if you have any questions :)
     */
     void(*effect)(GameState&);

    /**
     * @brief Upgrade will contain all the information required for an upgrade
     * 		  the user can make to their stand.
     * @param _name, the name of the upgrade
     * @param _cost, the cost of the upgrade
     * @param _id,   the enum ID of the upgrade in UPGRADE_ENUM
     * @param _effect, the effect of the upgrade. READ UPGRADE::EFFECT NOTE
     */
    Upgrade(QString _name, float _cost, int _id, void(*_effect)(GameState&)):
        name(_name),
        cost(_cost),
        id(_id),
        effect(_effect)
    {}

    /**
     * @brief Upgrade will contain all the information required for an upgrade
     * 		  the user can make to their stand.
     * @param _name, the name of the upgrade
     * @param _description, the description of the upgrade
     * @param _cost, the cost of the upgrade
     * @param _id,   the enum ID of the upgrade in UPGRADE_ENUM
     * @param _effect, the effect of the upgrade. READ UPGRADE::EFFECT NOTE
     */
    Upgrade(QString _name, QString _description, float _cost, int _id, void(*_effect)(GameState&)):
        name(_name),
        description(_description),
        cost(_cost),
        id(_id),
        effect(_effect)
    {}


};

class Upgrades
{

private:

    Upgrade* m_upgradeList[UPGRADE_ENUM::NUM_UPGRADES];

public:

    Upgrades()
    {

        this->m_upgradeList[UPGRADE_ENUM::NEON_SIGN] = new Upgrade(
                    "Neon Sign",
                    "Throw a neon sign on that dingy stand to attract some new customers!",
                    50.00,
                    UPGRADE_ENUM::NEON_SIGN,
                    [] (GameState& game)
                    {
                        game.stand.marketing += 10;
                    });

        this->m_upgradeList[UPGRADE_ENUM::WHALE_CANNON] = new Upgrade(
                    "Whale Cannon",
                    "Life might give you a lot more than lemons. Be sure you can shoot whatever life gives you out of the sky!",
                    2000.00,
                    UPGRADE_ENUM::WHALE_CANNON,
                    [] (GameState& game)
                    {});

        this->m_upgradeList[UPGRADE_ENUM::SUGAR_DEALER] = new Upgrade(
                    "Sugar Dealer",
                    "You call up an old friend from college who can get you some superb, high-quality sugar. He won't tell you where it's from, and it costs an arm and a leg, but it's deeeeelightful!",
                    150.00,
                    UPGRADE_ENUM::SUGAR_DEALER,
                    [] (GameState& game)
                    {
                        game.stand.reputation += 20;
                        game.world.multiplierPriceSugar += 0.50;
                    });

    }

    Upgrade*& operator[](int i) { return m_upgradeList[i]; }

};


#endif // UPGRADE_H
