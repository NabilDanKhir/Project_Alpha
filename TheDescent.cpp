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
    game.generateNewFloor(); // Ensure the first floor is generated
    game.gameLoop();

    system("cls");

    closegraph();

    int totalPts = 0;
    totalPts = player.getGamePoints();

    if (totalPts > 100 && player.isAlive()) {

        std::cout << "      __________" << std::endl;
        std::cout << "     |          |" << std::endl;
        std::cout << "     |          |" << std::endl;
        std::cout << "     |          |" << std::endl;
        std::cout << "     |   EXIT   |" << std::endl;
        std::cout << "     |          |" << std::endl;
        std::cout << "     |__________|" << std::endl;
        std::cout << "      ||      ||" << std::endl;
        std::cout << "      ||      ||" << std::endl;
        std::cout << "      ||______||" << std::endl;
        std::cout << "      ||______||" << std::endl;
        std::cout << "      ||      ||" << std::endl;
        std::cout << "      ||______||" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Your journey ends here... for now." << std::endl;
        std::cout << "The End! You scored " << totalPts << " / 225 points. Great job!" << std::endl;
        std::cout << std::endl;

    } else {
        std::cout << "               _____" << std::endl;
        std::cout << "             /      \\" << std::endl;
        std::cout << "             | () () |" << std::endl;
        std::cout << "             \\  ^   /" << std::endl;
        std::cout << "               ||||| " << std::endl;
        std::cout << "               |||||" << std::endl;
        std::cout << std::endl;

        std::cout << "    YOUR JOURNEY ENDS HERE..." << std::endl;
        std::cout << "   ____________________________" << std::endl;
        std::cout << "  /                            \\" << std::endl;
        std::cout << " /   Here lies an adventurer    \\" << std::endl;
        std::cout << "/   who dared to descend but     \\" << std::endl;
        std::cout << "|   fell to the perils below.    |" << std::endl;
        std::cout << "|_______________________________|" << std::endl;
        std::cout << std::endl;

        std::cout << "Game Over! You scored " << totalPts << " / 225 points. Better luck next time!" << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();

    return 0;
}
