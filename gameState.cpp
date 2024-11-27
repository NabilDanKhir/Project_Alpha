#include "gameState.h"
#include <graphics.h>  

gameState::gameState() : player(1, 1) { // Initialize player at position (1, 1)
    const char tempMap[10][20] = {
        "###################",
        "#.................#",
        "#.................#",
        "#.................#",
        "#.................#",
        "###################"
    };

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 20; ++j)
            map[i][j] = tempMap[i][j];

    // Set the player's initial position in the map
    Position mcPos = player.getMCPosition();
    map[mcPos.y][mcPos.x] = 'P';

    // Initialize a fixed number of enemies
    enemy[0] = Enemy(10, 3);
    // Additional enemies can be added similarly
    for (int i = 1; i < MAX_ENTITY; ++i) {
        enemy[i] = Enemy(0, 0);  // Initialize remaining enemies off-map
    }
}

void gameState::drawMap() {
    cleardevice();
    int cellSize = 16;
    for (int y = 0; y < viewportHeight; y++) {
        for (int x = 0; x < viewportWidth; x++) {
            int mapX = viewportX + x;
            int mapY = viewportY + y;
            if (mapY < 10 && mapX < 20) { // Ensure within bounds
                char symbol[2] = { map[mapY][mapX], '\0' };
                outtextxy(x * cellSize, y * cellSize, symbol);
            }
        }
    }

    // Draw enemies
    for (int i = 0; i < MAX_ENTITY; i++) {
        if (enemy[i].isAlive()) {
            Position enemyPos = enemy[i].getPosition();
            if (enemyPos.y >= viewportY && enemyPos.y < viewportY + viewportHeight &&
                enemyPos.x >= viewportX && enemyPos.x < viewportX + viewportWidth) {
                char symbol[2] = { 'E', '\0' };
                outtextxy((enemyPos.x - viewportX) * cellSize, (enemyPos.y - viewportY) * cellSize, symbol);
            }
        }
    }

    // Draw player
    char playerSymbol[2] = { 'P', '\0' };
    Position mcPos = player.getMCPosition();
    outtextxy((mcPos.x - viewportX) * cellSize, (mcPos.y - viewportY) * cellSize, playerSymbol);
}

void gameState::displayHUD() {
    char hudText[50];
    sprintf(hudText, "Health: %d / 10 Doom: %d / 50", player.getHealth(), player.getDoom());
    outtextxy(10, 180, hudText);
}

void gameState::gameLoop() {
    while (player.isAlive()) {
        drawMap();
        displayHUD();
        char input = getch();
        readInput(input);
    }
}

void gameState::readInput(char input) {
    // Calculate the new position based on input
    Position mcPos = player.getMCPosition();
    int newX = mcPos.x;
    int newY = mcPos.y;

    switch (input) {
    case 'w': newY -= 1; break;
    case 's': newY += 1; break;
    case 'a': newX -= 1; break;
    case 'd': newX += 1; break;
    }

    // Check for collision with walls and boundaries
    if (newX >= 0 && newX < 20 && newY >= 0 && newY < 10 && map[newY][newX] != '#') {
        // Check for collision with enemies
        for (int i = 0; i < MAX_ENTITY; i++) {
            Position enemyPos = enemy[i].getPosition();
            if (enemy[i].isAlive() && enemyPos.x == newX && enemyPos.y == newY) {
                // Transition to battle screen
                battleScreen(enemy[i], player);
                return;
            }
        }

        // Clear the player's old position
        map[mcPos.y][mcPos.x] = '.';

        // Move the player
        player.move(newX - mcPos.x, newY - mcPos.y);

        // Update mcPos to the new position
        mcPos = player.getMCPosition();

        // Set the player's new position
        map[mcPos.y][mcPos.x] = 'P';
    }

    // Update enemies' positions after player moves
    for (int i = 0; i < MAX_ENTITY; i++) {
        if (enemy[i].isAlive()) {
            Position enemyPos = enemy[i].getPosition();
            enemy[i].moveTowards(player.getMCPosition(), enemy, MAX_ENTITY);

            // Check for collision with the player after moving
            enemyPos = enemy[i].getPosition();
            if (enemyPos.x == mcPos.x && enemyPos.y == mcPos.y) {
                // Transition to battle screen
                battleScreen(enemy[i], player);
                return;
            }
        }
    }

    // Update the viewport
    updateViewport();
}

void gameState::updateViewport() {
    // Get the player's position
    Position mcPos = player.getMCPosition();

    // Center the viewport on the player
    viewportX = mcPos.x - viewportWidth / 2;
    viewportY = mcPos.y - viewportHeight / 2;

    // Ensure the viewport doesn't go out of bounds
    if (viewportX < 0) viewportX = 0;
    if (viewportY < 0) viewportY = 0;
    if (viewportX + viewportWidth > 20) viewportX = 20 - viewportWidth;
    if (viewportY + viewportHeight > 10) viewportY = 10 - viewportHeight;
}

void gameState::battleScreen(Enemy& enemy, MainCharacter& player) {
    cleardevice();
    outtextxy(100, 100, (char*)"Battle Start!");

    // Display battle options
    outtextxy(100, 150, (char*)"1. Attack");
    outtextxy(100, 170, (char*)"2. Defend");
    outtextxy(100, 190, (char*)"3. Item");
    outtextxy(100, 210, (char*)"4. Run");

    // Placeholder for user input handling
    char choice = getch(); // Wait for user input

    switch (choice) {
    case '1':
        // Attack logic (to be implroved)
        outtextxy(100, 250, (char*)"You chose to Attack!");
        enemy.takeDamage(player.attack());

        // Enemy attacks player if still alive
        if (enemy.isAlive()) {
            player.takeDamage(enemy.attack());
        }
        break;
    case '2':
        // Defend logic (to be implemented)
        outtextxy(100, 250, (char*)"You chose to Defend!");
        break;
    case '3':
        // Item logic (to be implemented)
        outtextxy(100, 250, (char*)"You chose to use an Item!");
        break;
    case '4':
        // Run logic (to be implemented)
        outtextxy(100, 250, (char*)"You chose to Run!");
        break;
    default:
        outtextxy(100, 250, (char*)"Invalid choice!");
        break;
    }

    getch(); // Wait for user input to continue

    // Return to the main game screen
    cleardevice();
    drawMap();
    displayHUD();
}
