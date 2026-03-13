#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SQUARES_PER_SIDE 10
#define TOTAL_SQUARES 40
#define ANIM_SPEED 10.0f  // Vitesse de glissement (Lerp)

Vector2 GetBoardCoordinates(int index, int cellSize) {
    int side = index / 10;
    int offset = index % 10;
    Vector2 pos = { 0, 0 };
    switch (side) {
        case 0: pos.x = SCREEN_WIDTH - (offset * cellSize) - cellSize; pos.y = SCREEN_HEIGHT - cellSize; break;
        case 1: pos.x = 0; pos.y = SCREEN_HEIGHT - (offset * cellSize) - cellSize; break;
        case 2: pos.x = offset * cellSize; pos.y = 0; break;
        case 3: pos.x = SCREEN_WIDTH - cellSize; pos.y = offset * cellSize; break;
    }
    return (Vector2){ pos.x + cellSize/2.0f, pos.y + cellSize/2.0f };
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monopoly - Animation Pas à Pas");
    SetTargetFPS(60);

    int cellSize = SCREEN_WIDTH / (SQUARES_PER_SIDE + 1);
    int playerLogicPos = 0;      // La case où le joueur va arriver
    int lastLogicPos = 0;        // La case d'où il vient (pour l'animation)
    int squaresToMove = 0;       // Nombre de cases restant à parcourir
    int lastRoll = 0;

    Vector2 visualPos = GetBoardCoordinates(0, cellSize);

    while (!WindowShouldClose()) {
        // 1. INPUT : On lance les dés
        if (IsKeyPressed(KEY_SPACE) && squaresToMove == 0) { // On ne relance pas si on bouge déjà
            lastRoll = GetRandomValue(1, 6) + GetRandomValue(1, 6);
            squaresToMove = lastRoll;
        }

        // 2. LOGIQUE D'ANIMATION
        Vector2 targetPos = GetBoardCoordinates((lastLogicPos + 1) % TOTAL_SQUARES, cellSize);
        
        if (squaresToMove > 0) {
            // On déplace visuellement le pion vers la PROCHAINE case
            visualPos = Vector2Lerp(visualPos, targetPos, ANIM_SPEED * GetFrameTime());

            // Si on est arrivé très proche de la case suivante
            if (Vector2Distance(visualPos, targetPos) < 1.0f) {
                visualPos = targetPos;         // On plaque la position pile sur la case
                lastLogicPos = (lastLogicPos + 1) % TOTAL_SQUARES; // On valide le passage sur la case
                squaresToMove--;               // Une case de moins à faire
            }
        }
        
        playerLogicPos = lastLogicPos;

        // 3. DESSIN
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < TOTAL_SQUARES; i++) {
            Vector2 p = GetBoardCoordinates(i, cellSize);
            DrawRectangleLines(p.x - cellSize/2, p.y - cellSize/2, cellSize, cellSize, BLACK);
            DrawText(TextFormat("%d", i), p.x - cellSize/2 + 5, p.y - cellSize/2 + 5, 10, GRAY);
        }

        DrawCircleV(visualPos, 15, GREEN);

        DrawText("ESPACE pour lancer les des", 280, 350, 20, DARKGRAY);
        DrawText(TextFormat("Dernier lancer: %d", lastRoll), 320, 380, 20, MAROON);
        DrawText(TextFormat("Cases restantes: %d", squaresToMove), 310, 410, 20, BLUE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}