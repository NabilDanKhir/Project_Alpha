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
		std::cout << "Points remaining: " << stats.getAvailablePoints() << "\n";
		std::cout << "\n";
		std::cout << "Enter the stat to increase (strength(S), intelligence(I), agility(A), luck(L)): ";
        std::string statName;
        std::cin >> statName;
	if (!stats.allocatePointToStat(statName)) {
            std::cout << "Invalid stat name. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            recalculateHealth(); // Update health after any stat allocation
        }
    }
	system("cls");
	std::cout << "Here is your final stat Ethan:\n";
	std::cout << "\n";
    displayStats();
}

void MainCharacter::displayStats() const {
    std::cout << "Strength: " << stats.getStrength() << " (Base: " << stats.strength << ")\n";
    std::cout << "Intelligence: " << stats.getIntelligence() << " (Base: " << stats.intelligence << ")\n";
    std::cout << "Agility: " << stats.getAgility() << " (Base: " << stats.agility << ")\n";
    std::cout << "Luck: " << stats.getLuck() << "\n";
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
    int baseDamage = stats.getStrength(); // Strength includes luck bonus
    int luckBonus = stats.getLuck() / 2; // Example: Add half of luck to damage
    int totalDamage = baseDamage + luckBonus;

    // Random factor and critical hit chance
    int randomFactor = std::rand() % 5;
    totalDamage += randomFactor;

    if (std::rand() % 100 < stats.getLuck()) {
        totalDamage *= 2; // Critical hit
    }

    return totalDamage;
}


bool attemptRun(const MainCharacter& player) {
    int agility = player.getAgility(); // Includes luck bonus
    int luckBoost = player.getLuck() / 2; // Luck directly improves success chance
    int successChance = 0;

    if (agility < 2) {
        successChance = 30 + luckBoost; // Base 30%, boosted by luck
    } else if (agility < 4) {
        successChance = 60 + luckBoost; // Base 60%, boosted by luck
    } else if (agility <= 6) {
        successChance = 90 + luckBoost; // Base 90%, boosted by luck
    }

    int randomValue = std::rand() % 100;
    return randomValue < successChance;
}


void MainCharacter::addGamePoints(int pts) {
    gamePoints += pts;
}

void MainCharacter::recalculateHealth() {
    // Calculate new max health based on intelligence
    int newMaxHealth = maxHealth + stats.getIntelligence() * 2; // For example, 2 extra health per intelligence point
    int healthDifference = newMaxHealth - maxHealth;
    health += healthDifference; // Add the difference to current health, if any
    maxHealth = newMaxHealth; // Update the maximum health
    if (health > maxHealth) {
        health = maxHealth; // Ensure current health doesn't exceed the new max
    }
}

