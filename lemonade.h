#ifndef LEMONADE_H
#define LEMONADE_H

#include <QObject>

/*
 * Forward declare LemonadeRecipe.
 */
struct LemonadeRecipe;

/**
 * A class that represents the Lemonade object in the education application.
 * @brief The Lemonade class
 */
class Lemonade
{

public:

    /**TODO fill out
     * @brief Lemonade
     */
    Lemonade(int,int,int,double);

    /**TODO fill out
     * @brief Lemonade
     */
    Lemonade();

    /**
     * Setters and getters.
     */
    void setRecipe(int, int, int, double);
    void setRecipe(const LemonadeRecipe recipe);

    void setPricePerCup(double pricePerCup);
    double getPricePerCup(){return pricePerCup;}

    void setLemon(int lemon);
    int getLemon(){return lemon;}

    void setSugar(int sugar);
    int getSugar(){return sugar;}

    void setIce(int ice);
    int getIce(){return ice;}

    void setNumPitchers(int numPitchers);
    int getNumPitchers(){return numPitchers;}

    /**
     * Methods to add elements.
     */
    void addOneLemon();
    void addOneSugar();
    void addOneIce();
    void addOnePitcher();

    /**
     * A method to reset the number of ingredients.
     * @brief reset
     */
    void reset();

private:
    /**
     * Instant variables that will be used to store the values of each below.
     * @brief sugar, lemon, ice, pricePerCup, numPitchers
     */
    int sugar;
    int lemon;
    int ice;

    //Might be moved out of this class, but starting with it here for now. TODO
    double pricePerCup;
    int numPitchers;
};

#endif // LEMONADE_H
