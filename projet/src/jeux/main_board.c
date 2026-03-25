#include "raylib.h"
#include "raymath.h"

#include "main_shooter.c"
#include "main_casino.c"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SQUARES_PER_SIDE 10
#define TOTAL_SQUARES 40
#define ANIM_SPEED 10.0f  // Vitesse de glissement (Lerp)

#define SHOOTER_TILE   10   // landing here launches the shooter mini-game
#define CASINO_TILE_1  20   // landing here launches the casino mini-game
#define CASINO_TILE_2  35 

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

static const char* TileLabel(int index)
{
    if (index == SHOOTER_TILE)  return "SHOOT";
    if (index == CASINO_TILE_1) return "CASINO";
    if (index == CASINO_TILE_2) return "CASINO";
    return NULL;
}

static void LaunchMiniGame(int tile)
{
    // Give the player a moment to read which tile they landed on,
    // then close the board window before opening the mini-game window.
    CloseWindow();
 
    if (tile == SHOOTER_TILE) {
        shooter();
    } else if (tile == CASINO_TILE_1 || tile == CASINO_TILE_2) {
        main_casino();
    }
 
    // Restore the board window after the mini-game closes
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monopoly - Animation Pas à Pas");
    SetTargetFPS(60);
}

int main_board() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monopoly - Animation Pas à Pas");
    SetTargetFPS(60);

    int cellSize = SCREEN_WIDTH / (SQUARES_PER_SIDE + 1);
    int playerLogicPos = 0;      // La case où le joueur va arriver
    int lastLogicPos = 0;        // La case d'où il vient (pour l'animation)
    int squaresToMove = 0;       // Nombre de cases restant à parcourir
    int lastRoll = 0;

    bool justLanded    = false;
    int  pendingLaunch = -1;
    int  launchCountdown = 0; 

    Vector2 visualPos = GetBoardCoordinates(0, cellSize);

    while (!WindowShouldClose()) {
        // 1. INPUT : On lance les dés
        if (IsKeyPressed(KEY_SPACE) && squaresToMove == 0) { // On ne relance pas si on bouge déjà
            lastRoll = GetRandomValue(1, 6) + GetRandomValue(1, 6);
            squaresToMove = lastRoll;
            justLanded    = false;
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

                if (squaresToMove == 0)
                    justLanded = true;  // we just stepped onto our final tile
            }
        }
        
        playerLogicPos = lastLogicPos;

        if (justLanded)
        {
            justLanded = false;
 
            if (playerLogicPos == SHOOTER_TILE  ||
                playerLogicPos == CASINO_TILE_1 ||
                playerLogicPos == CASINO_TILE_2)
            {
                // Small countdown so the player sees the "you landed here" text
                pendingLaunch    = playerLogicPos;
                launchCountdown  = 120; // 2 seconds at 60 FPS
            }
        }
        if (pendingLaunch >= 0)
        {
            launchCountdown--;
            if (launchCountdown <= 0)
            {
                int tile      = pendingLaunch;
                pendingLaunch = -1;
                LaunchMiniGame(tile);
                // After the mini-game window is closed and the board window
                // is re-created, the while loop continues normally.
                continue;
            }
        }

        // 3. DESSIN
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < TOTAL_SQUARES; i++) {
            Color tint = RAYWHITE;
            if (i == SHOOTER_TILE)                         tint = (Color){ 255, 180, 180, 255 }; // red-ish
            else if (i == CASINO_TILE_1 || i == CASINO_TILE_2) tint = (Color){ 180, 220, 255, 255 }; // blue-ish
            Vector2 p = GetBoardCoordinates(i, cellSize);
            DrawRectangle(p.x - cellSize/2, p.y - cellSize/2, cellSize, cellSize, tint);
            DrawRectangleLines(p.x - cellSize/2, p.y - cellSize/2, cellSize, cellSize, BLACK);
            DrawText(TextFormat("%d", i), p.x - cellSize/2 + 5, p.y - cellSize/2 + 5, 10, GRAY);
        }

        DrawCircleV(visualPos, 15, GREEN);

        DrawText("ESPACE pour lancer les des", 280, 350, 20, DARKGRAY);
        DrawText(TextFormat("Dernier lancer: %d", lastRoll), 320, 380, 20, MAROON);
        DrawText(TextFormat("Cases restantes: %d", squaresToMove), 310, 410, 20, BLUE);
        DrawText(TextFormat("Case actuelle   : %d", playerLogicPos), 315, 440, 20, DARKGREEN);
 
        // Announce incoming mini-game
        if (pendingLaunch >= 0)
        {
            const char* name = (pendingLaunch == SHOOTER_TILE) ? "SHOOTER" : "CASINO";
            DrawRectangle(150, 450, 500, 60, Fade(BLACK, 0.6f));
            DrawText(TextFormat("Mini-jeu : %s - lancement dans %ds...",
                     name, launchCountdown / 60 + 1),
                     160, 465, 20, YELLOW);
        }


        DrawRectangle(5, 5, 130, 50, Fade(LIGHTGRAY, 0.8f));
        DrawRectangle(8,  8,  14, 14, (Color){ 255, 180, 180, 255 });
        DrawText("Shooter",  26, 10, 12, BLACK);
        DrawRectangle(8, 26,  14, 14, (Color){ 180, 220, 255, 255 });
        DrawText("Casino",   26, 28, 12, BLACK);
 
        EndDrawing();
    }

    CloseWindow();
    return 0;
}