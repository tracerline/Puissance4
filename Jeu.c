#include "Jeu.h"
#include "IA.h"
#include <stdio.h>
#include <stdlib.h>

#define COLONNE 7;
#define LIGNE 6;

void afficher(Partie* partie){
    printf("\n");
    // affiche le plateau
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

// return la première ligne vide en fonction d'une colonne
int empilementPion(Partie* partie, int colonne){
    int position = 0;
    for (int i = 5; i >= 0 ; i--) {
        if(partie->plateau[i][colonne] != VIDE) {
            position += 1;
        }
    }
    printf("Position : %d", position);
    return position;
}

int jouerCoup(Partie* partie, int colonne){
    int position = empilementPion(partie, colonne);
     // le joueur joue son coup

    partie->plateau[position][colonne] = J1; // assigne le choix du joueur dans la grille
    return 0;
}

int verifierAlignement(Partie* partie, int i, int j) { // fonction qui surveille si un pion est le premier d'un aligement de 4 pions
    int fin_jeu = 0;
    //si alignement horizontal
    if ((partie->plateau[i][j+1] == J1) && (partie->plateau[i][j+2] == J1) && (partie->plateau[i][j+3] == J1)) {
        fin_jeu = 1;
    }
    //si alignement vertical
    else if ((partie->plateau[i+1][j] == J1) && (partie->plateau[i+2][j] == J1) && (partie->plateau[i+3][j] == J1)) {
        fin_jeu = 1;
    }
    //si alignement diagonale haute
    else if ((partie->plateau[i+1][j+1] == J1) && (partie->plateau[i+2][j+2] == J1) && (partie->plateau[i+3][j+3] == J1)) {
        fin_jeu = 1;
    }
    //si alignement diagonale basse
    else if ((partie->plateau[i-1][j-1] == J1) && (partie->plateau[i-2][j-2] == J1) && (partie->plateau[i-3][j-3] == J1)) {
        fin_jeu = 1;
    } //si aucun alignement
    else{
        return fin_jeu;
    }

}

//vérifie les conditions de victoire
Etat calculerEtat(Partie* partie) {
    for (int i = 5; i >=0; i--) {
        for (int j = 0; j < 7; ++j) {
            //vérifie la victoire du joueur 1
            if(partie->plateau[i][j] == J1){
                if (verifierAlignement(partie, i, j) == 1) {
                    return VICTOIRE_J1;
                }
            }

        }
    }
    return EN_COURS;
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