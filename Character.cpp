#include "Character.h"
#include <iostream>
#include <limits>

MainCharacter::MainCharacter(int x, int y) 
{
    position.x = x;
    position.y = y;
    health = maxHealth;
    doomCounter = 0;
	for (int i = 0; i < 5; ++i) {
		inventory[i] = 0;
	}
    // Initialize inventory or other members if necessary
	stats = Stats(1, 1, 1, 1, 5);

	allocateInitialPoints();
}

void MainCharacter::allocateInitialPoints() 
{
    std::cout << "Hello Ethan, what do you know about yourself " << endl;
			  
    while (stats.getAvailablePoints() > 0) {
        displayStats();
		std::cout << "Enter the stat to increase (strength(S), intelligence(I), agility(A), luck(L)): ";
        std::string statName;
        std::cin >> statName;
	if (!stats.allocatePointToStat(statName)) {
            std::cout << "Invalid stat name. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
	std::cout << "Here is your final stat:\n";
    displayStats();
}

void MainCharacter::displayStats() const {
    std::cout << "Strength: " << stats.getStrength() << "\n";
    std::cout << "Intelligence: " << stats.getIntelligence() << "\n";
    std::cout << "Agility: " << stats.getAgility() << "\n";
    std::cout << "Luck: " << stats.getLuck() << "\n";
    std::cout << "Points remaining: " << stats.getAvailablePoints() << "\n";
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
	int luck = stats.getLuck();
	if (std::rand() % 100 < luck) {
        // Damage avoided
        return;
    }

    health -= damage;
    if (health < 0) health = 0;
}

int MainCharacter::attack() const {
    // Implement attack logic
      int baseDamage = stats.getStrength();
	  int luckBonus = stats.getLuck() / 1;
	  int totalDamage = baseDamage + luckBonus;
	  
	  int randomFactor = std::rand() % 5;
	  totalDamage += randomFactor;
	  
	  if (std::rand() % 100 < stats.getLuck()) 
	  {
		totalDamage *= 2;
	  }

	  return totalDamage;
}