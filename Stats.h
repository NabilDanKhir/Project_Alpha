#ifndef STATS_H
#define STATS_H

class Stats {
public:
    int strength;
    int intelligence;
    int agility;
    int luck;

    Stats() : strength(1), intelligence(1), agility(1), luck(1) {}
    Stats(int str, int intl, int agi, int lck) : strength(str), intelligence(intl), agility(agi), luck(lck) {}

    int getStrength() const { return strength; }
    int getIntelligence() const { return intelligence; }
    int getAgility() const { return agility; }
    int getLuck() const { return luck; }

    void setStrength(int str) { strength = str; }
    void setIntelligence(int intl) { intelligence = intl; }
    void setAgility(int agi) { agility = agi; }
    void setLuck(int lck) { luck = lck; }
};

#endif // STATS_H