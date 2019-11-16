#ifndef LEMONADE_H
#define LEMONADE_H

#include <QObject>

class Lemonade
{

public:
    Lemonade();

    void setPricePerCup(double pricePerCup);
    double getPricePerCup(){return _pricePerCup;}

    void setLemon(int lemon);
    int getLemon(){return _lemon;}

    void setSugar(int sugar);
    int getSugar(){return _sugar;}

    void setIce(int ice);
    int getIce(){return _ice;}

    void setNumPitchers(int numPitchers);
    int getNumPitchers(){return _numPitchers;}

    void addOneLemon();
    void addOneSugar();
    void addOneIce();
    void addOnePitcher();

private:
    int _sugar;
    int _lemon;
    int _ice;

    //Might be moved out of this class, but starting with it here for now.
    double _pricePerCup;
    int _numPitchers;



};

#endif // LEMONADE_H
