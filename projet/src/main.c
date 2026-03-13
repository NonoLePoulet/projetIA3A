#include "raylib.h"
#include "raymath.h"
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SQUARES_PER_SIDE 10
#define TOTAL_SQUARES 40
#define ANIMATION_SPEED 20.0f


// Function to calculate the X and Y screen coordinates based on board index (0-39)
Vector2 GetBoardCoordinates(int index, int cellSize) {
    int side = index / 10;
    int offset = index % 10;
    Vector2 pos = { 0, 0 };

    switch (side) {
        case 0: // Bottom side (Right to Left)
            pos.x = SCREEN_WIDTH - (offset * cellSize) - cellSize;
            pos.y = SCREEN_HEIGHT - cellSize;
            break;
        case 1: // Left side (Bottom to Top)
            pos.x = 0;
            pos.y = SCREEN_HEIGHT - (offset * cellSize) - cellSize;
            break;
        case 2: // Top side (Left to Right)
            pos.x = offset * cellSize;
            pos.y = 0;
            break;
        case 3: // Right side (Top to Bottom)
            pos.x = SCREEN_WIDTH - cellSize;
            pos.y = offset * cellSize;
            break;
    }
    return pos;
}

int playermove(int playerPos, int cellSize){
     // Current square index
    Vector2 currentVisualpos = GetBoardCoordinates(playerPos, cellSize);
    playerPos = (playerPos + 1) % TOTAL_SQUARES;
    Vector2 playerWorldPos = GetBoardCoordinates(playerPos, cellSize);
        if (Vector2Distance(currentVisualpos, playerWorldPos) > 0.1f) {
            // Vector2Lerp moves a percentage of the remaining distance every frame
            currentVisualpos = Vector2Lerp(currentVisualpos, playerWorldPos, ANIMATION_SPEED * GetFrameTime());
        }
    DrawCircle(currentVisualpos.x + cellSize/2, currentVisualpos.y + cellSize/2, 15, GREEN);
    return playerPos;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib - Simple Monopoly Board");
    SetTargetFPS(60);
    int cellSize = SCREEN_WIDTH / (SQUARES_PER_SIDE + 1);
    int playerPos = 0;
    while (!WindowShouldClose()) {
        // Update: Move player on Space press
        int roll ; 
        if (IsKeyPressed(KEY_SPACE)) {
            roll = (GetRandomValue(1, 6) + GetRandomValue(1, 6));
            for(int i = 0 ; i<roll ; i++ ){
                playerPos = playermove(playerPos ,cellSize);
            }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 1. Draw the Board Squares
        for (int i = 0; i < TOTAL_SQUARES; i++) {
            Vector2 pos = GetBoardCoordinates(i, cellSize);
            
            // Draw square border
            DrawRectangleLines(pos.x, pos.y, cellSize, cellSize, BLACK);
            
            // Label the squares (optional)
            DrawText(TextFormat("%d", i), pos.x + 5, pos.y + 5, 10, GRAY);
        }
        
        Vector2 currentVisualpos = GetBoardCoordinates(playerPos,cellSize);
        DrawCircle(currentVisualpos.x + cellSize/2, currentVisualpos.y + cellSize/2, 15, GREEN);

        // UI Instructions
        DrawText("Press SPACE to move", SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2, 20, DARKGRAY);
        DrawText(TextFormat("Square: %d", playerPos), SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2 + 30, 20, MAROON);
        DrawText(TextFormat("You rolled: %d", roll), SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2 + 50, 20, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}