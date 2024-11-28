#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Position.h"
#include "Stats.h"

using namespace std;

class MainCharacter {
	private:
		Position position;
		int health;
		int doomCounter;
		const int maxHealth = 10;
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
		int getDoom();
		Position getMCPosition() const;
		void takeDamage(int damage);
    	int attack() const;

		

		int getStrength() const { return stats.getStrength(); }
    	int getIntelligence() const { return stats.getIntelligence(); }
    	int getAgility() const { return stats.getAgility(); }
    	int getLuck() const { return stats.getLuck(); }

		void setStrength(int str) { stats.setStrength(str); }
    	void setIntelligence(int intl) { stats.setIntelligence(intl); }
    	void setAgility(int agi) { stats.setAgility(agi); }
    	void setLuck(int lck) { stats.setLuck(lck); }
		void allocateInitialPoints();
		void displayStats() const;

};

#endif // !CHARACTER_H
