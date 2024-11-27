#include "Character.h"

MainCharacter::MainCharacter(int x, int y) {
    position.x = 1;
    position.y = 1;
    health = maxHealth;
    doomCounter = 0;
	for (int i = 0; i < 5; ++i) {
		inventory[i] = 0;
	}
    // Initialize inventory or other members if necessary
}

void MainCharacter::move(int mcx, int mcy) {
	position.x += mcx;
	position.y += mcy;
}

bool MainCharacter::doomed() {
	return doomCounter >= 50;
}

bool MainCharacter::isAlive() {
	return health > 0;
}

void MainCharacter::doomIncrease(int amt) {
	doomCounter += amt;
}

void MainCharacter::doomDecrease(int amt) {
	if (doomCounter > 0 ){
		doomCounter -= amt;
	}
}

int MainCharacter::getHealth() {
	return health;
}

int MainCharacter::getDoom() {
	return doomCounter;
}

Position MainCharacter::getMCPosition() const {
	return position;
}

void MainCharacter::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

int MainCharacter::attack() const {
    // Implement attack logic
    return 10; // Example attack value
}