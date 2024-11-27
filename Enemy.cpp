#include "Enemy.h"

Enemy::Enemy(int x, int y) : position({x, y}), health(5) {}

void Enemy::moveTowards(Position mcPosition, Enemy* enemies, int numEnemies) {
    Position newPosition = position;

    if (position.x < mcPosition.x) {
        newPosition.x++;
    } else if (position.x > mcPosition.x) {
        newPosition.x--;
    }

    if (position.y < mcPosition.y) {
        newPosition.y++;
    } else if (position.y > mcPosition.y) {
        newPosition.y--;
    }

    // Check for collisions with other enemies and the player
    bool collision = false;
    for (int i = 0; i < numEnemies; ++i) {
        if (&enemies[i] != this && enemies[i].isAlive() && enemies[i].getPosition().x == newPosition.x && enemies[i].getPosition().y == newPosition.y) {
            collision = true;
            break;
        }
    }

    // Move only if there is no collision
    if (!collision && (newPosition.x != mcPosition.x || newPosition.y != mcPosition.y)) {
        position = newPosition;
    }
}

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
    return 10; // Example attack value
}