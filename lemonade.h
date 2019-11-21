#ifndef LEMONADE_H
#define LEMONADE_H

#include <QObject>

class Lemonade
{

public:
    Lemonade(int,int,int,double);

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

    void addOneLemon();
    void addOneSugar();
    void addOneIce();
    void addOnePitcher();

private:
    int sugar;
    int lemon;
    int ice;

    //Might be moved out of this class, but starting with it here for now.
    double pricePerCup;
    int numPitchers;
};

#endif // LEMONADE_H
