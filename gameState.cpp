#include "gameState.h"
#include "item.h"
#include <graphics.h>  
#include <cstdlib>
#include <set>
#include <ctime>

gameState::gameState(MainCharacter& player) : player(player), currentFloor(5) { // Initialize player at position (1, 1)

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
    std::srand(std::time(0)); // Seed for random number generation
    placeEnemiesRandomly();
    placeItemsRandomly();
}

//Go NextFLoor
void gameState::transitionToNextFloor() {

    savedDoomCounter = player.getDoom();

    // Decrease the floor number
    currentFloor--;

    // Generate a new floor (map layout)
    generateNewFloor();

    // Update the display of the current floor
    displayFloorNumber();
}

// Display Floor Number
void gameState::displayFloorNumber() {
    char hudText[50];
    sprintf(hudText, "Floor: %d", currentFloor);
    outtextxy(10, 550, hudText);
}

//Make Floor
void gameState::generateNewFloor() {
    // Create a new map layout
    const char newMap[10][20] = {
        "###################",
        "#.................#",
        "#.................#",
        "#.................#",
        "#.................#",
        "###################"
    };

    // Copy the new layout into the current map
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 20; ++j) {
            map[i][j] = newMap[i][j];
        }
    }

    // Reset player's position to (1, 1)
    player.setMCPosition(1, 1);
    Position mcPos = player.getMCPosition();
    map[mcPos.y][mcPos.x] = 'P';

    // Reset enemies for the new floor
    enemy[0] = Enemy(10, 3);

    // Place enemies randomly on the new map
    placeEnemiesRandomly();

    // Spawn the boss on the new floor
    spawnBoss();
    placeItemsRandomly();

    // Reload sprites or any other floor-specific assets
    loadSprites();
}

void gameState::loadSprites() {
    // Load player sprite
    readimagefile("asset/player.bmp", 0, 0, 16, 16);
    int size = imagesize(0, 0, 16, 16);
    playerSprite = malloc(size);
    getimage(0, 0, 16, 16, playerSprite);
    cleardevice();

    // Load albab sprite
    readimagefile("asset/Albab.bmp", 0, 0, 16, 16);
    size = imagesize(0, 0, 16, 16);
    albabSprite = malloc(size);
    getimage(0, 0, 16, 16, albabSprite);
    cleardevice();

    // Load medkit sprite
    readimagefile("asset/Medical.bmp", 0, 0, 16, 16);
    size = imagesize(0, 0, 16, 16);
    medicalSprite = malloc(size);
    getimage(0, 0, 16, 16, medicalSprite);
    cleardevice();

    // Load syringe sprite
    readimagefile("asset/Syringe.bmp", 0, 0, 16, 16);
    size = imagesize(0, 0, 16, 16);
    syringeSprite = malloc(size);
    getimage(0, 0, 16, 16, syringeSprite);
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
                if (mapX == 18 && mapY == 2) {
                    putimage(x * cellSize, y * cellSize, albabSprite, COPY_PUT);
                } else if (map[mapY][mapX] == 'M') {
                    putimage(x * cellSize, y * cellSize, medicalSprite, COPY_PUT);
                } else if (map[mapY][mapX] == 'S') {
                     putimage(x * cellSize, y * cellSize, syringeSprite, COPY_PUT);
                } else {
                    char symbol[2] = { map[mapY][mapX], '\0' };
                    outtextxy(x * cellSize, y * cellSize, symbol);
                }
            }
        }
    }

    // Draw player
    Position mcPos = player.getMCPosition();
    int scaledX = (mcPos.x - viewportX) * cellSize;
    int scaledY = (mcPos.y - viewportY) * cellSize;
    int scaledSize = cellSize;
    putimage(scaledX, scaledY, playerSprite, COPY_PUT);

    // Draw Boss
    if (boss.isAlive()) {
        Position bossPos = boss.getBossPosition();
        int bossScaledX = (bossPos.x - viewportX) * cellSize;
        int bossScaledY = (bossPos.y - viewportY) * cellSize;
        putimage(bossScaledX, bossScaledY, bossSprite1, COPY_PUT);
    }

    
}

void gameState::displayHUD() {
    // Get the current health, max health, and doom values
    int currentHealth = player.getHealth();
    int maxHealth = player.getMaxHealth(); // Use the getter method
    int doom = player.getDoom();

    // Ensure health does not exceed max health
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }

    // Display health and doom values
    char hudText[50];
    sprintf(hudText, "Health: %d / %d Doom: %d / 50", player.getHealth(), maxHealth, player.getDoom());
    outtextxy(10, 600, hudText);

    // Display points
    char pointsText[50];
    sprintf(pointsText, "Points: %d", player.getGamePoints());
    outtextxy(300, 600, pointsText);
}

void gameState::gameLoop() {
    while (player.isAlive()) {
        drawMap();
        displayHUD();
        displayFloorNumber(); // Ensure floor number is displayed in the game loop
        player.getGamePoints(); // Ensure points are updated in the game loop
        char input = getch();
        readInput(input);

        if (currentFloor == 0) {
            endGame();
            break;
        }

        if (!player.isAlive()) {
            endGame();
            break;
        }
        
    }
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
    while(enemy.isAlive() && player.isAlive()) {

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
        int tableLeft = 80, tableTop = 400, tableRight = 250, tableBottom = tableTop + (3 * 30);
        rectangle(tableLeft, tableTop, tableRight, tableBottom); // Outer table border

        // Draw horizontal lines to create rows
        int rowHeight = 30;
        int numberOfRows = 3;
        for (int i = 1; i <= numberOfRows; ++i) {
            int y = tableTop + i * rowHeight;
            line(tableLeft, y, tableRight, y);
        }

        // Add text inside the table rows
        outtextxy((tableLeft + tableRight) / 2 - 50, tableTop - 30, (char*)"Battle Options");
        
        int textXOffset = tableLeft + 10; // Indent text slightly
        outtextxy(textXOffset, tableTop + 5, (char*)"1. Attack");
        outtextxy(textXOffset, tableTop + rowHeight + 5, (char*)"2. Defend");
        outtextxy(textXOffset, tableTop + 2 * rowHeight + 5, (char*)"3. Run");
        
        // Placeholder for user input handling
        char choice = getch(); // Wait for user input

        switch (choice) {
        case '1': {
            // Attack logic (to be improved)
            outtextxy(100, 300, (char*)"You chose to Attack!");
            enemy.takeDamage(player.attack());

            // Enemy attacks player if still alive
            if (enemy.isAlive()) {
                player.takeDamage(enemy.attack());
            } else {
                outtextxy(100, 335, (char*)"Enemy defeated!");
                player.doomDecrease(1);  // Doom decreases only if enemy is defeated
                player.addGamePoints(5); // Add 5 points when enemy is defeated
            }
            break;
        }
        case '2': {
            // Defend logic
           outtextxy(100, 300, (char*)"You chose to Defend!");

            int defenseValue = player.getIntelligence(); // Defense based on intelligence
            int incomingDamage = enemy.attack();
            int damageToPlayer;

            // Calculate success chance based on intelligence
            int successChance = defenseValue * 10; // Example: 10% success chance per intelligence point
            int randomValue = std::rand() % 100; // Random value between 0 and 99

            if (randomValue < successChance) {
                // Successful defense
                damageToPlayer = incomingDamage - defenseValue;

                if (damageToPlayer < 0) {
                    int deflectedDamage = 1; // Deflect only 1 damage
                    damageToPlayer = 0; // No damage to player
                    enemy.takeDamage(deflectedDamage); // Deflect damage back to enemy
                    outtextxy(100, 300, (char*)"You successfuly Defend!");
                }
            } else {
                // Unsuccessful defense, take full damage
                damageToPlayer = incomingDamage;
                outtextxy(100, 300, (char*)"You failed to Defend!");
            }

            player.takeDamage(damageToPlayer);
            break;
        }
        case '3': { // Run
            if (attemptRun()) {
                enemy.takeDamage(999); // Instantly defeat the enemy
                return; // Return to main game loop
            } else {
                // Enemy attacks player
                if(enemy.isAlive()) {
                    player.takeDamage(enemy.attack());
                }
                break;
            }
        }
        default: {
            outtextxy(100, 300, (char*)"Invalid choice!");
            break;
        }
        }

        getch(); // Wait for user input to continue

        if (player.doomed()) {
            outtextxy(100, 300, (char*)"You have been defeated by Doom!");
            endGame();
            break;  // End the game
        }

        // Return to the main game screen
        cleardevice();
        drawMap();
        displayHUD();

    }
    
}


void gameState::battleScreenBoss(Boss& boss, MainCharacter& player) {
    while (boss.isAlive() && player.isAlive()) {
        cleardevice();
        
        // Display character and boss health
        string playerHealthText = "Player Health: " + to_string(player.getHealth());
        outtextxy(80, 250, (char*)playerHealthText.c_str());
        string doomMeterText = "Doom Meter: " + to_string(player.getDoom());
        outtextxy(80, 270, (char*)doomMeterText.c_str());
        string bossHealthText = "Boss Health: " + to_string(boss.getHealth());
        outtextxy(500, 250, (char*)bossHealthText.c_str());

        // Draw Battle Scene / Player & Enemy
        int battleBoxLeft = 80, battleBoxTop = 10, battleBoxRight = 650, battleBoxBottom = 235;
        rectangle(battleBoxLeft, battleBoxTop, battleBoxRight, battleBoxBottom); // Outer box for health info

        //Player Asset
        readimagefile("asset/player.bmp", 100, 130, 130, 150);  // You can adjust the size of the image to match your layout
        int size = imagesize(100, 130, 130, 150);
        void* playerSprite = malloc(size);
        getimage(100, 130, 130, 150, playerSprite);

        //Boss Asset
        char BossSpritePath[10];
        sprintf(BossSpritePath, "asset/boss%d.bmp", currentFloor);
        readimagefile(BossSpritePath, 400, 50, 180, 190);
        size = imagesize(400, 50, 180, 190);
        bossSprite1 = malloc(size);
        getimage(400, 50, 180, 190, bossSprite1);

        int tableLeft = 80, tableTop = 400, tableRight = 250, tableBottom = tableTop + (3 * 30);
        rectangle(tableLeft, tableTop, tableRight, tableBottom); // Outer table border

        // Draw horizontal lines to create rows
        int rowHeight = 30;
        int numberOfRows = 3;
        for (int i = 1; i <= numberOfRows; ++i) {
            int y = tableTop + i * rowHeight;
            line(tableLeft, y, tableRight, y);
        }

        // Add text inside the table rows
        outtextxy((tableLeft + tableRight) / 2 - 50, tableTop - 30, (char*)"Battle Options");
        
        int textXOffset = tableLeft + 10; // Indent text slightly
        outtextxy(textXOffset, tableTop + 5, (char*)"1. Attack");
        outtextxy(textXOffset, tableTop + rowHeight + 5, (char*)"2. Defend");
        outtextxy(textXOffset, tableTop + 2 * rowHeight + 5, (char*)"3. Run");

        // Placeholder for user input handling
        char choice = getch(); // Wait for user input

        switch (choice) {
        case '1':
            // Attack logic
            outtextxy(100, 300, (char*)"You chose to Attack!");
            boss.takeDamage(player.attack());

            // Boss attacks player if still alive
            if (boss.isAlive()) {
                player.takeDamage(boss.attack());
            }
            break;
        case '2':
            // Defend logic (to be implemented)
            outtextxy(100, 300, (char*)"You chose to Defend!");
            break;
        case '3':
            // Run logic (to be implemented)
            outtextxy(100, 300, (char*)"You can't RUN Coward!");
            break;
        default:
            outtextxy(100, 300, (char*)"Invalid choice!");
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
        endGame();
    }

    getch(); // Wait for user input to continueaaaaaaaaaaaaaaaaa

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

    if (roll < escapeChance) {
        outtextxy(100, 300, (char*)"You successfully ran away!");
        getch();
    } else {
        outtextxy(100, 300, (char*)"You failed to escape!");
        getch();
    }
    
    return roll < escapeChance;
    
}

void gameState::placeEnemiesRandomly() {
    for (int i = 0; i < MAX_ENTITY; ++i) {
        int x = std::rand() % 20;
        int y = std::rand() % 10;
        enemy[i] = Enemy(x, y);
        enemyPlaced[i] = false; // Enemies are not placed initially
    }
}

void gameState::placeMedkitRandomly(std::set<std::pair<int, int>>& usedPositions) {
    for (int i = 0; i < MAX_MEDKIT; ++i) {
        int x, y;
        std::pair<int, int> position;
        do {
            x = std::rand() % 18 + 1; // Ensure within wall bounds
            y = std::rand() % 4 + 1; // Ensure within wall bounds
            position = std::make_pair(x, y);
        } while (usedPositions.find(position) != usedPositions.end() || map[y][x] == '#');
        
        usedPositions.insert(position);
        medkit[i] = Medkit(x, y);
        medkitPlaced[i] = true;
        Position medkitPos = medkit[i].getMedKitPosition();
        map[medkitPos.y][medkitPos.x] = 'M'; // Represent medkit on the map
    }
}

void gameState::placeSyringeRandomly(std::set<std::pair<int, int>>& usedPositions) {
    for (int i = 0; i < MAX_SYRINGE; ++i) {
        int x, y;
        std::pair<int, int> position;
        do {
            x = std::rand() % 18 + 1; // Ensure within wall bounds
            y = std::rand() % 4 + 1; // Ensure within wall bounds
            position = std::make_pair(x, y);
        } while (usedPositions.find(position) != usedPositions.end() || map[y][x] == '#');
        
        usedPositions.insert(position);
        syringe[i] = Syringe(x, y);
        syringePlace[i] = true;
        Position syringePos = syringe[i].getSyringePosition();
        map[syringePos.y][syringePos.x] = 'S'; // Represent syringe on the map
    }
}

void gameState::placeItemsRandomly() {
    std::set<std::pair<int, int>> usedPositions;
    placeMedkitRandomly(usedPositions);
    placeSyringeRandomly(usedPositions);
}

void gameState::checkRandomEncounter() {
    for (int i = 0; i < MAX_ENTITY; ++i) {
        if (!enemyPlaced[i] && (std::rand() % 100 < 40)) { // 10% chance to place an enemy
            enemyPlaced[i] = true;
        }
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
    if (newX >= 0 && newX < 20 && newY >= 0 && newY < 10 && (map[newY][newX] != '#' || (newX == 18 && newY == 2))) {

        // Check if the player reaches the far right of the map at y = 2
        if (newX == 18 && newY == 2) { 
            if (!boss.isAlive()) {
                // Transition to the next floor
                transitionToNextFloor();
                return;  // Exit the current movement logic
            } else {
                // Prevent moving out of bounds if the boss is alive
                return;
            }
        }

        if (std::rand() % 100 < 40) {
            int enemyIndex = std::rand() % MAX_ENTITY;
            battleScreen(enemy[enemyIndex], player);
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

    // Check for collision with the boss after player moves
    Position bossPos = boss.getBossPosition();
    if (boss.isAlive() && bossPos.x == mcPos.x && bossPos.y == mcPos.y) {
        // Transition to battle screen
        battleScreenBoss(boss, player);
        if (!boss.isAlive() && bossPos.x == mcPos.x && bossPos.y == mcPos.y) {
            player.addGamePoints(20);
        }
        return;
    }

    // Check for collision with medkits after player moves
    for (int i = 0; i < MAX_MEDKIT; ++i) {
        if (medkitPlaced[i]) {
            Position medkitPos = medkit[i].getMedKitPosition();
            if (mcPos.x == medkitPos.x && mcPos.y == medkitPos.y) {
                player.heal(medkit[i].getAmountHeal());
                medkitPlaced[i] = false; // Remove medkit after use
                map[medkitPos.y][medkitPos.x] = '.'; // Clear medkit from map
            }
        }
    }

    for (int i = 0; i < MAX_SYRINGE; ++i) {
        if (syringePlace[i]) {
            Position syringePos = syringe[i].getSyringePosition();
            if (mcPos.x == syringePos.x && mcPos.y == syringePos.y) {
                player.doomDecrease(syringe[i].getAmountDoomDec());
                syringePlace[i] = false; // Remove medkit after use
                map[syringePos.y][syringePos.x] = '.'; // Clear medkit from map
            }
        }
    }

    // Update the viewport
    updateViewport();
}

void gameState::endGame() {
    cleardevice();
    int screenWidth = getmaxx();
    int screenHeight = getmaxy();
    if (!player.isAlive()) {

        const char* message = "Too bad! You have died never reaching the bottom floor...";
        int textWidth = textwidth((char*)message);
        int textHeight = textheight((char*)message);
        int x = (screenWidth - textWidth) / 2;
        int y = (screenHeight - textHeight) / 2;
        outtextxy(x, y, (char*)message);
        getch(); // Wait for user input to close the game
        
    } else {
        const char* message = "Congratulations! You have reached the bottom floor..." ;
        int textWidth = textwidth((char*)message);
        int textHeight = textheight((char*)message);
        int x = (screenWidth - textWidth) / 2;
        int y = (screenHeight - textHeight) / 2;
        outtextxy(x, y, (char*)message);
        getch(); // Wait for user input to close the game
    }
    

}

void gameState::loadBossSprite(int floor) {
    if (bossSprite1) {
        free(bossSprite1); // Free the previous sprite memory
    }
    char spritePath[50];
    sprintf(spritePath, "asset/boss%d.bmp", floor);
    readimagefile(spritePath, 0, 0, 16, 16);
    int size = imagesize(0, 0, 16, 16);
    bossSprite1 = malloc(size);
    getimage(0, 0, 16, 16, bossSprite1);
    cleardevice();
}

void gameState::spawnBoss() {
    boss = Boss(15, 2); 
    loadBossSprite(currentFloor);
}