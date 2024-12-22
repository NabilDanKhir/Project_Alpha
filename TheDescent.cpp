#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include "gameState.h"
#include "Character.h"

int main() {

    std::cout << "########################################" << std::endl;
    std::cout << "#                                      #" << std::endl;
    std::cout << "#             The Descent              #" << std::endl;
    std::cout << "#                                      #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << std::endl;
    std::cout << "Press Enter to start the game..." << std::endl;
    std::cin.get(); 

    MainCharacter player(1, 1);
    player.allocateInitialPoints();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeight, "The Descent");

    gameState game(player);
    game.gameLoop();

    system("cls");

    closegraph();

    int totalPts = 0;
    totalPts = player.getGamePoints();

    if (totalPts < 100) {

        std::cout << "  _____" << std::endl;
        std::cout << " /     \\" << std::endl;
        std::cout << "| () () |" << std::endl;
        std::cout << " \\  ^  /" << std::endl;
        std::cout << "  |||||" << std::endl;
        std::cout << "  |||||" << std::endl;

        std::cout << std::endl;

        std::cout << "Game Over! You scored " << totalPts << " points. Better luck next time!" << std::endl;

        std::cout << std::endl;

    } else {
        std::cout << "The End! You scored " << totalPts << " points. Great job!" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    return 0;
}
