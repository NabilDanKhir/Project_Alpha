#include "Points.h"

Points::Points() : points(0) {}

void Points::addPoints(int pts) {
    points += pts;
}

int Points::getPoints() const {
    return points;
}

void Points::resetPoints() {
    points = 0;
}