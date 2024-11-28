#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class Enemy {
private:
	Position position;
	int health;
	
	//Boss
	int positionX, positionY;


public:
	Enemy(){
		position.x = 9;
		position.y = 9;
	}
	
	Enemy(int x, int y);
	void moveTowards(Position mcPosition, Enemy* enemies, int numEnemies);
	bool isAlive() const;
	Position getPosition() const;
	Position setPosition() const;
	void takeDamage(int damage);
    int attack() const;

	//BOSS (NEW)
	void setBossPosition(int x, int y){
		positionX = x;
        positionY = y;
	}

};
#endif // !ENEMY_H

