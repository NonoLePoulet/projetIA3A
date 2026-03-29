#include "raylib.h"
#include "raymath.h"
#include "./jeux/main_board.c"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


void mode_select(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mode select");
    SetTargetFPS(60);
    int mode  = 0 ; 
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_LEFT)){
            mode = 0;
        }
        else if (IsKeyPressed(KEY_RIGHT)){
            mode = 1 ;
        }

        if(IsKeyPressed(KEY_ENTER)){
            CloseWindow();
            main_board(mode);
        }

        // DRAWING
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Choisissez votre mode de jeu :" , 150 ,200 , 35, BLACK);
        DrawText("PVIA", 200, 400, 30, RED);
        DrawText("PVP",600,400,30,BLUE);
        if(mode == 0){
            DrawCircle(180,410,15,GREEN);
        }
        else if (mode == 1){
            DrawCircle(580,410,15,GREEN);
        }
        EndDrawing();
    }
    CloseWindow();
}