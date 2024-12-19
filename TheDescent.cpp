#include <graphics.h>
#include <winbgim.h>
#include "gameState.h"

int main() {
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    initwindow(screenWidth, screenHeight, "The Descent");

    gameState game;
    game.gameLoop();

    closegraph();
    return 0;

}
