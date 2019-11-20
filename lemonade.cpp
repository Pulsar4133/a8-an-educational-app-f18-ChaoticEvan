#include "lemonade.h"

Lemonade::Lemonade(int lemons,int sugar,int ice)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    pricePerCup = 0;
    numPitchers = 1;
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

