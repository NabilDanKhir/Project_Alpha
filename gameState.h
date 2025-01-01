#ifndef GAMESTATE_H
#define GAMESTATE_H


#define MAP_WIDTH 20
#define MAP_HEIGHT 10
#include <graphics.h>
#include <vector>
#include <set>
#include "Character.h"
#include "Enemy.h"
#include "item.h"

const int MAX_ENTITY = 5;
const int MAX_MEDKIT = 1;
const int MAX_SYRINGE = 1;

class gameState {
private:
    int viewportX, viewportY; // Top-left corner of the viewport
    const int viewportWidth = 20; // Width of the viewport in cells
    const int viewportHeight = 10; // Height of the viewport in cells
    char map[10][20]; // The map
    int currentFloor;
    int savedDoomCounter;
    MainCharacter& player; // The player

    Enemy enemy[MAX_ENTITY]; // The enemies
    bool enemyPlaced[MAX_ENTITY]; // Track if enemies are placed on the map
    
    void* playerSprite;
    void* mobSprite;
    void* bossSprite1;
    void* albabSprite;
    void* medicalSprite;
    void* syringeSprite;

    //NEW BOSS
    Boss boss;
    
    Medkit medkit[MAX_MEDKIT];
    bool medkitPlaced[MAX_MEDKIT];

    Syringe syringe[MAX_SYRINGE];
    bool syringePlace[MAX_SYRINGE];

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
    
    void placeMedkitRandomly(std::set<std::pair<int, int>>& usedPositions);
    void placeSyringeRandomly(std::set<std::pair<int, int>>& usedPositions);
    void placeItemsRandomly();

    void endGame();

    void spawnBoss();
    void loadBossSprite(int floor);
};

#endif // GAMESTATE_H
