#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH_SHOOTER 800
#define SCREEN_HEIGHT_SHOOTER 800
#define BULLET_MAX 50000
#define NB_PLAYER 2

typedef struct __player{
    Vector2 speed;
    Vector2 pos;
    int is_alive;
    int explode_timer;
    int frameCount;
    int cur_frame;
    Rectangle frameRec;
} Player;

typedef struct _bullet{
    Vector2 direction;
    Vector2 pos;
    int timer;
    bool display;
} Bullet;




void fire_bullet(Bullet* list, int index, int time){
    list[index].display = 1;
    list[index].pos.x = 400 + 150*sinf(time/60.0f + PI);
    list[index].pos.y = 150;
    list[index].direction.x = (rand()%800) - 400;
    list[index].direction.y = (rand()%700);
    list[index].direction = Vector2Normalize(list[index].direction);
    list[index].direction.x *=6;
    list[index].direction.y *=6;
    list[index].timer = time;
}


int shooter(int mode) {
    InitWindow(SCREEN_WIDTH_SHOOTER, SCREEN_HEIGHT_SHOOTER, "Shooter");
    Bullet *bullet_list = (Bullet *)RL_CALLOC(BULLET_MAX, sizeof(Bullet));
    Player *player_list = (Player *)RL_CALLOC(NB_PLAYER, sizeof(Player));
    player_list[0].pos=(Vector2){300,700};
    player_list[1].pos=(Vector2){500,700};
    srand(time(NULL));

    Texture2D SpaceShip = LoadTexture("src/textures/shooter_spaceship1.png");
    Texture2D bullet = LoadTexture("src/textures/laser_bullet.png");
    Texture2D enemy = LoadTexture("src/textures/Shooter_enemy.png");
    Rectangle enemy_frame_rec = (Rectangle){0.0f,0.0f,enemy.width/8,enemy.height/5};
    Rectangle bullet_rect = (Rectangle){0.0f,0.0f,(float)bullet.width,(float)bullet.height};
    for(int i=0;i<NB_PLAYER;++i){
        player_list[i].frameRec = (Rectangle){ 0.0f, 0.0f, (float)SpaceShip.width/12, (float)SpaceShip.height/5 };
        player_list[i].is_alive = 1;
        player_list[i].explode_timer = 0;
    }
    int time = 0;
    int framesSpeed = 12;
    float delay = 90.0f;
    float bullet_timer = 0;
    int nb_bullet = 0;
    float textTime = 360;
    Vector2 TargetPoint = (Vector2){rand()%800,rand()%350+450};
    Vector2 Trigger;
    int shouldDodge = 0;
    Bullet bulletToDodge;
    int winner;
    float endTimer = 0.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()){

        time++;
        bullet_timer++;
        if (delay<0.5f) delay = 0.5f;
        if (player_list[1].speed.x > 3) player_list[1].speed.x = 3;
        if (player_list[1].speed.x < -3) player_list[1].speed.x = -3;
        if (player_list[1].speed.y > 3) player_list[1].speed.y = 3;
        if (player_list[1].speed.y < -3) player_list[1].speed.y = -3;

        if(bullet_timer > delay){
            bullet_timer = 0;
            fire_bullet(bullet_list,nb_bullet,time);
            nb_bullet++;
            delay -= 18.0f/(nb_bullet);
        }
        for(int i=0;i<NB_PLAYER;++i){
            if(player_list[i].explode_timer > 55){
                player_list[i].is_alive = 0;
                player_list[i].pos = Vector2Zero();
            }
            player_list[i].frameCount++;
            if(player_list[i].is_alive == 1){
                if (player_list[i].frameCount >= (60/framesSpeed)){
                    player_list[i].frameCount = 0;
                    player_list[i].cur_frame++;
                    if (player_list[i].cur_frame > 3) player_list[i].cur_frame = 0;
                    player_list[i].frameRec.x = (float)player_list[i].cur_frame*(float)SpaceShip.width/12;
                }
            }
            else if(player_list[i].is_alive == 2){
                player_list[i].explode_timer++;
                if (player_list[i].frameCount >= (60/framesSpeed)){
                    player_list[i].frameCount = 0;
                    player_list[i].cur_frame++;
                    player_list[i].frameRec.x = (float)player_list[i].cur_frame*(float)SpaceShip.width/12;
                }
            }
            else{
                player_list[i].pos = Vector2Zero();
            }
        }

//------------------------------------Input player 1-----------------------------------------------
        if(player_list[0].is_alive == 1){
            if(IsKeyDown(KEY_LEFT)) player_list[0].speed.x = -3;
            if(IsKeyDown(KEY_RIGHT)) player_list[0].speed.x = 3;    
            if(IsKeyDown(KEY_UP)) player_list[0].speed.y = -3;
            if(IsKeyDown(KEY_DOWN)) player_list[0].speed.y = 3;
        }  
        if(!IsKeyDown(KEY_LEFT)&&!IsKeyDown(KEY_RIGHT) || player_list[0].is_alive != 1) player_list[0].speed.x = (float) 0;
        if(!IsKeyDown(KEY_UP)&&!IsKeyDown(KEY_DOWN) || player_list[0].is_alive != 1) player_list[0].speed.y = (float) 0;
//-------------------------------------------------------------------------------------------------
//------------------------------------Input player 2-----------------------------------------------
        if(mode == 0){
            if(player_list[1].is_alive == 1){
                if(IsKeyDown(KEY_A)) player_list[1].speed.x = -3;
                if(IsKeyDown(KEY_D)) player_list[1].speed.x = 3;    
                if(IsKeyDown(KEY_W)) player_list[1].speed.y = -3;
                if(IsKeyDown(KEY_S)) player_list[1].speed.y = 3;
            }  
            if(!IsKeyDown(KEY_A)&&!IsKeyDown(KEY_D) || player_list[1].is_alive != 1) player_list[1].speed.x = (float) 0;
            if(!IsKeyDown(KEY_W)&&!IsKeyDown(KEY_S) || player_list[1].is_alive != 1) player_list[1].speed.y = (float) 0;
        }
//-------------------------------------------------------------------------------------------------
//--------------------------------------AI Moveset-------------------------------------------------

        if(mode == 1){
            Trigger = (Vector2){player_list[1].pos.x,player_list[1].pos.y-70};
            if(shouldDodge == 0 && player_list[1].is_alive == 1){
                if(abs(player_list[1].pos.x-TargetPoint.x)<16 && abs(player_list[1].pos.y-TargetPoint.y)<16){
                    TargetPoint = (Vector2){rand()%800,rand()%350+450};
                }
                if(TargetPoint.x-player_list[1].pos.x > 0){
                    player_list[1].speed.x = 2;
                }
                else if(TargetPoint.x-player_list[1].pos.x < 0){
                    player_list[1].speed.x = -2;
                }
                if(TargetPoint.y-player_list[1].pos.y > 0){
                    player_list[1].speed.y = 2;
                }
                else if(TargetPoint.y-player_list[1].pos.y < 0){
                    player_list[1].speed.y = -2;
                }
                if(abs(player_list[1].pos.x-TargetPoint.x)<10){
                    player_list[1].pos.x = TargetPoint.x;
                    player_list[1].speed.x = 0;
                }
                if(abs(player_list[1].pos.y-TargetPoint.y)<10){
                    player_list[1].pos.y = TargetPoint.y;
                    player_list[1].speed.y = 0;
                }
            }
            else if(shouldDodge == 1){
                if(abs(bulletToDodge.direction.x)<1){
                    player_list[1].speed.x = 3;
                }
                else{
                    player_list[1].speed.x = -(bulletToDodge.direction.x/abs(bulletToDodge.direction.x))*3;
                    player_list[1].speed.y = 3;
                }
                if(!(abs(Trigger.x-bulletToDodge.pos.x)<70 && abs(Trigger.y-bulletToDodge.pos.y)<30)){
                    shouldDodge = 0;
                }
            }
            else{
                player_list[1].speed = Vector2Zero();
            }
        }

//-------------------------------------------------------------------------------------------------

        for(int i=0;i<NB_PLAYER;++i){
            Vector2Normalize(player_list[i].speed);

            player_list[i].pos.x += player_list[i].speed.x;
            player_list[i].pos.y += player_list[i].speed.y;

            if(player_list[i].pos.x < 0) player_list[i].pos.x = 0;
            if(player_list[i].pos.x > SCREEN_WIDTH_SHOOTER) player_list[i].pos.x = SCREEN_WIDTH_SHOOTER;
            if(player_list[i].pos.y < 250) player_list[i].pos.y = 250;
            if(player_list[i].pos.y > SCREEN_HEIGHT_SHOOTER) player_list[i].pos.y = SCREEN_HEIGHT_SHOOTER;
            for(int j=0;j<nb_bullet;++j){
                if (abs(player_list[i].pos.x-bullet_list[j].pos.x)<16 && abs(player_list[i].pos.y-bullet_list[j].pos.y)<16){
                    bullet_list[j].display = 0;
                    player_list[i].is_alive = 2;
                    player_list[i].frameRec.y = 3*(float)SpaceShip.height/5;
                    player_list[i].cur_frame = 0;

                }
                if(abs(Trigger.x-bullet_list[j].pos.x)<70 && abs(Trigger.y-bullet_list[j].pos.y)<30){
                    shouldDodge = 1;
                    bulletToDodge = bullet_list[j];
                    TargetPoint = (Vector2){rand()%800,rand()%350+450};
                }
            }
        }
        for (int j=0;j<nb_bullet;++j){
            bullet_list[j].pos.x += bullet_list[j].direction.x;
            bullet_list[j].pos.y += bullet_list[j].direction.y;
            if(time - bullet_list[j].timer > 300) bullet_list[j].display = 0;
        }

        if(player_list[0].is_alive == 0 && endTimer == 0.0f){
            winner = 1;
            endTimer = time;
        }
        if(player_list[1].is_alive == 0 && endTimer == 0.0f){
            winner = 0;
            endTimer = time;
        }
        if(time-endTimer > 180.0f && endTimer != 0.0f) return winner;

        ClearBackground(BLACK);
        BeginDrawing();

        DrawText(TextFormat("Time : %d s", time/60), 10, 10, 20, WHITE);
        if(player_list[0].is_alive != 0){
            DrawTextureRec(SpaceShip, player_list[0].frameRec,(Vector2){player_list[0].pos.x-16,player_list[0].pos.y-16}, WHITE);
            DrawText("Player1",player_list[0].pos.x-18,player_list[0].pos.y+20, 7, RED);
        }
        if(player_list[1].is_alive != 0){
            DrawTextureRec(SpaceShip, player_list[1].frameRec,(Vector2){player_list[1].pos.x-16,player_list[1].pos.y-16}, WHITE);
            DrawText("IA",player_list[1].pos.x-7,player_list[1].pos.y+20, 7, BLUE);
        }
        if(endTimer > 0.0f){
            DrawText(TextFormat("PLAYER %d WINS",winner + 1),200,400,50,WHITE);
        }

        for (int k=0;k<nb_bullet;++k){
            if (bullet_list[k].display == 1) DrawTexturePro(bullet,bullet_rect,(Rectangle){bullet_list[k].pos.x,bullet_list[k].pos.y,15.0f,15.0f},Vector2Zero(),0.0f,WHITE);
        }
        DrawTexturePro(enemy,enemy_frame_rec,(Rectangle){200,0,400,200},(Vector2){150*sinf(time/60.0f),0},0,WHITE);
        

        EndDrawing();
    }
    UnloadTexture(SpaceShip);
    UnloadTexture(bullet);
    CloseWindow();

    return 0 ;

}