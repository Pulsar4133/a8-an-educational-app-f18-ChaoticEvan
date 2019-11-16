#include "lemonade.h"

Lemonade::Lemonade()
{
    _lemon = 0;
    _sugar = 0;
    _ice = 0;
    _pricePerCup = 0;
    _numPitchers = 0;
}

void Lemonade::setPricePerCup(double pricePerCup)
{
    this->_pricePerCup = pricePerCup;
}

void Lemonade::setLemon(int lemon)
{
    this->_lemon = lemon;
}

void Lemonade::setSugar(int sugar)
{
    this->_sugar = sugar;
}

void Lemonade::setIce(int ice)
{
    this->_ice = ice;
}

void Lemonade::setNumPitchers(int numPitchers)
{
    this->_numPitchers = numPitchers;
}

void Lemonade::addOneLemon()
{
    ++_lemon;
}

void Lemonade::addOneSugar()
{
    ++_sugar;
}

void Lemonade::addOneIce()
{
    ++_ice;
}

void Lemonade::addOnePitcher()
{
    ++_numPitchers;
}
