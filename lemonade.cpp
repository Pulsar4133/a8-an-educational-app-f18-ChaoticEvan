#include "lemonade.h"

Lemonade::Lemonade()
{
    _lemon = 0;
    _sugar = 0;
    _price = 0;
}

void Lemonade::setPrice(double price)
{
    this->_price = price;
}

void Lemonade::setLemon(int lemon)
{
    this->_lemon = lemon;
}

void Lemonade::setSugar(int sugar)
{
    this->_sugar = sugar;
}

void Lemonade::addOneLemon()
{
    ++_lemon;
}

void Lemonade::addOneSugar()
{
    ++_sugar;
}
