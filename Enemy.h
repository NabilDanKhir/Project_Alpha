#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class Enemy {
private:
	Position position;
	int health;
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


};

class Boss : public Enemy {
	private:
	Position pos;
	int health;

	public: 
	Boss() {
		pos.x = 5;
		pos.y = 2;
	}

	//BOSS (NEW)
	Boss(int x, int y);
	Position setBossPosition() const;
	Position getBossPosition() const;

};
#endif // !ENEMY_H

