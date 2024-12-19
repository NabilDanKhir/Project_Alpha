#include "gameState.h"
#include <graphics.h>  

gameState::gameState() : player(1, 1) { // Initialize player at position (1, 1)
player.allocateInitialPoints(); // Prompt the player to allocate their initial stats

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

   
    initializeBoss();  // Initialize the boss
    loadSprites();
}

void gameState::loadSprites() {
    
    readimagefile("asset/player.bmp", 0, 0, 16, 16);
    int size = imagesize(0, 0, 16, 16);
    playerSprite = malloc(size);
    getimage(0, 0, 16, 16, playerSprite);
    cleardevice();

    readimagefile("asset/enemy.bmp", 0, 0, 16, 16);
    size = imagesize(0, 0, 16, 16);
    mobSprite = malloc(size);
    getimage(0, 0, 16, 16, mobSprite);
    cleardevice();

    readimagefile("asset/enemy.bmp", 0, 0, 16, 16);
    size = imagesize(0, 0, 16, 16);
    bossSprite1 = malloc(size);
    getimage(0, 0, 16, 16, bossSprite1);
    cleardevice();
}


//Draw MAP
void gameState::drawMap() {
    cleardevice();
    int cellSize = min(getmaxx() / viewportWidth, getmaxy() / viewportHeight);
    for (int y = 0; y < viewportHeight; y++) {
        for (int x = 0; x < viewportWidth; x++) {
            int mapX = viewportX + x;
            int mapY = viewportY + y;
            if (mapY < 20 && mapX < 20) { // Ensure within bounds
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
                int scaledX = (enemyPos.x - viewportX) * cellSize;
                int scaledY = (enemyPos.y - viewportY) * cellSize;
                int scaledSize = cellSize;
                putimage(scaledX, scaledY, mobSprite, COPY_PUT);
            }
        }
    }

    // Draw player
    Position mcPos = player.getMCPosition();
    int scaledX = (mcPos.x - viewportX) * cellSize;
    int scaledY = (mcPos.y - viewportY) * cellSize;
    int scaledSize = cellSize;
    putimage(scaledX, scaledY, playerSprite, COPY_PUT);

    //Draw Boss
    if (boss.isAlive()) {
        Position bossPos = boss.getBossPosition();
        int bossScaledX = (bossPos.x - viewportX) * cellSize;
        int bossScaledY = (bossPos.y - viewportY) * cellSize;
        int scaledSize = cellSize;
        putimage(bossScaledX, bossScaledY, bossSprite1, COPY_PUT);
    }
    
}

void gameState::displayHUD() {
    char hudText[50];
    sprintf(hudText, "Health: %d / 10 Doom: %d / 50", player.getHealth(), player.getDoom());
    outtextxy(10, 600, hudText);
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

    // Check for collision with the boss after player moves
    Position bossPos = boss.getBossPosition();
    if (boss.isAlive() && bossPos.x == mcPos.x && bossPos.y == mcPos.y) {
        // Transition to battle screen
        battleScreenBoss(boss, player);
        return;
    }

    // Update the viewport
    updateViewport();
}

void gameState::initializeBoss() {
    boss.getBossPosition(); //Set initial boss position
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
   
    // Display character and enemy health
    string playerHealthText = "Player Health: " + to_string(player.getHealth());
    outtextxy(80, 250, (char*)playerHealthText.c_str());
    string doomMeterText = "Doom Meter: " + to_string(player.getDoom());
    outtextxy(80, 270, (char*)doomMeterText.c_str());
    string enemyHealthText = "Enemy Health: " + to_string(enemy.getHealth());
    outtextxy(450, 250, (char*)enemyHealthText.c_str());

    // Draw Battle Scene / Player & Enemy
    int battleBoxLeft = 80, battleBoxTop = 10, battleBoxRight = 650, battleBoxBottom = 235;
    rectangle(battleBoxLeft, battleBoxTop, battleBoxRight, battleBoxBottom); // Outer box for health info

    //Player Assets
    readimagefile("asset/player.bmp", 100, 60, 180, 190);  // You can adjust the size of the image to match your layout
    int size = imagesize(100, 60, 180, 190);
    void* playerSprite = malloc(size);
    getimage(100, 60, 180, 190, playerSprite);

    //Enemy Assets
    readimagefile("asset/enemy.bmp", 400, 50, 180, 190);
    size = imagesize(400, 50, 180, 190);
    void* mobSprite = malloc(size);
    getimage(400, 50, 180, 190, mobSprite);

    // Draw the table box
    int tableLeft = 80, tableTop = 400, tableRight = 250, tableBottom = tableTop + (4 * 30);
    rectangle(tableLeft, tableTop, tableRight, tableBottom); // Outer table border

    // Draw horizontal lines to create rows
    int rowHeight = 30;
    for (int y = tableTop + rowHeight; y < tableBottom; y += rowHeight) {
        line(tableLeft, y, tableRight, y);
    }

    // Add text inside the table rows
    outtextxy((tableLeft + tableRight) / 2 - 50, tableTop - 30, (char*)"Battle Options");
    
    int textXOffset = tableLeft + 10; // Indent text slightly
    outtextxy(textXOffset, tableTop + 5, (char*)"1. Attack");
    outtextxy(textXOffset, tableTop + rowHeight + 5, (char*)"2. Defend");
    outtextxy(textXOffset, tableTop + 2 * rowHeight + 5, (char*)"3. Item");
    outtextxy(textXOffset, tableTop + 3 * rowHeight + 5, (char*)"4. Run");
    
    // Placeholder for user input handling
    char choice = getch(); // Wait for user input

    switch (choice) {
    case '1':
        // Attack logic (to be implroved)
        outtextxy(100, 300, (char*)"You chose to Attack!");
        enemy.takeDamage(player.attack());

        // Enemy attacks player if still alive
        if (enemy.isAlive()) {
            player.takeDamage(enemy.attack());
        }
        break;
    case '2':
        // Defend logic (to be implemented)
        outtextxy(100, 300, (char*)"You chose to Defend!");
        break;
    case '3':
        // Item logic (to be implemented)
        outtextxy(100, 300, (char*)"You chose to use an Item!");
        break;
    case '4': // Run
        if (attemptRun()) {
        outtextxy(100, 300, (char*)"You successfully ran away!");
        // Return to main game loop
        return;
        } else {
        outtextxy(100, 300, (char*)"You failed to escape!");
        // Enemy attacks player
        player.takeDamage(enemy.attack());
        }
        break;
    default:
        outtextxy(100, 300, (char*)"Invalid choice!");
        break;
    }

    getch(); // Wait for user input to continue

    // Return to the main game screen
    cleardevice();
    drawMap();
    displayHUD();
}


void gameState::battleScreenBoss(Boss& boss, MainCharacter& player) {
    while (boss.isAlive() && player.isAlive()) {
        cleardevice();
        
        // Display character and boss health
        string playerHealthText = "Player Health: " + to_string(player.getHealth());
        outtextxy(80, 250, (char*)playerHealthText.c_str());
        string bossHealthText = "Boss Health: " + to_string(boss.getHealth());
        outtextxy(500, 250, (char*)bossHealthText.c_str());

        // Draw Battle Scene / Player & Enemy
        int battleBoxLeft = 80, battleBoxTop = 10, battleBoxRight = 650, battleBoxBottom = 235;
        rectangle(battleBoxLeft, battleBoxTop, battleBoxRight, battleBoxBottom); // Outer box for health info

        //Player Asset
        readimagefile("asset/player.bmp", 0, 0, 16, 16);  // You can adjust the size of the image to match your layout
        int size = imagesize(0, 0, 64, 64);
        void* playerSprite = malloc(size);
        getimage(0, 0, 64, 64, playerSprite);

        // Draw the table box
        int tableLeft = 80, tableTop = 400, tableRight = 250, tableBottom = tableTop + (4 * 30);
        rectangle(tableLeft, tableTop, tableRight, tableBottom); // Outer table border

        // Draw horizontal lines to create rows
        int rowHeight = 30;
        for (int y = tableTop + rowHeight; y < tableBottom; y += rowHeight) {
            line(tableLeft, y, tableRight, y);
        }

        // Add text inside the table rows
        outtextxy((tableLeft + tableRight) / 2 - 50, tableTop - 30, (char*)"Battle Options");
        
        int textXOffset = tableLeft + 10; // Indent text slightly
        outtextxy(textXOffset, tableTop + 5, (char*)"1. Attack");
        outtextxy(textXOffset, tableTop + rowHeight + 5, (char*)"2. Defend");
        outtextxy(textXOffset, tableTop + 2 * rowHeight + 5, (char*)"3. Item");
        outtextxy(textXOffset, tableTop + 3 * rowHeight + 5, (char*)"4. Run");

        // Placeholder for user input handling
        char choice = getch(); // Wait for user input

        switch (choice) {
        case '1':
            // Attack logic
            outtextxy(100, 250, (char*)"You chose to Attack!");
            boss.takeDamage(player.attack());

            // Boss attacks player if still alive
            if (boss.isAlive()) {
                player.takeDamage(boss.attack());
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
            return; // Exit the battle
        default:
            outtextxy(100, 250, (char*)"Invalid choice!");
            break;
        }

        getch(); // Wait for user input to continue
    }

    cleardevice();
    if (!boss.isAlive()) {
        map[boss.getBossPosition().y][boss.getBossPosition().x] = '.'; // Remove boss from map
        outtextxy(100, 300, (char*)"You defeated the Boss!");
    } else if (!player.isAlive()) {
        outtextxy(100, 300, (char*)"You were defeated by the Boss!");
    }

    getch(); // Wait for user input to continue

    // Return to the main game screen
    cleardevice();
    drawMap();
    displayHUD();
}

bool gameState::attemptRun() {
    int playerAgility = player.getAgility(); // Get the player's agility
    int escapeChance = 0;

    if (playerAgility < 2) {
        escapeChance = 20; // Low chance to escape
    } else if (playerAgility < 4) {
        escapeChance = 50; // Medium chance to escape
    } else if (playerAgility <= 6) {
        escapeChance = 80; // High chance to escape
    } else {
        escapeChance = 100; // Guaranteed escape
    }

    int roll = std::rand() % 100; // Roll a random number between 0 and 99
    return roll < escapeChance;
}