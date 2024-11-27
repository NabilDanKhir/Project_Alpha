#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Position.h"

using namespace std;

class MainCharacter {
	private:
		Position position;
		int health;
		int doomCounter;
		const int maxHealth = 10;
		const int doomLimit = 50;
		int inventory[5];

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

};

#endif // !CHARACTER_H
