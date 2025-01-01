#include "item.h"

// Getter for Medkit's position
Position Medkit::getMedKitPosition() const {
    return posi;
}

// Getter for Medkit's heal amount
int Medkit::getAmountHeal() const {
    return amountHeal;
}
