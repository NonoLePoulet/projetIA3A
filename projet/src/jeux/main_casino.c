#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "time.h"

#define SCREEN_WIDTH 1242
#define SCREEN_HEIGHT 708
#define MAX_POINTS 36
#define SCORE_BASE 10

int main_casino ()
{
    srand(time(NULL));
    int score = SCORE_BASE;
    int score2 = SCORE_BASE;

    int max_point = score;
    int max_point2 = score2;

    Vector2 Player_pos_cas = {SCREEN_WIDTH/16 * 15,650};
    Vector2 Player_speed_cas = {0,0};

    Vector2 Player_pos_cas2 = {SCREEN_WIDTH/8 * 4,650};
    Vector2 Player_speed_cas2 = {0,0};

    Vector2 points_verts[MAX_POINTS];
    int points_count = 0;

    Vector2 points_jaunes[MAX_POINTS];
    int points_count2 = 0;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aller on gambling");
    Texture2D background = LoadTexture("src/textures/roulette2.png");
    Texture2D cursor = LoadTexture("src/textures/link.png");
    Texture2D pion = LoadTexture("src/textures/jetons.png");
    Rectangle pion1= {0.0f, 0.0f, (int)pion.width/8, (int)pion.height/4};
    Rectangle pion2= {0.0f,(float)pion.height/4, (float)pion.width/8, (float)pion.height/4};

    Rectangle source = {0.0f,0.0f,(float)background.width,(float)background.height};
    Rectangle dest = {0.0f,0.0f,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT};
    SetTargetFPS(60);


    float angle_bille = 0.0f;
    float rayon_orbite = 180.0f; // La taille du cercle autour duquel le point tourne
    Vector2 centre_orbite = { 294, 414 }; // Le centre de l'écran
    float vitesse_rotation = ((rand()%30)+20)/100.0f; // Plus le chiffre est grand, plus ça tourne vite
    Vector2 pos_bille = { 0, 0 };


    float chrono = 0.0f;          // Notre chronomètre qui commence à 0
    float temps_attente = 1.0f;   // Le nombre de secondes à attendre

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

        // --- EVITER LES BORDURES
        if (Player_pos_cas.x < 25)
        {
            Player_pos_cas.x = 25;
        }

        if (Player_pos_cas.x > SCREEN_WIDTH-25)
        {
            Player_pos_cas.x = SCREEN_WIDTH-25;
        }
        if (Player_pos_cas.y < 25)
        {
            Player_pos_cas.y = 25;
        }

        if (Player_pos_cas.y > SCREEN_WIDTH-25)
        {
            Player_pos_cas.y = SCREEN_WIDTH-25;
        }

        if (Player_pos_cas2.x < 25)
        {
            Player_pos_cas2.x = 25;
        }

        if (Player_pos_cas2.x > SCREEN_WIDTH-25)
        {
            Player_pos_cas2.x = SCREEN_WIDTH-25;
        }
        if (Player_pos_cas2.y < 25)
        {
            Player_pos_cas2.y = 25;
        }

        if (Player_pos_cas2.y > SCREEN_WIDTH-25)
        {
            Player_pos_cas2.y = SCREEN_WIDTH-25;
        }
        
        
        // --- CRÉATION DES POINTS ---
        if(IsKeyPressed(KEY_ENTER)) {
            // On vérifie qu'il reste de la place dans notre tableau de points
            if (points_count < score) {
                points_verts[points_count] = Player_pos_cas; // Le point apparaît sur le joueur bleu
                points_count++; // On augmente le compteur de points posés
            }
        }
        if(IsKeyPressed(KEY_SPACE)) {
            // On vérifie qu'il reste de la place dans notre tableau de points
            if (points_count2 < score2) {
                points_jaunes[points_count2] = Player_pos_cas2; // Le point apparaît sur le joueur rouge
                points_count2++; // On augmente le compteur de points posés
            }
        }


        // --- SUPPRESION DES POINTS ---
        if(IsKeyPressed(KEY_BACKSPACE) && points_count != 0)
        {
            points_count--;
        }

        if(IsKeyPressed(KEY_LEFT_CONTROL) && points_count2 != 0)
        {
            points_count2--;
        }


        // --- CALCUL DE L'ORBITE ---
        // 1. On augmente l'angle pour faire avancer le point
        angle_bille += vitesse_rotation;

        // 2. On applique la formule (cosinus pour X, sinus pour Y)
        pos_bille.x = centre_orbite.x + rayon_orbite * cosf(angle_bille);
        pos_bille.y = centre_orbite.y + rayon_orbite * sinf(angle_bille);


        chrono += GetFrameTime();

        if (chrono >= temps_attente && vitesse_rotation>0.01) {
            
            vitesse_rotation = vitesse_rotation*0.8;


            chrono = 0.0f; 
        }


        if (vitesse_rotation < 0.0099)
        {
            vitesse_rotation = 0;
        }
        
        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(background,source,dest,Vector2Zero(),0.0f,WHITE);

            // On dessine TOUS les points enregistrés grâce à une boucle
            for (int i = 0; i < points_count; i++) {
                DrawTextureRec(pion, pion1,(Vector2){points_verts[i].x-12,points_verts[i].y-28}, WHITE) ;
            }
            for (int i = 0; i < points_count2; i++) {
                DrawTextureRec(pion, pion2,(Vector2){points_jaunes[i].x-12,points_jaunes[i].y-28}, WHITE);
            }

            DrawText(TextFormat("%d/36", score-points_count), 1080, 20, 20, RED);
            DrawText(TextFormat("%d/36", score2-points_count2), 720, 20, 20, WHITE);

            DrawTexture(cursor,Player_pos_cas.x,Player_pos_cas.y,WHITE);
            DrawTexture(cursor,Player_pos_cas2.x,Player_pos_cas2.y,WHITE);

            DrawCircleV(pos_bille, 10, WHITE);

        EndDrawing();


    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}