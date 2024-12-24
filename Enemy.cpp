#include "Enemy.h"

Enemy::Enemy(int x, int y) : position({x, y}), health(5) {}
Boss::Boss(int bossX, int bossY) : pos({bossX, bossY}), health(20) {}

bool Enemy::isAlive() const {
    return health > 0;
}

Position Enemy::getPosition() const {
    return position;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

int Enemy::attack() const {
    // Implement attack logic
    return 1; // Example attack value
}

Position Boss::getBossPosition() const {
    return pos;
}

bool Boss::isAlive() const {
    return health > 0;
}

int Boss::attack() const {
    // Implement attack logic
    return 2; // Example attack value
}

void Boss::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}