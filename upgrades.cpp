#include "upgrades.h"
#include "gamestate.h"

Upgrades::Upgrades()
{

        this->m_upgradeList[UPGRADE_ENUM::NEON_SIGN] = new Upgrade(
                    "Neon Sign", // Name
                    "Throw a neon sign on that dingy stand to attract some new customers!", // Description
                    50.00, // Cost
                    UPGRADE_ENUM::NEON_SIGN, // Enum ID
                    [] (GameState& game) // Effect
                    {
                        game.stand.marketing += 10;
                    });

        this->m_upgradeList[UPGRADE_ENUM::WHALE_UMBRELLA] = new Upgrade(
                    "Whale Umbrella",
                    "Life might give you a lot more than lemons. Be sure you can bounce whatever life gives you out of the sky!",
                    2000.00,
                    UPGRADE_ENUM::WHALE_UMBRELLA,
                    [] (GameState& game)
                    {});

        this->m_upgradeList[UPGRADE_ENUM::SUGAR_DEALER] = new Upgrade(
                    "Lemonade Dealer",
                    "You call up an old friend from college who can get you some superb, high-quality sugar and lemons. He won't tell you where he gets them, and they cost an arm and a leg, but they're the bees knees!",
                    150.00,
                    UPGRADE_ENUM::SUGAR_DEALER,
                    [] (GameState& game)
                    {
                        game.stand.reputation += 20;
                        game.world.multiplierPriceSugar += 0.50;
                        game.world.multiplierPriceLemons += 0.50;
                    });

}
