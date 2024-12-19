#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <graphics.h>
#include "Character.h"
#include "Enemy.h"

const int MAX_ENTITY = 5;

class gameState {
private:
    int viewportX, viewportY; // Top-left corner of the viewport
    const int viewportWidth = 20; // Width of the viewport in cells
    const int viewportHeight = 10; // Height of the viewport in cells
    char map[10][20]; // The map
    MainCharacter player; // The player

    Enemy enemy[MAX_ENTITY]; // The enemies
    //Position initialEnemyPositions[MAX_ENTITY] { {100, 200}, {150, 250}, {200, 300},}; // Add new position
    
    void* playerSprite;
    void* mobSprite;

    //NEW BOSS
    Enemy Boss;
    Position initialBossPosition = {5, 5};

public:
    gameState();
    void readInput(char input);
    void drawMap();
    void updateViewport();
    void displayHUD();
    void gameLoop();
    void battleScreen(Enemy& enemy, MainCharacter& player);
    void battleScreenBoss(Enemy& Boss, MainCharacter& player);
    void loadSprites();
    
    void initializeBoss();
};

#endif // GAMESTATE_H
