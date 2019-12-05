#include "lemonade.h"
#include "recipe.h"

/// Basic constructor to build a lemonade object with the input values of lemons, sugar, and ice.
/// Sets price per cup and number of pitchers to 0 and 1 respectively by default.
/// \brief Lemonade::Lemonade
/// \param lemons
/// \param sugar
/// \param ice
///
Lemonade::Lemonade(int lemons,int sugar,int ice, double price, int pitchers)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    this->pricePerCup = price;
    this->numPitchers = pitchers;
}

Lemonade::Lemonade()
{
    reset();
}

Lemonade::Lemonade(Lemonade *lemonade)
{
    this->lemon = lemonade->lemon;
    this->ice = lemonade->ice;
    this->sugar = lemonade->sugar;
    this->pricePerCup = lemonade->pricePerCup;
    this->numPitchers = lemonade->numPitchers;
}

void Lemonade::setRecipe(int lemons, int sugar, int ice, double price, int pitchers)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    this->pricePerCup = price;
    this->numPitchers = pitchers;
}

void Lemonade::setRecipe(const LemonadeRecipe recipe)
{
    this->lemon = recipe.lemons;
    this->sugar = recipe.sugar;
    this->ice = recipe.ice;
    this->pricePerCup = recipe.pricePerCup;
    this->numPitchers = recipe.pitchers;
}

///
/// \brief Lemonade::setPricePerCup
/// \param pricePerCup
///
void Lemonade::setPricePerCup(double pricePerCup)
{
    this->pricePerCup = pricePerCup;
}

///
/// \brief Lemonade::setLemon
/// \param lemon
///
void Lemonade::setLemon(int lemon)
{
    this->lemon = lemon;
}

///
/// \brief Lemonade::setSugar
/// \param sugar
///
void Lemonade::setSugar(int sugar)
{
    this->sugar = sugar;
}

///
/// \brief Lemonade::setIce
/// \param ice
///
void Lemonade::setIce(int ice)
{
    this->ice = ice;
}

///
/// \brief Lemonade::setNumPitchers
/// \param numPitchers
///
void Lemonade::setNumPitchers(int numPitchers)
{
    this->numPitchers = numPitchers;
}

///
/// \brief Lemonade::addOneLemon
///
void Lemonade::addOneLemon()
{
    ++lemon;
}

///
/// \brief Lemonade::addOneSugar
///
void Lemonade::addOneSugar()
{
    ++sugar;
}

///
/// \brief Lemonade::addOneIce
///
void Lemonade::addOneIce()
{
    ++ice;
}

///
/// \brief Lemonade::addOnePitcher
///
void Lemonade::addOnePitcher()
{
    ++numPitchers;
}

void Lemonade::reset()
{
    this->setRecipe(0,0,0,0,0);
}
