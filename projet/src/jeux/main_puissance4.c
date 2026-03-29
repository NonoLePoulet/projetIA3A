#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1242
#define SCREEN_HEIGHT 708

// ==========================================
// PARAMETRES DU PUISSANCE 4
// ==========================================
#define LIGNES 6
#define COLONNES 7
#define VIDE 0
#define JOUEUR_1 1
#define JOUEUR_2 2 

#define PROFONDEUR_MAX 7 

// ==========================================
// 1. FONCTIONS LOGIQUES DU JEU ET DE L'IA
// ==========================================

int verifier_victoire(int grille[LIGNES][COLONNES], int joueur) {
    for (int l = 0; l < LIGNES; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            if (grille[l][c] == joueur && grille[l][c+1] == joueur && 
                grille[l][c+2] == joueur && grille[l][c+3] == joueur) return 1;
        }
    }
    for (int l = 0; l < LIGNES - 3; l++) {
        for (int c = 0; c < COLONNES; c++) {
            if (grille[l][c] == joueur && grille[l+1][c] == joueur && 
                grille[l+2][c] == joueur && grille[l+3][c] == joueur) return 1;
        }
    }
    for (int l = 0; l < LIGNES - 3; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            if (grille[l][c] == joueur && grille[l+1][c+1] == joueur && 
                grille[l+2][c+2] == joueur && grille[l+3][c+3] == joueur) return 1;
        }
    }
    for (int l = 3; l < LIGNES; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            if (grille[l][c] == joueur && grille[l-1][c+1] == joueur && 
                grille[l-2][c+2] == joueur && grille[l-3][c+3] == joueur) return 1;
        }
    }
    return 0;
}

// Évalue une "fenêtre" de 4 cases pour donner un score à la position
int evaluer_fenetre(int fenetre[4], int joueur) {
    int score = 0;
    int adversaire = (joueur == JOUEUR_1) ? JOUEUR_2 : JOUEUR_1;
    int pions_joueur = 0;
    int pions_adv = 0;
    int cases_vides = 0;

    for (int i = 0; i < 4; i++) {
        if (fenetre[i] == joueur) pions_joueur++;
        else if (fenetre[i] == adversaire) pions_adv++;
        else cases_vides++;
    }

    // Le joueur a presque gagné
    if (pions_joueur == 4) score += 100000;
    else if (pions_joueur == 3 && cases_vides == 1) score += 50;
    else if (pions_joueur == 2 && cases_vides == 2) score += 10;

    // L'adversaire a presque gagné (IL FAUT LE BLOQUER ABSOLUMENT)
    if (pions_adv == 3 && cases_vides == 1) score -= 80;

    return score;
}

// Parcours toute la grille pour lui donner une note globale
int evaluer_grille(int grille[LIGNES][COLONNES], int joueur) {
    int score = 0;
    int fenetre[4];

    // 1. Favoriser le contrôle du centre (Stratégie de base au Puissance 4)
    int centre_col = COLONNES / 2;
    int pions_centre = 0;
    for (int l = 0; l < LIGNES; l++) {
        if (grille[l][centre_col] == joueur) pions_centre++;
    }
    score += pions_centre * 30; // +30 points par pion dans la colonne centrale

    // 2. Évaluation Horizontale
    for (int l = 0; l < LIGNES; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            for (int i = 0; i < 4; i++) fenetre[i] = grille[l][c+i];
            score += evaluer_fenetre(fenetre, joueur);
        }
    }
    // 3. Évaluation Verticale
    for (int l = 0; l < LIGNES - 3; l++) {
        for (int c = 0; c < COLONNES; c++) {
            for (int i = 0; i < 4; i++) fenetre[i] = grille[l+i][c];
            score += evaluer_fenetre(fenetre, joueur);
        }
    }
    // 4. Évaluation Diagonale (Descendante et Montante)
    for (int l = 0; l < LIGNES - 3; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            for (int i = 0; i < 4; i++) fenetre[i] = grille[l+i][c+i];
            score += evaluer_fenetre(fenetre, joueur);
        }
    }
    for (int l = 3; l < LIGNES; l++) {
        for (int c = 0; c < COLONNES - 3; c++) {
            for (int i = 0; i < 4; i++) fenetre[i] = grille[l-i][c+i];
            score += evaluer_fenetre(fenetre, joueur);
        }
    }
    return score;
}

int trouver_ligne_libre(int grille[LIGNES][COLONNES], int col) {
    for (int l = LIGNES - 1; l >= 0; l--) {
        if (grille[l][col] == VIDE) return l;
    }
    return -1; 
}

int minimax(int grille[LIGNES][COLONNES], int profondeur, int alpha, int beta, int est_maximisant) {
    if (verifier_victoire(grille, JOUEUR_2)) return 1000000; 
    if (verifier_victoire(grille, JOUEUR_1)) return -1000000; 
    if (profondeur == 0) return evaluer_grille(grille, JOUEUR_2); //Si on atteint la limite de profondeur, on évalue intelligemment la position !

    //Pour une IA plus facile à battre
    //if (profondeur == 0) return 0;

    int grille_pleine = 1;
    for (int c = 0; c < COLONNES; c++) {
        if (grille[0][c] == VIDE) grille_pleine = 0;
    }
    if (grille_pleine) return 0;

    if (est_maximisant) {
        int max_eval = -999999;
        for (int c = 0; c < COLONNES; c++) {
            int l = trouver_ligne_libre(grille, c);
            if (l != -1) {
                grille[l][c] = JOUEUR_2; 
                int eval = minimax(grille, profondeur - 1, alpha, beta, 0);
                grille[l][c] = VIDE; 

                if (eval > max_eval) max_eval = eval;
                if (eval > alpha) alpha = eval;
                if (alpha >= beta) break; 
            }
        }
        return max_eval;
    } else {
        int min_eval = 999999;
        for (int c = 0; c < COLONNES; c++) {
            int l = trouver_ligne_libre(grille, c);
            if (l != -1) {
                grille[l][c] = JOUEUR_1; 
                int eval = minimax(grille, profondeur - 1, alpha, beta, 1);
                grille[l][c] = VIDE; 

                if (eval < min_eval) min_eval = eval;
                if (eval < beta) beta = eval;
                if (alpha >= beta) break; 
            }
        }
        return min_eval;
    }
}

// ==========================================
// 2. FONCTION PRINCIPALE DU JEU
// ==========================================
int main_puissance4(int mode_ia) 
{
    srand(time(NULL));
    float timer_fin = 0.0f;
    float chrono = 0.0f;
    int grille[LIGNES][COLONNES] = {0};
    int colonne_choisie1 = 3;   
    int colonne_choisie2 = 3;   
    int vainqueur = 0;
    int tour_joueur = rand() %2 +1; //Pour commencer aléatoirement

    //Mémoire pour le mouvement fluide de l'IA
    int ia_calcule = 0; 
    int ia_colonne_cible = 3;
    int ia_timer_mouvement = 0;
    
    int taille_case = 90;
    int offset_x = (SCREEN_WIDTH - (COLONNES * taille_case)) / 2;
    int offset_y = 150;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Puissance 4 - Projet IA");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("src/textures/p4.png");

    // Chargement et redimensionnement du Panda (Joueur 1)
    Image imgPanda = LoadImage("src/textures/panda.png");
    ImageResize(&imgPanda, taille_case +12, taille_case+12); // Redimensionne à 80x80 pour rentrer dans la case de 90
    Texture2D panda = LoadTextureFromImage(imgPanda);
    UnloadImage(imgPanda); // On libère l'image de la RAM, on ne garde que la texture

    // Chargement et redimensionnement du Chat (Joueur 2)
    Image imgChat = LoadImage("src/textures/chat.png");
    ImageResize(&imgChat, taille_case+12, taille_case+12);
    Texture2D chat = LoadTextureFromImage(imgChat);
    UnloadImage(imgChat);

    // Calcul de la taille exacte du plateau
    int largeur_plateau = (COLONNES * taille_case) + 20;
    int hauteur_plateau = (LIGNES * taille_case) + 20;

    // Chargement et redimensionnement de l'image de fond du plateau
    Image imgplat = LoadImage("src/textures/gravel4.png");
    ImageResize(&imgplat, largeur_plateau, hauteur_plateau);
    Texture2D plat = LoadTextureFromImage(imgplat);
    UnloadImage(imgplat);

    // ==========================================
    // CREATION DU MASQUE DU PLATEAU 
    // ==========================================
    RenderTexture2D masque_plateau = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    BeginTextureMode(masque_plateau);
        ClearBackground(BLANK); // On part sur un fond virtuellement transparent
        
        // 1. On dessine le gros rectangle bleu
        DrawTexture(plat, offset_x - 10, offset_y - 10, WHITE);
        
        // 2. On "creuse" les trous
        BeginBlendMode(BLEND_SUBTRACT_COLORS);
        for (int l = 0; l < LIGNES; l++) {
            for (int c = 0; c < COLONNES; c++) {
                int centre_x = offset_x + (c * taille_case) + (taille_case / 2);
                int centre_y = offset_y + (l * taille_case) + (taille_case / 2);
                
                // Soustraire du bleu foncé au bleu foncé donne une transparence parfaite (le trou !)
                DrawCircle(centre_x, centre_y, taille_case / 2 - 5, DARKBLUE);
            }
        }
        EndBlendMode();
    EndTextureMode();

    // ==========================================
    // 3. BOUCLE PRINCIPALE
    // ==========================================
    while (!WindowShouldClose()) {

        // --- GESTION DU JOUEUR 1 ---
        if (tour_joueur == JOUEUR_1 && vainqueur == 0) {
            if (IsKeyPressed(KEY_LEFT) && colonne_choisie1 > 0) colonne_choisie1--;
            if (IsKeyPressed(KEY_RIGHT) && colonne_choisie1 < COLONNES - 1) colonne_choisie1++;

            if (IsKeyPressed(KEY_ENTER)) {
                int ligne_libre = trouver_ligne_libre(grille, colonne_choisie1);
                if (ligne_libre != -1) {
                    grille[ligne_libre][colonne_choisie1] = JOUEUR_1;
                    if (verifier_victoire(grille, JOUEUR_1))
                    {
                        vainqueur = JOUEUR_1;
                    }
                    else tour_joueur = JOUEUR_2; 
                }
            }
        }
        
        // --- GESTION DU JOUEUR 2 (HUMAIN OU IA) ---
        else if (tour_joueur == JOUEUR_2 && vainqueur == 0) {
            if (mode_ia == 0) {
                if (IsKeyPressed(KEY_A) && colonne_choisie2 > 0) colonne_choisie2--;
                if (IsKeyPressed(KEY_D) && colonne_choisie2 < COLONNES - 1) colonne_choisie2++;

                if (IsKeyPressed(KEY_SPACE)) {
                    int ligne_libre = trouver_ligne_libre(grille, colonne_choisie2);
                    if (ligne_libre != -1) {
                        grille[ligne_libre][colonne_choisie2] = JOUEUR_2;
                        if (verifier_victoire(grille, JOUEUR_2)) vainqueur = JOUEUR_2;
                        else tour_joueur = JOUEUR_1;
                    }
                }
            } 
            else if (mode_ia == 1) {
                // --- LOGIQUE D'IA FLUIDE ET NATURELLE ---
                
                if (ia_calcule == 0) {
                    // ETAPE 1 : L'IA calcule silencieusement son meilleur coup
                    int meilleur_score = -999999;
                    int meilleure_colonne = 0;

                    for (int c = 0; c < COLONNES; c++) {
                        int ligne_libre = trouver_ligne_libre(grille, c);
                        if (ligne_libre != -1) {
                            grille[ligne_libre][c] = JOUEUR_2; 
                            int score_coup = minimax(grille, PROFONDEUR_MAX - 1, -999999, 999999, 0);
                            grille[ligne_libre][c] = VIDE; 

                            if (score_coup > meilleur_score) {
                                meilleur_score = score_coup;
                                meilleure_colonne = c;
                            }
                        }
                    }
                    ia_colonne_cible = meilleure_colonne; 
                    ia_calcule = 1; 
                    
                    // Astuce : On met un minuteur négatif pour forcer l'IA à 
                    // "réfléchir" sans bouger pendant une demi-seconde !
                    ia_timer_mouvement = -30; 
                } 
                else if (ia_calcule == 1) {
                    // ETAPE 2 : L'IA déplace son curseur vers la cible
                    ia_timer_mouvement++;
                    
                    // Vitesse du déplacement : 15 frames entre chaque case (plus lent et naturel)
                    if (ia_timer_mouvement > 15) { 
                        ia_timer_mouvement = 0;

                        if (colonne_choisie2 < ia_colonne_cible) {
                            colonne_choisie2++; 
                        } else if (colonne_choisie2 > ia_colonne_cible) {
                            colonne_choisie2--; 
                        } else {
                            // Le curseur est arrivé au-dessus de la bonne colonne !
                            // On passe à l'étape 3 sans lâcher le pion tout de suite.
                            ia_calcule = 2; 
                            ia_timer_mouvement = 0; 
                        }
                    }
                }
                else if (ia_calcule == 2) {
                    // ETAPE 3 : Petite hésitation au-dessus du trou puis lâcher du pion
                    ia_timer_mouvement++;
                    
                    // L'IA attend environ 0.3 seconde avant de valider son coup
                    if (ia_timer_mouvement > 20) { 
                        int ligne_finale = trouver_ligne_libre(grille, ia_colonne_cible);
                        grille[ligne_finale][ia_colonne_cible] = JOUEUR_2;

                        if (verifier_victoire(grille, JOUEUR_2)) vainqueur = JOUEUR_2;
                        else tour_joueur = JOUEUR_1; 
                        
                        ia_calcule = 0; // Réinitialise le cerveau pour son prochain tour
                    }
                }
            }
        }

        //----FIN DE PARTIE -----
        chrono += GetFrameTime();
        if(vainqueur!=0)
        {
            timer_fin+= GetFrameTime();
            if(timer_fin>7.0f)
            {
                break;
            }
        }

        // ==========================================
        // AFFICHAGE (DRAWING)
        // ==========================================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 1. COUCHE DU FOND : L'image de background général
        DrawTexturePro(
            background,
            (Rectangle){ 0, 0, background.width, background.height }, 
            (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },         
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );

        // 2. COUCHE DU MILIEU : On dessine les jetons (qui vont glisser derrière le plateau)
        for (int l = 0; l < LIGNES; l++) {
            for (int c = 0; c < COLONNES; c++) {
                int pos_x = offset_x + (c * taille_case) - 6;
                int pos_y = offset_y + (l * taille_case) - 6;

                if (grille[l][c] == JOUEUR_1) {
                    DrawTexture(panda, pos_x, pos_y, WHITE);
                } 
                else if (grille[l][c] == JOUEUR_2) {
                    DrawTexture(chat, pos_x, pos_y, WHITE);
                }
            }
        }

        // 3. COUCHE DU DESSUS : Le plateau percé (le gravel) !
        // On affiche la texture générée avant la boucle. 
        // Note: Les RenderTexture sont inversées verticalement, on met une hauteur négative.
        DrawTextureRec(
            masque_plateau.texture, 
            (Rectangle){ 0, 0, (float)masque_plateau.texture.width, -(float)masque_plateau.texture.height }, 
            (Vector2){ 0, 0 }, 
            WHITE
        );

        // 4. INTERFACE : Textes d'informations et curseurs
        if (vainqueur == 0) {
            DrawText("REGLES : ALIGNER 4 PIONS POUR GAGNER !!", 300, 30, 30, WHITE);
            if (tour_joueur == JOUEUR_1) {
                DrawText("TOUR DU JOUEUR 1 (Panda) - Touches: <- -> et ENTREE", 300, 90, 20, SKYBLUE);
                // Curseur du J1
                int pos_x = offset_x + (colonne_choisie1 * taille_case) - 6;
                DrawTexture(panda, pos_x, offset_y - 85, WHITE);
            } 
            else if (tour_joueur == JOUEUR_2) {
                if (mode_ia == 0) {
                    DrawText("TOUR DU JOUEUR 2 (Chat) - Touches: A D et ESPACE", 300, 90, 20, ORANGE);
                    // Curseur du J2 humain
                    int pos_x = offset_x + (colonne_choisie2 * taille_case) - 6;
                    DrawTexture(chat, pos_x, offset_y - 85, WHITE);
                } else {
                    DrawText("L'IA REFLECHIT ET DEPLACE SON PION...", 400, 90, 20, ORANGE);
                    // CURSEUR DE L'IA (C'est cette ligne qui manquait !)
                    int pos_x = offset_x + (colonne_choisie2 * taille_case) - 6;
                    DrawTexture(chat, pos_x, offset_y - 85, WHITE); 
                }
            }
        }
        else
        {
            DrawText(TextFormat("Le joueur %d a gagné la partie !! Félicitations", vainqueur), 200, 60, 40, GREEN);
        }

        EndDrawing();
    }
    // Nettoyage de la mémoire à la fermeture
    UnloadRenderTexture(masque_plateau); 
    UnloadTexture(background);
    UnloadTexture(panda);
    UnloadTexture(chat);
    UnloadTexture(plat);
    CloseWindow();
    return vainqueur;
}