#ifndef STATS_H
#define STATS_H

class Stats {
public:
    int strength;
    int intelligence;
    int agility;
    int luck;
    int availablePoints;

    Stats() : strength(1), intelligence(1), agility(1), luck(1), availablePoints(0) {}
    Stats(int str, int intl, int agi, int lck, int points = 5) : strength(str), intelligence(intl), agility(agi), luck(lck), availablePoints(points){}

    int getStrength() const { return strength; }
    int getIntelligence() const { return intelligence; }
    int getAgility() const { return agility; }
    int getLuck() const { return luck; }
    int getAvailablePoints() const { return availablePoints; }

    void setStrength(int str) { strength = str; }
    void setIntelligence(int intl) { intelligence = intl; }
    void setAgility(int agi) { agility = agi; }
    void setLuck(int lck) { luck = lck; }
    void addAvailablePoint() { ++availablePoints; }
    void allocateInitialPoints();
    void displayStats() const;

    bool allocatePointToStat(const std::string& statName) {
    if (availablePoints <= 0) {
        return false; // No points available to allocate
    }

    if (statName == "strength") {
        ++strength;
    } else if (statName == "intelligence") {
        ++intelligence;
    } else if (statName == "agility") {
        ++agility;
    } else if (statName == "luck") {
        ++luck;
    } else {
        return false; // Invalid stat name
    }

    --availablePoints; // Deduct the allocated point
    return true;
}
};

#endif // STATS_H