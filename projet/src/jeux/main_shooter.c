#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BULLET_MAX 50000
#define NB_PLAYER 3

typedef struct _player{
    Vector2 speed;
    Vector2 pos;
    bool is_alive;
} Player;

typedef struct _bullet{
    Vector2 direction;
    Vector2 pos;
    int timer;
    bool display;
} Bullet;


void fire_bullet(Bullet* list, int index, int time){
    list[index].display = 1;
    list[index].pos.x = 400;
    list[index].pos.y = 50;
    list[index].direction.x = (rand()%800) - 400;
    list[index].direction.y = (rand()%700);
    list[index].direction = Vector2Normalize(list[index].direction);
    list[index].direction.x *=3;
    list[index].direction.y *=3;
    list[index].timer = time;
}


void shooter() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monopoly - Animation Pas à Pas");
    Bullet *bullet_list = (Bullet *)RL_CALLOC(BULLET_MAX, sizeof(Bullet));
    Player *player_list = (Player *)RL_CALLOC(NB_PLAYER, sizeof(Player));
    player_list[0].pos=(Vector2){300,700};
    player_list[1].pos=(Vector2){500,700};
    srand(time(NULL));

    Texture2D SpaceShip = LoadTexture("src/textures/shooter_spaceship.png");
    Rectangle frameRec = { 0.0f, 0.0f, (float)SpaceShip.width/4, (float)SpaceShip.height };
    int framesCounter = 0;
    int time = 0;
    int framesSpeed = 8;
    int cur_frame = 0;
    float delay = 90.0f;
    float bullet_timer = 0;
    int nb_bullet = 0;
    float textTime = 360;

    SetTargetFPS(60);

    fire_bullet(bullet_list,0,time);
    bullet_list[0].pos = (Vector2){400,50};

    while (!WindowShouldClose()){

        framesCounter++;
        time++;
        bullet_timer++;
        if (delay<1.0f) delay = 1.0f;

        if(bullet_timer > delay){
            bullet_timer = 0;
            fire_bullet(bullet_list,nb_bullet,time);
            nb_bullet++;
            delay -= 18.0f/(nb_bullet);
        }
        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            cur_frame++;
            if (cur_frame > 3) cur_frame = 0;
            frameRec.x = (float)cur_frame*(float)SpaceShip.width/4;
        }

//------------------------------------Input player 1-----------------------------------------------
        if(IsKeyDown(KEY_A)) player_list[0].speed.x = -5;
        if(IsKeyDown(KEY_D)) player_list[0].speed.x = 5;    
        if(IsKeyDown(KEY_W)) player_list[0].speed.y = -5;
        if(IsKeyDown(KEY_S)) player_list[0].speed.y = 5;
            
        if(!IsKeyDown(KEY_A)&&!IsKeyDown(KEY_D)) player_list[0].speed.x = (float) 0;
        if(!IsKeyDown(KEY_W)&&!IsKeyDown(KEY_S)) player_list[0].speed.y = (float) 0;
//-------------------------------------------------------------------------------------------------
//------------------------------------Input player 2-----------------------------------------------
        if(IsKeyDown(KEY_LEFT)) player_list[1].speed.x = -5;
        if(IsKeyDown(KEY_RIGHT)) player_list[1].speed.x = 5;    
        if(IsKeyDown(KEY_UP)) player_list[1].speed.y = -5;
        if(IsKeyDown(KEY_DOWN)) player_list[1].speed.y = 5;
            
        if(!IsKeyDown(KEY_LEFT)&&!IsKeyDown(KEY_RIGHT)) player_list[1].speed.x = (float) 0;
        if(!IsKeyDown(KEY_UP)&&!IsKeyDown(KEY_DOWN)) player_list[1].speed.y = (float) 0;
//-------------------------------------------------------------------------------------------------


        for(int i=0;i<2;++i){
            Vector2Normalize(player_list[i].speed);

            player_list[i].pos.x += player_list[i].speed.x;
            player_list[i].pos.y += player_list[i].speed.y;

            if(player_list[i].pos.x < 0) player_list[i].pos.x = 0;
            if(player_list[i].pos.x > SCREEN_WIDTH) player_list[i].pos.x = SCREEN_WIDTH;
            if(player_list[i].pos.y < 0) player_list[i].pos.y = 0;
            if(player_list[i].pos.y > SCREEN_HEIGHT) player_list[i].pos.y = SCREEN_HEIGHT;
        }
        for (int j=0;j<nb_bullet;++j){
            bullet_list[j].pos.x += bullet_list[j].direction.x;
            bullet_list[j].pos.y += bullet_list[j].direction.y;
            if(time - bullet_list[j].timer > 300) bullet_list[j].display = 0;
        }
        ClearBackground(BLACK);
        BeginDrawing();

        DrawText(TextFormat("Time : %d s", time/60), 10, 10, 20, WHITE);
        DrawTextureRec(SpaceShip, frameRec,(Vector2){player_list[0].pos.x-16,player_list[0].pos.y-16}, WHITE);
        DrawText("Player1",player_list[0].pos.x-18,player_list[0].pos.y+20, 7, RED);
        DrawTextureRec(SpaceShip, frameRec,(Vector2){player_list[1].pos.x-16,player_list[1].pos.y-16}, WHITE);
        DrawText("Player2",player_list[1].pos.x-18,player_list[1].pos.y+20, 7, BLUE);
        for (int k=0;k<nb_bullet;++k){
            if (bullet_list[k].display == 1) DrawCircle(bullet_list[k].pos.x,bullet_list[k].pos.y,8,RED);
        }
        DrawRectangle(200,0,400,150,WHITE);
        DrawText(TextFormat("%d", nb_bullet), 400, 300, 20, WHITE);

        EndDrawing();
    }
    UnloadTexture(SpaceShip);
    CloseWindow();

}