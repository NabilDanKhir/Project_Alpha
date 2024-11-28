#include <graphics.h>
#include <winbgim.h>
#include "gameState.h"

int main() {
    int gd = DETECT, gm;
    
    initgraph(&gd, &gm, (char*)"");

    gameState game;
    game.gameLoop();

    closegraph();
    return 0;

    //test
}
