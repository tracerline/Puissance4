//
// Created by Adam HAISSOUBI-VOGIER and Jerome LONGUEBRAY on 13/01/2022.
//

#include "Erreurs.h"
#include "Jeu.h"

// vérifie que le joueur entre un entier entre 1 et 7
int checkErrorInput(int input) {
    if (input <= 7 && input >= 1)
        return 0;
    return 1;
}

// vérifie si une colonne est pleine
int checkErrorFullColonne(Partie* partie, int colonne) {
    if (partie->plateau[5][colonne-1] != VIDE) {
        return 1;
    } else {
        return 0;
    }
}

// surveille sur la grille est pleine
int checkFullMap(Partie* partie) {
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; ++j) {
            if (partie->plateau[i][j] == VIDE) {
                return 0;
            }
        }
    }
    return 1;
}