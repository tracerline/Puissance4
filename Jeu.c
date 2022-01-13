#include "Jeu.h"
#include "IA.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Erreurs.h"

#define COLONNE 7;
#define LIGNE 6;

void afficher(Partie* partie) {
    printf("\n");
    // affiche le plateau
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; ++j) {
            if(partie->plateau[i][j] == VIDE){
                printf("[   ]");
            }
            else if(partie->plateau[i][j] == J1) {
                printf("[ X ]");
            }
            else if(partie->plateau[i][j] == J2) {
                printf("[ O ]");
            }

        }
        printf("\n");
    }
    for (int i = 0; i < 7; ++i) {
        printf("  %d  ", i+1);
    }
    //choixPremierJoueur();
    printf("\n");
}

int choixPremierJoueur(void) {
    srand(time(NULL));
    int joueurChoisi = rand()%((2)) + 1;
    return joueurChoisi;
}

// return la première ligne vide en fonction d'une colonne
int empilementPion(Partie* partie, int colonne) {
    int position = 0;
    for (int i = 5; i >= 0 ; i--) {
        if(partie->plateau[i][colonne] != VIDE) {
            position += 1;
        }
    }

    printf("Position : %d", position);
    return position;
}

int jouerCoup(Partie* partie, int colonne) {
    int position = empilementPion(partie, colonne);

    // le joueur joue son coup
    if (partie->tour == 1) {
        partie->plateau[position][colonne] = J1;
        partie->tour += 1;// assigne le choix du joueur dans la grille
    } else {
        partie->plateau[position][colonne] = J2;
        partie->tour -= 1;// assigne le choix du joueur dans la grille
    }
    return 0;


}

int verifierAlignementJ2(Partie* partie, int i, int j) { // fonction qui surveille si un pion est le premier d'un aligement de 4 pions
    //si alignement horizontal
    if ((partie->plateau[i][j+1] == J2) && (partie->plateau[i][j+2] == J2) && (partie->plateau[i][j+3] == J2)) {
        return 1;
    }
        //si alignement vertical
    else if ((partie->plateau[i+1][j] == J2) && (partie->plateau[i+2][j] == J2) && (partie->plateau[i+3][j] == J2)) {
        return 1;
    }
        //si alignement diagonale haute
    else if ((partie->plateau[i+1][j+1] == J2) && (partie->plateau[i+2][j+2] == J2) && (partie->plateau[i+3][j+3] == J2)) {
        return 1;
    }
        //si alignement diagonale basse
    else if ((partie->plateau[i-1][j-1] == J2) && (partie->plateau[i-2][j-2] == J2) && (partie->plateau[i-3][j-3] == J2)) {
        return 1;
    }
    return 0; //si aucun alignement
}

int verifierAlignementJ1(Partie* partie, int i, int j) { // fonction qui surveille si un pion est le premier d'un aligement de 4 pions
    //si alignement horizontal
    if ((partie->plateau[i][j+1] == J1) && (partie->plateau[i][j+2] == J1) && (partie->plateau[i][j+3] == J1)) {
        return 1;
    }
    //si alignement vertical
    else if ((partie->plateau[i+1][j] == J1) && (partie->plateau[i+2][j] == J1) && (partie->plateau[i+3][j] == J1)) {
        return 1;
    }
    //si alignement diagonale haute
    else if ((partie->plateau[i+1][j+1] == J1) && (partie->plateau[i+2][j+2] == J1) && (partie->plateau[i+3][j+3] == J1)) {
        return 1;
    }
    //si alignement diagonale basse
    else if ((partie->plateau[i-1][j-1] == J1) && (partie->plateau[i-2][j-2] == J1) && (partie->plateau[i-3][j-3] == J1)) {
        return 1;
    }
    return 0; //si aucun alignement
}

// vérifie les conditions de victoire
Etat calculerEtat(Partie* partie) {
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; ++j) {
            // vérifie la victoire du joueur 1
            if(partie->plateau[i][j] == J1){
                if (verifierAlignementJ1(partie, i, j) == 1) {
                    printf("\nJoueur 1 a gagné !\n");
                    return VICTOIRE_J1;
                }
            }
            // vérifie la victoire du joueur 2
            if (partie->plateau[i][j] == J2){
                if (verifierAlignementJ2(partie, i, j) == 1) {
                    printf("\nJoueur 2 a gagné !\n");
                    return VICTOIRE_J2;
                }
            }
            // vérifie s'il y a une égalité
            if (checkFullMap(partie) == 1) {
                printf("\négalité !\n");
                return EGALITE;
            }
        }
    }
    return EN_COURS;
}

int bouclePrincipale(Partie* partie) {
    int responsePlayer;
    int errorEntier = 0;
    int errorColonne = 0;

    afficher(partie);
    while(1) { // boucle de jeu
        if (calculerEtat(partie) == EN_COURS) { // partie en cours de jeu (pas de victoire ni d'égalité)
            printf("Joueur %d, choisissez un nombre entier entre 1 et 7 : ", partie->tour); // afficher le plateau
            scanf("%d", &responsePlayer); // demander au joueur la colonne

            // vérifie que le joueur entre un entier entre 1 et 7
            errorEntier = checkErrorInput(responsePlayer);
            while (errorEntier == 1) {
                printf("Joueur %d, choisissez un nombre entier entre 1 et 7 : ", partie->tour);
                scanf("%d", &responsePlayer); // récupère la colonne voulue
                errorEntier = checkErrorInput(responsePlayer);
            }

            // vérifie si une colonne est pleine
            errorColonne = checkErrorFullColonne(partie, responsePlayer);
            while (errorColonne == 1) {
                printf("Joueur %d, cette colonne est pleine, veuillez choisir une autre colonne : ", partie->tour);
                scanf("%d", &responsePlayer); // récupère la colonne voulue
                errorColonne = checkErrorFullColonne(partie, responsePlayer);
            }

            jouerCoup(partie, responsePlayer - 1); // le joueur joue son coup
            afficher(partie); // on met à jour le plateau
        } else {
            break; // End game
        }
    }
    return 0;
}