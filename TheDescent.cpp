#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <thread>
#include <chrono>
#include "gameState.h"
#include "Character.h"

int main() {

    std::cout << "########################################" << std::endl;
    std::cout << "#                                      #" << std::endl;
    std::cout << "#             The Descent              #" << std::endl;
    std::cout << "#                                      #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Press Enter to start the game..." << std::endl;
    std::cin.get(); 

    MainCharacter player(1, 1);
    player.allocateInitialPoints();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeight, "The Descent");

    gameState game(player);
    game.gameLoop();

    closegraph();
    return 0;
}
