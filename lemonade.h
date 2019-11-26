#ifndef LEMONADE_H
#define LEMONADE_H

#include <QObject>

// Forward declare LemonadeRecipe.
struct LemonadeRecipe;

class Lemonade
{

public:
    Lemonade(int,int,int,double);
    Lemonade();

    //Setters and getters.
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

    //A method to add elements.
    void addOneLemon();
    void addOneSugar();
    void addOneIce();
    void addOnePitcher();

    void reset();

private:
    int sugar;
    int lemon;
    int ice;

    //Might be moved out of this class, but starting with it here for now.
    double pricePerCup;
    int numPitchers;
};

#endif // LEMONADE_H
