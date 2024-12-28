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
    int currentFloor;
    MainCharacter& player; // The player

    Enemy enemy[MAX_ENTITY]; // The enemies
    bool enemyPlaced[MAX_ENTITY]; // Track if enemies are placed on the map
    
    void* playerSprite;
    void* mobSprite;
    void* bossSprite1;
    void* albabSprite;

    //NEW BOSS
    Boss boss;

public:
    gameState(MainCharacter& player);
    void readInput(char input);
    void drawMap();
    void updateViewport();
    void displayHUD();
    void gameLoop();
    void battleScreen(Enemy& enemy, MainCharacter& player);
    void battleScreenBoss(Boss& boss, MainCharacter& player);
    void loadSprites();
    void initializeBoss();
    bool attemptRun();

    void generateNewFloor();
    void displayFloorNumber();
    void transitionToNextFloor(); 

    void placeEnemiesRandomly();
    void checkRandomEncounter();
};

#endif // GAMESTATE_H
