#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH_TRON 1400
#define SCREEN_HEIGHT_TRON 800
#define GRID_WIDTH 140
#define GRID_HEIGHT 80
#define NB_PLAYER 3

typedef struct _player{
    Vector2 pos;
    Vector2 speed;
    int is_alive;
    int dirrection;
} Players;

typedef struct _tile{
    Vector2 pos;
    int is_taken;
    Color color;
} tile;

void main_tron(){
    InitWindow(SCREEN_WIDTH_TRON, SCREEN_HEIGHT_TRON, "Monopoly - Animation Pas à Pas");
    Players* players_list = (Players*)RL_CALLOC(3,sizeof(Players));
    tile board[GRID_WIDTH][GRID_HEIGHT];
    for(int i=0;i<GRID_WIDTH;++i){
        for(int j=0;j<GRID_HEIGHT;++j){
            board[i][j].color = BLACK;
        }
    }
    players_list[0].is_alive = 1;
    players_list[0].dirrection = 1;
    players_list[0].speed = Vector2Zero();
    while (!WindowShouldClose()){

        if(players_list[0].is_alive == 1){
            if(IsKeyDown(KEY_A) && players_list[0].dirrection){
                players_list[0].speed.x = -1;
                players_list[0].speed.y = 0;
                players_list[0].dirrection = !players_list[0].dirrection;
            }
            if(IsKeyDown(KEY_D) && players_list[0].dirrection){
                players_list[0].speed.x = 1;
                players_list[0].speed.y = 0;
                players_list[0].dirrection = !players_list[0].dirrection;
            }   
            if(IsKeyDown(KEY_W) && !players_list[0].dirrection){
                players_list[0].speed.y = -1;
                players_list[0].speed.x = 0;
                players_list[0].dirrection = !players_list[0].dirrection;
            }
            if(IsKeyDown(KEY_S) && !players_list[0].dirrection){
                players_list[0].speed.y = 1;
                players_list[0].speed.x = 0;
                players_list[0].dirrection = !players_list[0].dirrection;
            }
        }
        board[(int)players_list[0].pos.x*GRID_WIDTH/SCREEN_WIDTH_TRON][(int)players_list[0].pos.y*GRID_HEIGHT/SCREEN_HEIGHT_TRON].color = RED;

        for(int i=0;i<NB_PLAYER;++i){
            if (players_list[i].speed.x > 1) players_list[i].speed.x = 1;
            if (players_list[i].speed.y > 1) players_list[i].speed.y = 1;
            players_list[i].pos.x += players_list[i].speed.x;
            players_list[i].pos.y += players_list[i].speed.y;
        }

        ClearBackground(BLACK);
        BeginDrawing();
        DrawCircle(players_list[0].pos.x,players_list[0].pos.y,15.0f,WHITE);
        for(int i=0;i<GRID_WIDTH;++i){
            for(int j=0;j<GRID_HEIGHT;++j){
                DrawRectangle(i*(SCREEN_WIDTH_TRON/GRID_WIDTH),j*(SCREEN_HEIGHT_TRON/GRID_HEIGHT),SCREEN_WIDTH_TRON/GRID_WIDTH,SCREEN_HEIGHT_TRON/GRID_HEIGHT,board[i][j].color);
            }
        }
        DrawText(TextFormat("%f                 %f",players_list[0].speed.x,players_list[0].speed.y),400,400,15,WHITE);
        EndDrawing();
    }
    CloseWindow();
}