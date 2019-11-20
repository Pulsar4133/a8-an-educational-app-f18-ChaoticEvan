#include "lemonade.h"
/// Basic constructor to build a lemonade object with the input values of lemons, sugar, and ice.
/// Sets price per cup and number of pitchers to 0 and 1 respectively by default.
/// \brief Lemonade::Lemonade
/// \param lemons
/// \param sugar
/// \param ice
///
Lemonade::Lemonade(int lemons,int sugar,int ice)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    pricePerCup = 0;
    numPitchers = 1;
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

