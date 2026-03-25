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
    // ==========================================
    // 1. INITIALISATION DES VARIABLES
    // ==========================================
    srand(time(NULL));
    int score = SCORE_BASE;
    int score2 = SCORE_BASE;
    int choose = 0;
    int timer = 0;


    // --- Roulette ---
    // Ordre officiel des numéros sur une roulette européenne (en tournant dans le sens des aiguilles d'une montre)
    int ordre_roulette[37] = {0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26};
    
    int numero_gagnant = -1; // -1 veut dire que la bille tourne encore
    
    // Un décalage en radians pour aligner code avec image 
    float offset_image = PI /2.0f;

    // --- Joueur 1 (Bleu/Pion1) ---
    Vector2 Player_pos_cas = {SCREEN_WIDTH/16 * 15, 650};
    Vector2 Player_speed_cas = {0,0};
    Vector2 points_verts[MAX_POINTS];
    int points_count = 0;

    // --- Joueur 2 (Rouge/Pion2) ---
    Vector2 Player_pos_cas2 = {SCREEN_WIDTH/8 * 4, 650};
    Vector2 Player_speed_cas2 = {0,0};
    Vector2 points_jaunes[MAX_POINTS];
    int points_count2 = 0;

    // --- Bille & Orbite ---
    float angle_bille = 0.0f;
    float rayon_orbite = 180.0f; 
    Vector2 centre_orbite = { 294, 414 }; 
    float vitesse_rotation = ((rand()%50)+rand()%30)/100.0f; 
    Vector2 pos_bille = { 0, 0 };

    // --- Chronomètre (Ralentissement de la bille) ---
    float chrono = 0.0f;          
    float temps_attente = 1.0f;
    float temps_choose = 15.0f;   

    // ==========================================
    // 2. INITIALISATION DE LA FENÊTRE ET TEXTURES
    // ==========================================
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aller on gambling");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("src/textures/roulette2.png");
    Texture2D cursor = LoadTexture("src/textures/link.png");
    Texture2D pion = LoadTexture("src/textures/jetons.png");
    
    Rectangle pion1 = {0.0f, 0.0f, (int)pion.width/8, (int)pion.height/4};
    Rectangle pion2 = {0.0f, (float)pion.height/4, (float)pion.width/8, (float)pion.height/4};
    Rectangle source = {0.0f, 0.0f, (float)background.width, (float)background.height};
    Rectangle dest = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};


    // ==========================================
    // 3. BOUCLE PRINCIPALE DU JEU
    // ==========================================
    while (!WindowShouldClose()){
        timer++;
        // ------------------------------------------
        // A. MISE À JOUR DE LA LOGIQUE (UPDATE)
        // ------------------------------------------

        // --- Mouvements Joueur 1 (Flèches) ---
        if(IsKeyDown(KEY_LEFT)) Player_speed_cas.x = -5;
        if(IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = 5;
        if(IsKeyDown(KEY_UP)) Player_speed_cas.y = -5;
        if(IsKeyDown(KEY_DOWN)) Player_speed_cas.y = 5;
            
        if(!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = 0.0f;
        if(!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) Player_speed_cas.y = 0.0f;

        Player_pos_cas.x += Player_speed_cas.x;
        Player_pos_cas.y += Player_speed_cas.y;

        // --- Mouvements Joueur 2 (ZQSD) ---
        if(IsKeyDown(KEY_A)) Player_speed_cas2.x = -5;
        if(IsKeyDown(KEY_D)) Player_speed_cas2.x = 5;
        if(IsKeyDown(KEY_W)) Player_speed_cas2.y = -5;
        if(IsKeyDown(KEY_S)) Player_speed_cas2.y = 5;
            
        if(!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) Player_speed_cas2.x = 0.0f;
        if(!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) Player_speed_cas2.y = 0.0f;

        Player_pos_cas2.x += Player_speed_cas2.x;
        Player_pos_cas2.y += Player_speed_cas2.y;

        // --- Limites d'écran (Bordures) ---
        if (Player_pos_cas.x < SCREEN_WIDTH/2) Player_pos_cas.x = SCREEN_WIDTH/2;
        if (Player_pos_cas.x > SCREEN_WIDTH-25) Player_pos_cas.x = SCREEN_WIDTH-25;
        if (Player_pos_cas.y < SCREEN_HEIGHT/3) Player_pos_cas.y = SCREEN_HEIGHT/3;
        if (Player_pos_cas.y > SCREEN_HEIGHT-25) Player_pos_cas.y = SCREEN_HEIGHT-25;

        if (Player_pos_cas2.x < SCREEN_WIDTH/2) Player_pos_cas2.x = SCREEN_WIDTH/2;
        if (Player_pos_cas2.x > SCREEN_WIDTH-25) Player_pos_cas2.x = SCREEN_WIDTH-25;
        if (Player_pos_cas2.y < SCREEN_HEIGHT/3) Player_pos_cas2.y = SCREEN_HEIGHT/3;
        if (Player_pos_cas2.y > SCREEN_HEIGHT-25) Player_pos_cas2.y = SCREEN_HEIGHT-25;
        
        // --- Gestion des points (Ajout) ---
        if(IsKeyPressed(KEY_ENTER) && points_count < score) {
            points_verts[points_count] = Player_pos_cas;
            points_count++;
        }
        if(IsKeyPressed(KEY_SPACE) && points_count2 < score2) {
            points_jaunes[points_count2] = Player_pos_cas2;
            points_count2++;
        }

        // --- Gestion des points (Suppression) ---
        if(IsKeyPressed(KEY_BACKSPACE) && points_count != 0) points_count--;
        if(IsKeyPressed(KEY_LEFT_CONTROL) && points_count2 != 0) points_count2--;

        // --- Physique de la bille (Orbite & Ralentissement) ---
        angle_bille += vitesse_rotation;
        pos_bille.x = centre_orbite.x + rayon_orbite * cosf(angle_bille);
        pos_bille.y = centre_orbite.y + rayon_orbite * sinf(angle_bille);

        chrono += GetFrameTime();

        if (chrono >= temps_choose && choose == 0)
        {
            choose = -1;
            chrono = 0.0f;
        }
        

        if (chrono >= temps_attente && vitesse_rotation > 0.01f && choose!=0) {
            vitesse_rotation *= 0.8f; // Réduit la vitesse de 20%
            chrono = 0.0f; 
        }

        if (vitesse_rotation < 0.0099f && vitesse_rotation > 0.0f) {
            vitesse_rotation = 0.0f; // Arrêt complet de la bille
            
            // 1. On garde seulement l'angle du dernier tour (entre 0 et 2*PI)
            float angle_final = fmodf(angle_bille + offset_image, 2.0f * PI);
            
            // 2. Si l'angle est négatif (ça peut arriver), on le remet en positif
            if (angle_final < 0) angle_final += 2.0f * PI;
            
            // 3. On calcule la taille d'une tranche (2*PI divisé par 37 cases)
            float taille_tranche = (2.0f * PI) / 37.0f;
            
            // 4. On trouve l'index de la case en divisant l'angle par la taille de la tranche
            int index_case = (int)(angle_final / taille_tranche)+1;
            
            // 5. On récupère le vrai numéro gagnant dans notre tableau !
            numero_gagnant = ordre_roulette[index_case];
            
            TraceLog(LOG_INFO, "LA BILLE EST TOMBEE SUR LE : %d", numero_gagnant);
        }
        
        // ------------------------------------------
        // B. AFFICHAGE (DRAWING)
        // ------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            // 1. Fond
            DrawTexturePro(background, source, dest, Vector2Zero(), 0.0f, WHITE);

            // 2. Jetons (Points verts et jaunes)
            for (int i = 0; i < points_count; i++) {
                DrawTextureRec(pion, pion1, (Vector2){points_verts[i].x-12, points_verts[i].y-28}, WHITE);
            }
            for (int i = 0; i < points_count2; i++) {
                DrawTextureRec(pion, pion2, (Vector2){points_jaunes[i].x-12, points_jaunes[i].y-28}, WHITE);
            }

            // 3. UI (Interface / Scores)
            DrawText(TextFormat("%d/36", score - points_count), 1080, 20, 20, RED);
            DrawText(TextFormat("%d/36", score2 - points_count2), 720, 20, 20, WHITE);

            // 4. Joueurs (Curseurs)
            DrawTexture(cursor, Player_pos_cas.x, Player_pos_cas.y, WHITE);
            DrawTexture(cursor, Player_pos_cas2.x, Player_pos_cas2.y, WHITE);
            
            // 5. La Bille
            DrawCircleV(pos_bille, 10, WHITE);
            DrawCircleV(pos_bille, 1.0f, BLUE);


            if (vitesse_rotation == 0.0f && numero_gagnant != -1) {
                DrawText(TextFormat("LE NUMERO GAGNANT EST : %d", numero_gagnant), 600, 60, 40, GREEN);
            }
            if (choose == 0)
            {
                DrawText(TextFormat("Faites vos jeux !! : %d", 15-(timer/60)), 720, 60, 40, GREEN);
            }
            else if (vitesse_rotation != 0.0f)
            {
                DrawText(TextFormat("Rien ne va plus !!!"), 760, 60, 40, GREEN);
            }
            
            
            
        EndDrawing();
    }

    // ==========================================
    // 4. NETTOYAGE ET FERMETURE
    // ==========================================
    UnloadTexture(pion);   // N'oublie pas de décharger toutes tes textures !
    UnloadTexture(cursor);
    UnloadTexture(background);
    
    CloseWindow();
    return 0;
}