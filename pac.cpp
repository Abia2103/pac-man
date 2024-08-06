#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 680
#define HEIGHT 480
#define PAC_SIZE 20
#define GHOST_SIZE 20
#define FOOD_SIZE 5
#define FOOD_COUNT 50

// Pac-Man attributes
int pacX = 100, pacY = 100, pacDir = 0; // 0: right, 1: down, 2: left, 3: up
int chomp = 1, score = 0;

// Ghost attributes
int ghostX[2], ghostY[2], ghostDir[2] = {0, 1}; // 0: right, 1: down, 2: left, 3: up

// Food attributes
int foodX[FOOD_COUNT], foodY[FOOD_COUNT];
int foodEaten[FOOD_COUNT] = {0};

// Function to draw Pac-Man
void drawPacMan(int x, int y, int size, int direction) {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);

    int start_angle, end_angle;
    if (chomp) {
        switch (direction) {
            case 0: start_angle = 30; end_angle = 330; break;
            case 1: start_angle = 120; end_angle = 420; break;
            case 2: start_angle = 210; end_angle = 510; break;
            case 3: start_angle = 300; end_angle = 600; break;
        }
        chomp = 0;
    } else {
        start_angle = 0;
        end_angle = 360;
        chomp = 1;
    }

    pieslice(x, y, start_angle, end_angle, size);
}

// Function to erase Pac-Man
void erasePacMan(int x, int y, int size) {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, size, size);
}

// Function to draw ghosts
void drawGhost(int x, int y, int size, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(x, y, size, size);
}

// Function to erase ghosts
void eraseGhost(int x, int y, int size) {
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, size, size);
}

// Function to draw food
void drawFood(int x, int y, int size) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y, size, size);
}

// Initialize food positions
void initializeFood() {
    srand(time(NULL));
    for (int i = 0; i < FOOD_COUNT; i++) {
        foodX[i] = rand() % (WIDTH - 2 * FOOD_SIZE) + FOOD_SIZE;
        foodY[i] = rand() % (HEIGHT - 2 * FOOD_SIZE) + FOOD_SIZE;
    }
}

// Check for collision with food
void checkFoodCollision() {
    for (int i = 0; i < FOOD_COUNT; i++) {
        if (!foodEaten[i] && abs(pacX - foodX[i]) < PAC_SIZE && abs(pacY - foodY[i]) < PAC_SIZE) {
            foodEaten[i] = 1;
            score += 10;
        }
    }
}

// Check for collision with ghosts
int checkGhostCollision() {
    for (int i = 0; i < 2; i++) {
        if (abs(pacX - ghostX[i]) < PAC_SIZE && abs(pacY - ghostY[i]) < PAC_SIZE) {
            return 1;
        }
    }
    return 0;
}

// Update ghost positions
void updateGhosts() {
    for (int i = 0; i < 2; i++) {
        eraseGhost(ghostX[i], ghostY[i], GHOST_SIZE);
        switch (ghostDir[i]) {
            case 0: ghostX[i] += 5; break;
            case 1: ghostY[i] += 5; break;
            case 2: ghostX[i] -= 5; break;
            case 3: ghostY[i] -= 5; break;
        }

        // Change direction randomly
        if (rand() % 20 == 0) {
            ghostDir[i] = rand() % 4;
        }

        // Wrap around screen
        if (ghostX[i] < 0) ghostX[i] = WIDTH;
        if (ghostX[i] > WIDTH) ghostX[i] = 0;
        if (ghostY[i] < 0) ghostY[i] = HEIGHT;
        if (ghostY[i] > HEIGHT) ghostY[i] = 0;

        drawGhost(ghostX[i], ghostY[i], GHOST_SIZE, RED);
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    initializeFood();
    ghostX[0] = 300; ghostY[0] = 200;
    ghostX[1] = 400; ghostY[1] = 300;

    while (1) {
        // Check for user input
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
                case 'w': pacDir = 3; break;
                case 's': pacDir = 1; break;
                case 'a': pacDir = 2; break;
                case 'd': pacDir = 0; break;
                case 27: closegraph(); return 0; // Esc to exit
            }
        }

        // Erase previous Pac-Man
        erasePacMan(pacX, pacY, PAC_SIZE);

        // Update Pac-Man position
        switch (pacDir) {
            case 0: pacX += 5; break;
            case 1: pacY += 5; break;
            case 2: pacX -= 5; break;
            case 3: pacY -= 5; break;
        }

        // Wrap around screen
        if (pacX < 0) pacX = WIDTH;
        if (pacX > WIDTH) pacX = 0;
        if (pacY < 0) pacY = HEIGHT;
        if (pacY > HEIGHT) pacY = 0;

        // Draw new Pac-Man
        drawPacMan(pacX, pacY, PAC_SIZE, pacDir);

        // Check for collisions
        checkFoodCollision();
        if (checkGhostCollision()) {
            outtextxy(WIDTH / 2 - 50, HEIGHT / 2, "Game Over!");
            delay(3000);
            break;
        }

        // Update ghosts
        updateGhosts();

        // Draw food
        for (int i = 0; i < FOOD_COUNT; i++) {
            if (!foodEaten[i]) {
                drawFood(foodX[i], foodY[i], FOOD_SIZE);
            }
        }

        // Display score
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        setcolor(WHITE);
        outtextxy(10, 10, scoreText);

        // Delay for smoother animation
        delay(50);
    }

    closegraph();
    return 0;
}