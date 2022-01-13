#include "Jeu.h"
#include "IA.h"
#include <stdio.h>
#include <stdlib.h>

#define COLONNE 7;
#define LIGNE 6;

void afficher(Partie* partie){
    printf("\n");
    // On affiche le plateau
    for (int i = 5; i >=0; i--) {
        for (int j = 0; j < 7; ++j) {
            if(partie->plateau[i][j] == VIDE){
                printf("[   ]");
            }
            else if(partie->plateau[i][j] == J1) {
                printf("[ X ]");
            }
            else if(partie->plateau[i][j] == J2){
                printf("[ O ]");
            }

        }
        printf("\n");
    }
    for (int i = 0; i < 7; ++i) {
        printf("  %d  ", i+1);
    }
    printf("\n");
}

int jouerCoup(Partie* partie, int colonne){
    partie->plateau[0][colonne] = J1; // assigne le choix du joueur dans la grille
    return 0;
}


int verifierAlignement(Partie* partie, int i, int j) { // fonction qui surveille si un point est le premier d'un aligement de 4 points
    if ((partie->plateau[i][j+1] == J1) && (partie->plateau[i][j+2] == J1) && (partie->plateau[i][j+3] == J1)) { //alignement horizontal
        return 1;
    }
    if ((partie->plateau[i+1][j] == J1) && (partie->plateau[i+2][j] == J1) && (partie->plateau[i+3][j] == J1)) { //alignement vertical
        return 1;
    }
    if ((partie->plateau[i+1][j+1] == J1) && (partie->plateau[i+2][j+2] == J1) && (partie->plateau[i+3][j+3] == J1)) { //alignement diagonale haute
        return 1;
    }
    if ((partie->plateau[i-1][j-1] == J1) && (partie->plateau[i-2][j-2] == J1) && (partie->plateau[i-3][j-3] == J1)) { //alignement diagonale basse
        return 1;
    }
    return 0;
}

Etat calculerEtat(Partie* partie) {

    //     for (int i = 5; i >=0; i--) {
    //        for (int j = 0; j < 7; ++j) {
    //            if(partie->plateau[i][j] == J1){
    //              if (verifierAlignement(partie->plateau[i][j], i, j) == 1) {
    //                  return VU
    //              }
    //            }
    return EN_COURS;
}

int empilement(Partie* partie, int colonne){
    
}

int bouclePrincipale(Partie* partie){
    int responsePlayer;
    while(1) { // durée du jeu
        if (calculerEtat(partie) == EN_COURS) { // partie en cours de jeu (pas de victoire ni d'égalité)
            afficher(partie);
            printf("Joueur X, choisissez une colonne [1-7] : "); // afficher le plateau

            scanf("%d", &responsePlayer); // demander au joueur la colonne
            printf("responsePlayer: %d ", responsePlayer);// récupère la colonne voulue

            jouerCoup(partie, responsePlayer-1); // le joueur joue son coup
            afficher(partie); // on met à jour le plateau
        } else { // sinon, si victoire ou égalité
            break; // on stop le jeu
        }
    }
    return 0;
}