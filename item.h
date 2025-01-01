#ifndef ITEM_H
#define ITEM_H

#include "Position.h"

class Medkit {
private:
    Position posi;
    int amountHeal;

public:
    Medkit() : posi({9, 9}), amountHeal(5) {}
    Medkit(int x, int y) : posi({x, y}), amountHeal(5) {}
    
    Position getMedKitPosition() const{
        return posi;
    };
    int getAmountHeal() const{
        return amountHeal;
    }
};


class Syringe : public Medkit{
private:
    Position posisi;
    int amountDoomDec;

public:
    Syringe() : posisi({9,9}), amountDoomDec(5){}
    Syringe(int x, int y) : posisi({x, y}), amountDoomDec(5) {}
    
    Position getSyringePosition() const{
        return posisi;
    }
    int getAmountDoomDec() const{
        return amountDoomDec;
    }
};
#endif // !ITEM_H