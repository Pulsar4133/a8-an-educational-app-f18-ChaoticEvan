#include "lemonade.h"

Lemonade::Lemonade()
{
    lemon = 0;
    sugar = 0;
    ice = 0;
    pricePerCup = 0;
    numPitchers = 0;
}

void Lemonade::setPricePerCup(double pricePerCup)
{
    this->pricePerCup = pricePerCup;
}

void Lemonade::setLemon(int lemon)
{
    this->lemon = lemon;
}

void Lemonade::setSugar(int sugar)
{
    this->sugar = sugar;
}

void Lemonade::setIce(int ice)
{
    this->ice = ice;
}

void Lemonade::setNumPitchers(int numPitchers)
{
    this->numPitchers = numPitchers;
}

void Lemonade::addOneLemon()
{
    ++lemon;
}

void Lemonade::addOneSugar()
{
    ++sugar;
}

void Lemonade::addOneIce()
{
    ++ice;
}

void Lemonade::addOnePitcher()
{
    ++numPitchers;
}

