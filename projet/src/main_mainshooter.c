#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

Vector2 Player_pos = {0,0};
Vector2 Player_speed = {0,0};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monopoly - Animation Pas à Pas");
    SetTargetFPS(60);

    int playerLogicPos = 0;      // La case où le joueur va arriver
    int lastLogicPos = 0;        // La case d'où il vient (pour l'animation)
    int squaresToMove = 0;       // Nombre de cases restant à parcourir
    int lastRoll = 0;

    while (!WindowShouldClose()){
        if(IsKeyDown(KEY_LEFT)) Player_speed.x = -5;

        if(IsKeyDown(KEY_RIGHT)) Player_speed.x = 5;
        
        if(IsKeyDown(KEY_UP)) Player_speed.y = -5;

        if(IsKeyDown(KEY_DOWN)) Player_speed.y = 5;
        
        if(!IsKeyDown(KEY_LEFT)&&!IsKeyDown(KEY_RIGHT)) Player_speed.x = (float) 0;
        if(!IsKeyDown(KEY_UP)&&!IsKeyDown(KEY_DOWN)) Player_speed.y = (float) 0;


        Vector2Normalize(Player_speed);

        Player_pos.x += Player_speed.x;
        Player_pos.y += Player_speed.y;

        ClearBackground(BLACK);
        BeginDrawing();
        DrawText(TextFormat("Player_pos = %f , %f",Player_pos.x, Player_pos.y), 280, 350, 20, WHITE);
        DrawCircleV(Player_pos, 15, GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;

}