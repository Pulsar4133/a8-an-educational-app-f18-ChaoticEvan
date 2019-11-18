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

float Lemonade::totalCostOfLemonade(){
    //Todo: if organic upgrade is made, change the prices of lemons (and sugar).
//    float cost_of_lemons = lemon * .8;
//    float cost_of_sugar = sugar * .1;
    float cost_of_ice = ice * .1;
    float cost_of_lemons = lemon * .4;
    float cost_of_sugar = sugar * .5;
    float total_cost_of_ingredients = cost_of_ice+ cost_of_sugar+ cost_of_lemons;
    //calculate cost in relation to number of pitchers.
    total_cost_of_ingredients = numPitchers * total_cost_of_ingredients;
    //emit sigReturnCost(total_cost_of_ingredients);
    return total_cost_of_ingredients;
}
