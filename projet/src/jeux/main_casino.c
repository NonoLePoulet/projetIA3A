#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_POINTS 100

int main_casino ()
{
    int score = 20;
    int score2 = 20;

    int max_point = score;
    int max_point2 = score2;

    Vector2 Player_pos_cas = {0,0};
    Vector2 Player_speed_cas = {0,0};

    Vector2 Player_pos_cas2 = {0,0};
    Vector2 Player_speed_cas2 = {0,0};

    Vector2 points_verts[MAX_POINTS];
    int points_count = 0;

    Vector2 points_jaunes[MAX_POINTS];
    int points_count2 = 0;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aller on gambling");
    SetTargetFPS(60);
    while (!WindowShouldClose()){


        if(IsKeyDown(KEY_LEFT)) Player_speed_cas.x = -5;
        if(IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = 5;
        if(IsKeyDown(KEY_UP)) Player_speed_cas.y = -5;
        if(IsKeyDown(KEY_DOWN)) Player_speed_cas.y = 5;
            
        if(!IsKeyDown(KEY_LEFT)&&!IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = (float) 0;
        if(!IsKeyDown(KEY_UP)&&!IsKeyDown(KEY_DOWN)) Player_speed_cas.y = (float) 0;

        Player_pos_cas.x += Player_speed_cas.x;
        Player_pos_cas.y += Player_speed_cas.y;


        if(IsKeyDown(KEY_A)) Player_speed_cas2.x = -5;
        if(IsKeyDown(KEY_D)) Player_speed_cas2.x = 5;
        if(IsKeyDown(KEY_W)) Player_speed_cas2.y = -5;
        if(IsKeyDown(KEY_S)) Player_speed_cas2.y = 5;
            
        if(!IsKeyDown(KEY_A)&&!IsKeyDown(KEY_D)) Player_speed_cas2.x = (float) 0;
        if(!IsKeyDown(KEY_W)&&!IsKeyDown(KEY_S)) Player_speed_cas2.y = (float) 0;


        

        Player_pos_cas2.x += Player_speed_cas2.x;
        Player_pos_cas2.y += Player_speed_cas2.y;



        // --- CRÉATION DES POINTS VERTS ---
        if(IsKeyPressed(KEY_ENTER)) {
            // On vérifie qu'il reste de la place dans notre tableau de points
            if (points_count < MAX_POINTS) {
                points_verts[points_count] = Player_pos_cas; // Le point apparaît sur le joueur bleu
                points_count++; // On augmente le compteur de points posés
            }
        }
        if(IsKeyPressed(KEY_SPACE)) {
            // On vérifie qu'il reste de la place dans notre tableau de points
            if (points_count2 < MAX_POINTS) {
                points_jaunes[points_count2] = Player_pos_cas2; // Le point apparaît sur le joueur rouge
                points_count2++; // On augmente le compteur de points posés
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            // On dessine TOUS les points verts enregistrés grâce à une boucle
            for (int i = 0; i < points_count; i++) {
                DrawCircleV(points_verts[i], 10, GREEN); 
            }
            for (int i = 0; i < points_count2; i++) {
                DrawCircleV(points_jaunes[i], 10, YELLOW); 
            }

            DrawCircleV(Player_pos_cas, 25, DARKBLUE);
            DrawCircleV(Player_pos_cas2, 25, RED);

        EndDrawing();


    }
    CloseWindow();
    return 0;
}