#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SQUARES_PER_SIDE 10
#define TOTAL_SQUARES 40

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

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib - Simple Monopoly Board");
    SetTargetFPS(60);

    int playerPos = 0; // Current square index
    int cellSize = SCREEN_WIDTH / (SQUARES_PER_SIDE + 1);

    while (!WindowShouldClose()) {
        // Update: Move player on Space press
        if (IsKeyPressed(KEY_SPACE)) {
            playerPos = (playerPos + 1) % TOTAL_SQUARES;
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

        // 2. Draw the Player
        Vector2 playerWorldPos = GetBoardCoordinates(playerPos, cellSize);
        DrawCircle(playerWorldPos.x + cellSize/2, playerWorldPos.y + cellSize/2, 15, RED);

        // UI Instructions
        DrawText("Press SPACE to move", SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2, 20, DARKGRAY);
        DrawText(TextFormat("Square: %d", playerPos), SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2 + 30, 20, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}