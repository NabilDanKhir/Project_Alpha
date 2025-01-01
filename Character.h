#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Position.h"
#include "Stats.h"

using namespace std;

class MainCharacter {
	private:
		Position position;
		int gamePoints = 0;
		int health;
		int doomCounter;
		int maxHealth = 10;
		const int doomLimit = 50;
		int inventory[5];
		Stats stats;

	public:
		MainCharacter(){
			position.x = 9;
			position.y = 9;
		}
		MainCharacter(int x, int y);
		void move(int mcx, int mcy);
		bool isAlive();
		bool doomed();
		void doomIncrease(int amount);
		void doomDecrease(int amount);

		int getHealth();
		int getDoom() const;
		Position getMCPosition() const;
		void takeDamage(int damage);
    	int attack() const;

		int getStrength() const { return stats.getStrength(); }
    	int getIntelligence() const { return stats.getIntelligence(); }
    	int getAgility() const { return stats.getAgility(); }
    	int getLuck() const { return stats.getLuck(); }
		int getLuckBonus(int baseStat) const {
    	return (baseStat * stats.getLuck()) / 20; // Correct access through `stats`
		}



		void setStrength(int str) { stats.setStrength(str); }
    	void setIntelligence(int intl) { stats.setIntelligence(intl); }
    	void setAgility(int agi) { stats.setAgility(agi); }
    	void setLuck(int lck) { stats.setLuck(lck); }
		void allocateInitialPoints();
		void recalculateHealth();
		void displayStats() const;
		void addGamePoints(int pts);
		void heal(int amount);

    	int getGamePoints() const {
			return gamePoints;
		};

		void setMCPosition(int x, int y){
			position.x = x;
			position.y = y;
		}

		int getMaxHealth() const {
    		return maxHealth;
		}

};

#endif // !CHARACTER_H
