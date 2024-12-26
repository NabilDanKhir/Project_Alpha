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

    int getStrength() const {
    return strength + getLuckBonus(strength); // Include bonus
    }
    int getIntelligence() const {
    return intelligence + getLuckBonus(intelligence);
    }
    int getAgility() const {
    return agility + getLuckBonus(agility);
    }
    int getLuck() const {
    return luck; // Luck does not modify itself
    }
    int getAvailablePoints() const { return availablePoints; }
    int getLuckBonus(int baseStat) const {
    return (baseStat * luck) / 20; // Calculate luck bonus for baseStat
    }  



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
    
        else {
            if (statName == "S" || statName == "s" || statName == "strength" || statName == "Strength") {
            ++strength;
            }   
    
            else if (statName == "I" || statName == "i" || statName == "intelligence" || statName == "Intelligence") {
            ++intelligence;
            } 
            else if (statName == "A" || statName == "a" || statName == "agility" || statName == "Agility") {
            ++agility;
            }    
            else if (statName == "L" || statName == "l" || statName == "luck" || statName == "Luck") {
            ++luck;
            } 
            else {
                return false; // Invalid stat name
            }

            --availablePoints; // Deduct the allocated point
            return true;
        }   
   }
};
#endif // STATS_H