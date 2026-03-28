#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"

#define SCREEN_WIDTH 1242
#define SCREEN_HEIGHT 708
#define MAX_POINTS 10
#define SCORE_BASE 10

// ==========================================
// STRUCTURES ET DONNÉES DE LA ROULETTE
// ==========================================
typedef struct {
    int numero;
    int couleur;  // 0 = Vert, 1 = Rouge, 2 = Noir
    int parite;   // 0 = Zéro, 1 = Impair, 2 = Pair
    int moitie;   // 0 = Zéro, 1 = Manque (1-18), 2 = Passe (19-36)
    int douzaine; // 0 = Zéro, 1 = 1ère (1-12), 2 = 2ème (13-24), 3 = 3ème (25-36)
    int colonne;  // 0 = Zéro, 1 = 1ère col, 2 = 2ème col, 3 = 3ème col
} CaseRoulette;

int main_casino ()
{
    int i;
    int j;
    int k = 0;
    int l = 0;
    int fin_score = 0;
    int fin_score2 = 0; 
    int win1 = 0;
    int win2 = 0; 
    int roulette_terminee = 0;
    int manches_jouees = 0;
    int vainqueur = -1; // -1 = partie en cours, 0 = égalité, 1 = J1, 2 = J2
    float timer_fin_manche = 0.0f;
    
    // ==========================================
    // 1. INITIALISATION DES VARIABLES
    // ==========================================
    srand(time(NULL));
    int score = SCORE_BASE;
    int score2 = SCORE_BASE;
    int choose = 0;
    int timer = 0;

    CaseRoulette table_roulette[37];
    table_roulette[0]=(CaseRoulette){0,0,0,0,0,0}; table_roulette[1]=(CaseRoulette){1,1,1,1,1,1}; table_roulette[2]=(CaseRoulette){2,2,2,1,1,2}; table_roulette[3]=(CaseRoulette){3,1,1,1,1,3}; table_roulette[4]=(CaseRoulette){4,2,2,1,1,1}; table_roulette[5]=(CaseRoulette){5,1,1,1,1,2}; table_roulette[6]=(CaseRoulette){6,2,2,1,1,3}; table_roulette[7]=(CaseRoulette){7,1,1,1,1,1}; table_roulette[8]=(CaseRoulette){8,2,2,1,1,2}; table_roulette[9]=(CaseRoulette){9,1,1,1,1,3}; table_roulette[10]=(CaseRoulette){10,2,2,1,1,1}; table_roulette[11]=(CaseRoulette){11,2,1,1,1,2}; table_roulette[12]=(CaseRoulette){12,1,2,1,1,3}; table_roulette[13]=(CaseRoulette){13,2,1,1,2,1}; table_roulette[14]=(CaseRoulette){14,1,2,1,2,2}; table_roulette[15]=(CaseRoulette){15,2,1,1,2,3}; table_roulette[16]=(CaseRoulette){16,1,2,1,2,1}; table_roulette[17]=(CaseRoulette){17,2,1,1,2,2}; table_roulette[18]=(CaseRoulette){18,1,2,1,2,3}; table_roulette[19]=(CaseRoulette){19,1,1,2,2,1}; table_roulette[20]=(CaseRoulette){20,2,2,2,2,2}; table_roulette[21]=(CaseRoulette){21,1,1,2,2,3}; table_roulette[22]=(CaseRoulette){22,2,2,2,2,1}; table_roulette[23]=(CaseRoulette){23,1,1,2,2,2}; table_roulette[24]=(CaseRoulette){24,2,2,2,2,3}; table_roulette[25]=(CaseRoulette){25,1,1,2,3,1}; table_roulette[26]=(CaseRoulette){26,2,2,2,3,2}; table_roulette[27]=(CaseRoulette){27,1,1,2,3,3}; table_roulette[28]=(CaseRoulette){28,2,2,2,3,1}; table_roulette[29]=(CaseRoulette){29,2,1,2,3,2}; table_roulette[30]=(CaseRoulette){30,1,2,2,3,3}; table_roulette[31]=(CaseRoulette){31,2,1,2,3,1}; table_roulette[32]=(CaseRoulette){32,1,2,2,3,2}; table_roulette[33]=(CaseRoulette){33,2,1,2,3,3}; table_roulette[34]=(CaseRoulette){34,1,2,2,3,1}; table_roulette[35]=(CaseRoulette){35,2,1,2,3,2}; table_roulette[36]=(CaseRoulette){36,1,2,2,3,3}; 

    int ordre_roulette[37] = {0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26};
    
    int numero_gagnant = -1; 
    float offset_image = PI / 2.0f;

    // --- Matrice et Hitboxes du tapis ---
    int grille_nombres[3][12] = {
        { 3,  6,  9, 12, 15, 18, 21, 24, 27, 30, 33, 36 }, 
        { 2,  5,  8, 11, 14, 17, 20, 23, 26, 29, 32, 35 }, 
        { 1,  4,  7, 10, 13, 16, 19, 22, 25, 28, 31, 34 }  
    };

    int grille_bizarre[2][6] = {
        { 1, 1, 2, 2, 3, 3 }, 
        { 1, 2, 1, 2, 1, 2 }
    };

    int grille_dernier[3][1] = {
        {3},
        {2},
        {1}
    }; 

    Rectangle zero = {323*2.0f, 123*2.0f, (346-323)*2.0f, (230-123)*2.0f};
    Rectangle nombres = {347*2.0f, 123*2.0f, (565-347)*2.0f, (230-123)*2.0f};
    Rectangle bizarre = {347*2.0f, 230*2.0f, (565-347)*2.0f, (301-230)*2.0f};
    Rectangle dernier = {565*2.0f, 123*2.0f, (583-565)*2.0f, (230-123)*2.0f};

    Vector2 Player_pos_cas = {SCREEN_WIDTH/16 * 15, 650};
    Vector2 Player_speed_cas = {0,0};
    Vector2 points_verts[MAX_POINTS];
    Vector2 win_verts[MAX_POINTS];
    int points_count = 0;

    Vector2 Player_pos_cas2 = {SCREEN_WIDTH/8 * 4, 650};
    Vector2 Player_speed_cas2 = {0,0};
    Vector2 points_jaunes[MAX_POINTS];
    Vector2 win_jaunes[MAX_POINTS];
    int points_count2 = 0;

    float angle_bille = 0.0f;
    float rayon_orbite = 180.0f; 
    Vector2 centre_orbite = { 294, 414 }; 
    float vitesse_rotation = ((rand()%50)+rand()%30)/100.0f; 
    Vector2 pos_bille = { 0, 0 };
    
    float chrono = 0.0f;          
    float temps_attente = 1.0f;
    float temps_choose = 15.0f; 

    // ==========================================
    // 2. INITIALISATION DE LA FENÊTRE
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
    // 3. BOUCLE PRINCIPALE
    // ==========================================
    while (!WindowShouldClose()){
        timer++;

        // --- Mouvements Joueur 1 & 2 ---
        if (choose == 0) {
            if(IsKeyDown(KEY_LEFT)) Player_speed_cas.x = -5;
            if(IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = 5;
            if(IsKeyDown(KEY_UP)) Player_speed_cas.y = -5;
            if(IsKeyDown(KEY_DOWN)) Player_speed_cas.y = 5;
            if(!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) Player_speed_cas.x = 0.0f;
            if(!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) Player_speed_cas.y = 0.0f;
            Player_pos_cas.x += Player_speed_cas.x;
            Player_pos_cas.y += Player_speed_cas.y;

            if(IsKeyDown(KEY_A)) Player_speed_cas2.x = -5;
            if(IsKeyDown(KEY_D)) Player_speed_cas2.x = 5;
            if(IsKeyDown(KEY_W)) Player_speed_cas2.y = -5;
            if(IsKeyDown(KEY_S)) Player_speed_cas2.y = 5;
            if(!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) Player_speed_cas2.x = 0.0f;
            if(!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) Player_speed_cas2.y = 0.0f;
            Player_pos_cas2.x += Player_speed_cas2.x;
            Player_pos_cas2.y += Player_speed_cas2.y;
        }

        // --- Limites d'écran ---
        if (Player_pos_cas.x < SCREEN_WIDTH/2) Player_pos_cas.x = SCREEN_WIDTH/2;
        if (Player_pos_cas.x > SCREEN_WIDTH-25) Player_pos_cas.x = SCREEN_WIDTH-25;
        if (Player_pos_cas.y < SCREEN_HEIGHT/3) Player_pos_cas.y = SCREEN_HEIGHT/3;
        if (Player_pos_cas.y > SCREEN_HEIGHT-25) Player_pos_cas.y = SCREEN_HEIGHT-25;

        if (Player_pos_cas2.x < SCREEN_WIDTH/2) Player_pos_cas2.x = SCREEN_WIDTH/2;
        if (Player_pos_cas2.x > SCREEN_WIDTH-25) Player_pos_cas2.x = SCREEN_WIDTH-25;
        if (Player_pos_cas2.y < SCREEN_HEIGHT/3) Player_pos_cas2.y = SCREEN_HEIGHT/3;
        if (Player_pos_cas2.y > SCREEN_HEIGHT-25) Player_pos_cas2.y = SCREEN_HEIGHT-25;
        
        if(choose!=0) {
            Player_pos_cas.x = SCREEN_WIDTH/16 * 15;
            Player_pos_cas.y = 650;
            Player_pos_cas2.x = SCREEN_WIDTH/8 * 4;
            Player_pos_cas2.y = 650;
        }

        // --- Ajout/Suppression de points ---
        if (choose == 0) {
            if(IsKeyPressed(KEY_ENTER) && points_count < score) {
                points_verts[points_count] = Player_pos_cas;
                points_count++;
            }
            if(IsKeyPressed(KEY_SPACE) && points_count2 < score2) {
                points_jaunes[points_count2] = Player_pos_cas2;
                points_count2++;
            }
            if(IsKeyPressed(KEY_BACKSPACE) && points_count != 0) points_count--;
            if(IsKeyPressed(KEY_LEFT_CONTROL) && points_count2 != 0) points_count2--;
        }

        // --- Physique de la bille ---
        angle_bille += vitesse_rotation;
        pos_bille.x = centre_orbite.x + rayon_orbite * cosf(angle_bille);
        pos_bille.y = centre_orbite.y + rayon_orbite * sinf(angle_bille);

        chrono += GetFrameTime();

        if (chrono >= temps_choose && choose == 0) {
            choose = -1;
            chrono = 0.0f;
            fin_score = -1;  
            fin_score2 = -1;
        }

        if (chrono >= temps_attente && vitesse_rotation > 0.01f && choose != 0) {
            vitesse_rotation *= 0.8f; 
            chrono = 0.0f; 
        }

        if (vitesse_rotation < 0.0099f && vitesse_rotation > 0.0f) {
            vitesse_rotation = 0.0f; 
            roulette_terminee = 1;
            
            float taille_tranche = (2.0f * PI) / 37.0f;
            float angle_final = fmodf(angle_bille + offset_image, 2.0f * PI);
            if (angle_final < 0) angle_final += 2.0f * PI;
            
            float angle_corrige = fmodf(angle_final + (taille_tranche / 2.0f), 2.0f * PI);
            int index_case = (int)(angle_corrige / taille_tranche);
            
            if (index_case >= 37) index_case = 0;
            numero_gagnant = ordre_roulette[index_case];
            
            TraceLog(LOG_INFO, "LA BILLE EST TOMBEE SUR LE : %d", numero_gagnant);
        }

        
        // --- SCORES JOUEUR 1 ---
        if(vitesse_rotation == 0.0f && fin_score != 0) {
            k = 0;        // RESET IMPORTANT
            win1 = 0;     // (optionnel mais conseillé)
            for (i = 0; i<points_count; ++i) {
                Vector2 pos_pion = points_verts[i]; 

                if (CheckCollisionPointRec(pos_pion, nombres)) {
                    float largeur_case = nombres.width / 12.0f; 
                    float hauteur_case = nombres.height / 3.0f; 
                    int index_colonne = (int)((pos_pion.x - nombres.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion.y - nombres.y) / hauteur_case);
                    if (index_colonne > 11) index_colonne = 11;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 2)    index_ligne = 2;
                    if (index_ligne < 0)    index_ligne = 0;
                    
                    if (grille_nombres[index_ligne][index_colonne] == numero_gagnant) {
                        win1 += 36;
                        score += 36;
                        win_verts[k] = points_verts[i];
                        k += 1;
                    }
                }
                else if (CheckCollisionPointRec(pos_pion, zero)) {
                    if (numero_gagnant == 0) {
                        win1 += 36;
                        score += 36;
                        win_verts[k] = points_verts[i];
                        k += 1;
                    }
                }
                else if (CheckCollisionPointRec(pos_pion, bizarre)) {
                    float largeur_case = bizarre.width / 6.0f; 
                    float hauteur_case = bizarre.height / 2.0f; 
                    int index_colonne = (int)((pos_pion.x - bizarre.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion.y - bizarre.y) / hauteur_case);
                    if (index_colonne > 5) index_colonne = 5;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 1)    index_ligne = 1;
                    if (index_ligne < 0)    index_ligne = 0;
                    
                    int numero_mise = grille_bizarre[index_ligne][index_colonne];
                    
                    if(index_ligne == 0) {
                        if (numero_mise == table_roulette[numero_gagnant].douzaine) {
                            win1 += 3; 
                            score += 3;
                            win_verts[k] = points_verts[i];
                            k += 1;
                        }
                    }
                    else if(index_colonne == 0 || index_colonne == 5) {
                        if (numero_mise == table_roulette[numero_gagnant].moitie) {
                            win1 += 2; 
                            score += 2;
                            win_verts[k] = points_verts[i];
                            k += 1;
                        }
                    }
                    else if(index_colonne == 1 || index_colonne == 4) {
                        if (numero_mise == table_roulette[numero_gagnant].parite) {
                            win1 += 2; 
                            score += 2;
                            win_verts[k] = points_verts[i];
                            k += 1;
                        }
                       
                    }
                    else if(index_colonne == 2 || index_colonne == 3) {
                        if (numero_mise == table_roulette[numero_gagnant].couleur) {
                            win1 += 2;
                            score += 2;
                            win_verts[k] = points_verts[i];
                            k += 1;
                        }
                        
                    }
                }
                else if(CheckCollisionPointRec(pos_pion, dernier)) {
                    float largeur_case = dernier.width / 1.0f; 
                    float hauteur_case = dernier.height / 3.0f; 
                    int index_colonne = (int)((pos_pion.x - dernier.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion.y - dernier.y) / hauteur_case);
                    if (index_colonne > 0) index_colonne = 0;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 2)    index_ligne = 2;
                    if (index_ligne < 0)    index_ligne = 0;

                    if (grille_dernier[index_ligne][index_colonne] == table_roulette[numero_gagnant].colonne) {
                        win1 += 3; 
                        score += 3;
                        win_verts[k] = points_verts[i];
                        k += 1;
                    }
                    
                }
            }
            fin_score = 0;
        }

        // --- SCORES JOUEUR 2 ---
        if(vitesse_rotation == 0.0f && fin_score2 != 0) {
            l = 0;        // RESET IMPORTANT
            win2 = 0;
            for (i = 0; i<points_count2; ++i) {
                Vector2 pos_pion2 = points_jaunes[i]; 

                if (CheckCollisionPointRec(pos_pion2, nombres)) {
                    float largeur_case = nombres.width / 12.0f; 
                    float hauteur_case = nombres.height / 3.0f; 
                    int index_colonne = (int)((pos_pion2.x - nombres.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion2.y - nombres.y) / hauteur_case);
                    if (index_colonne > 11) index_colonne = 11;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 2)    index_ligne = 2;
                    if (index_ligne < 0)    index_ligne = 0;
                    
                    if (grille_nombres[index_ligne][index_colonne] == numero_gagnant) {
                        win2 += 36; 
                        score2 += 36; 
                        win_jaunes[l] = points_jaunes[i];
                        l += 1;
                    }
                }
                else if (CheckCollisionPointRec(pos_pion2, zero)) {
                    if (numero_gagnant == 0) {
                        win2 += 36; 
                        score2 += 36; 
                        win_jaunes[l] = points_jaunes[i];
                        l += 1;
                    }
                }
                else if (CheckCollisionPointRec(pos_pion2, bizarre)) {
                    float largeur_case = bizarre.width / 6.0f; 
                    float hauteur_case = bizarre.height / 2.0f; 
                    int index_colonne = (int)((pos_pion2.x - bizarre.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion2.y - bizarre.y) / hauteur_case);
                    if (index_colonne > 5) index_colonne = 5;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 1)    index_ligne = 1;
                    if (index_ligne < 0)    index_ligne = 0;
                    
                    int numero_mise = grille_bizarre[index_ligne][index_colonne];
                    
                    if(index_ligne == 0) { 
                        if (numero_mise == table_roulette[numero_gagnant].douzaine) {
                            win2 += 3; 
                            score2 += 3; 
                            win_jaunes[l] = points_jaunes[i];
                            l += 1;
                        }
                    }
                    else if(index_colonne == 0 || index_colonne == 5) {
                        if (numero_mise == table_roulette[numero_gagnant].moitie) {
                            win2 += 2; 
                            score2 += 2; 
                            win_jaunes[l] = points_jaunes[i];
                            l += 1;
                        }
                        
                    }
                    else if(index_colonne == 1 || index_colonne == 4) {
                        if (numero_mise == table_roulette[numero_gagnant].parite) {
                            win2 += 2; 
                            score2 += 2; 
                            win_jaunes[l] = points_jaunes[i];
                            l += 1;
                        }
                       
                    }
                    else if(index_colonne == 2 || index_colonne == 3) {
                        if (numero_mise == table_roulette[numero_gagnant].couleur) {
                            win2 += 2; 
                            score2 += 2; 
                            win_jaunes[l] = points_jaunes[i];
                            l += 1;
                        }
                        
                    }
                }
                else if(CheckCollisionPointRec(pos_pion2, dernier)) {
                    float largeur_case = dernier.width / 1.0f; 
                    float hauteur_case = dernier.height / 3.0f; 
                    int index_colonne = (int)((pos_pion2.x - dernier.x) / largeur_case);
                    int index_ligne   = (int)((pos_pion2.y - dernier.y) / hauteur_case);
                    if (index_colonne > 0) index_colonne = 0;
                    if (index_colonne < 0)  index_colonne = 0;
                    if (index_ligne > 2)    index_ligne = 2;
                    if (index_ligne < 0)    index_ligne = 0;

                    if (grille_dernier[index_ligne][index_colonne] == table_roulette[numero_gagnant].colonne) {
                        win2 += 3; 
                        score2 += 3; 
                        win_jaunes[l] = points_jaunes[i];
                        l += 1;
                    }
                    
                }
            }
            fin_score2 = 0;
        }
        
        // ==========================================
        // ------FIN DE MANCHE ET VÉRIFICATIONS------
        // ==========================================
        if(vitesse_rotation == 0.0f && fin_score2 != 0) {
            manches_jouees++;
            // Condition 1 : Un joueur atteint 36
            if (score >= 36 || score2 >= 36) {
                if (score > score2) vainqueur = 1;
                else if (score2 > score) vainqueur = 2;
                else vainqueur = 0;
            } 
            // Condition 2 : Un joueur fait banqueroute
            else if (score <= 0 || score2 <= 0) {
                if (score > score2) vainqueur = 1;
                else if (score2 > score) vainqueur = 2;
                else vainqueur = 0;
            } 
            // Condition 3 : Fin des 3 manches
            else if (manches_jouees >= 3) {
                if (score > score2) vainqueur = 1;
                else if (score2 > score) vainqueur = 2;
                else vainqueur = 0;
            }
        }

        // --- Lancement nouvelle partie ---
        if (roulette_terminee == 1) {
            timer_fin_manche += GetFrameTime();
            
            // On laisse les résultats affichés pendant 5 secondes
            if (timer_fin_manche > 5.0f) {
                if (vainqueur != -1) {
                    break; // On sort de la boucle while, la partie est finie !
                } else {
                    // RESET POUR LA MANCHE SUIVANTE
                    points_count = 0;
                    points_count2 = 0;
                    k = 0;
                    l = 0;
                    choose = 0;
                    timer = 0;
                    chrono = 0.0f;
                    timer_fin_manche = 0.0f;
                    vitesse_rotation = ((rand()%50)+rand()%30)/100.0f; // Nouvelle impulsion !
                    roulette_terminee = 0;
                    numero_gagnant = -1;
                }
            }
        }

        // ==========================================
        // AFFICHAGE (DRAWING)
        // ==========================================
        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(background, source, dest, Vector2Zero(), 0.0f, WHITE);

            if(numero_gagnant == -1)
            {
                for (i = 0; i < points_count; i++) {
                    DrawTextureRec(pion, pion1, (Vector2){points_verts[i].x-12, points_verts[i].y-28}, WHITE);
                }
                for (i = 0; i < points_count2; i++) {
                    DrawTextureRec(pion, pion2, (Vector2){points_jaunes[i].x-12, points_jaunes[i].y-28}, WHITE);
                }
            }
            if(numero_gagnant != -1)
            {
                for (i = 0; i < k; i++) {
                    DrawTextureRec(pion, pion1, (Vector2){win_verts[i].x-12, win_verts[i].y-28}, WHITE);
                }
                for (i = 0; i < l; i++) {
                    DrawTextureRec(pion, pion2, (Vector2){win_jaunes[i].x-12, win_jaunes[i].y-28}, WHITE);
                }
            }
            DrawText(TextFormat("%d/36", score - points_count), 1080, 20, 20, RED);
            DrawText(TextFormat("%d/36", score2 - points_count2), 720, 20, 20, WHITE);
            DrawText(TextFormat("Manche numéro : %d/3", manches_jouees+1), 47, 74, 37, WHITE);

            DrawTexture(cursor, Player_pos_cas.x, Player_pos_cas.y, WHITE);
            DrawTexture(cursor, Player_pos_cas2.x, Player_pos_cas2.y, WHITE);
            
            DrawCircleV(pos_bille, 10, WHITE);
            DrawCircleV(pos_bille, 1.0f, BLUE);

            if (roulette_terminee == 1) {
                DrawText(TextFormat("LE NUMERO GAGNANT EST : %d", numero_gagnant), 600, 60, 40, GREEN);
                DrawText(TextFormat("+%d", win1), SCREEN_WIDTH - 630, 600, 30, WHITE);
                DrawText(TextFormat("+%d", win2), SCREEN_WIDTH -100, 600, 30, RED);
            }
            if (choose == 0) {
                DrawText(TextFormat("Faites vos jeux !! : %d", 15-(timer/60)), 720, 60, 40, GREEN);
            } else if (vitesse_rotation != 0.0f) {
                DrawText(TextFormat("Rien ne va plus !!!"), 760, 60, 40, GREEN);
            }

        EndDrawing();
    }

    UnloadTexture(pion);   
    UnloadTexture(cursor);
    UnloadTexture(background);
    
    CloseWindow();
    return vainqueur;
}