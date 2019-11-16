#ifndef LEMONADE_H
#define LEMONADE_H

#include <QObject>

class Lemonade
{

public:
    Lemonade();

    void setPrice(double price);
    double getPrice(){return _price;}

    void setLemon(int lemon);
    int getLemon(){return _lemon;}

    void setSugar(int sugar);
    int getSugar(){return _sugar;}

    void addOneLemon();
    void addOneSugar();

private:
    int _sugar;
    int _lemon;

    //Might be moved out of this class, but starting with it here for now.
    double _price;

};

#endif // LEMONADE_H
