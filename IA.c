#include "IA.h"
#include <stdlib.h>
#include <stdio.h>

// TODO
//ajouter nb aligenement J2
//définir l'alignement le plus profitable

//WARNING : par convention, l'IA est le joueur 2 :)

Partie* copierPartie(Partie* partie) {
    return NULL;
}

// définit le nombre d’alignements de 4 pions d’IA possibles auxquels pourrait participer cette case
int verifierAlignementIA(Partie* partie, int i, int j) {
    int counter = 0;

    //si alignement horizontal droite
    if (((partie->plateau[i][j+1] == J2) || (partie->plateau[i][j+1] == VIDE)) && ((partie->plateau[i][j+2] == J2) || (partie->plateau[i][j+2] == VIDE)) && ((partie->plateau[i][j+3] == J2) || (partie->plateau[i][j+3] == VIDE))) {
        counter++;
    }
    //si alignement horizontal gauche
    if (((partie->plateau[i][j-1] == J2) || (partie->plateau[i][j-1] == VIDE)) && ((partie->plateau[i][j-2] == J2) || (partie->plateau[i][j-2] == VIDE)) && ((partie->plateau[i][j-3] == J2) || (partie->plateau[i][j-3] == VIDE))) {
        counter++;
    }
        //si alignement vertical haut
    if (((partie->plateau[i+1][j] == J2) || (partie->plateau[i+1][j] == VIDE)) && ((partie->plateau[i+2][j] == J2) || (partie->plateau[i+2][j] == VIDE)) && ((partie->plateau[i+3][j] == J2) || (partie->plateau[i+3][j] == VIDE))) {
        counter++;
    }
        //si alignement vertical bas
    if (((partie->plateau[i-1][j] == J2) || (partie->plateau[i-1][j] == VIDE)) && ((partie->plateau[i-2][j] == J2) || (partie->plateau[i-2][j] == VIDE)) && ((partie->plateau[i-3][j] == J2) || (partie->plateau[i-3][j] == VIDE))) {
        counter++;
    }
        //si alignement diagonale haute haute
    if (((partie->plateau[i+1][j+1] == J2) || (partie->plateau[i+1][j+1] == VIDE)) && ((partie->plateau[i+2][j+2] == J2) || (partie->plateau[i+2][j+2] == VIDE)) && ((partie->plateau[i+3][j+3] == J2) || (partie->plateau[i+3][j+3] == VIDE))) {
        counter++;
    }
        //si alignement diagonale basse
    if (((partie->plateau[i-1][j-1] == J2) || (partie->plateau[i-1][j-1] == VIDE)) && ((partie->plateau[i-2][j-2] == J2) || (partie->plateau[i-2][j-2] == VIDE)) && ((partie->plateau[i-3][j-3] == J2) || (partie->plateau[i-3][j-3] == VIDE))) {
        counter++;
    }
    return counter;
}

int evaluationCase(Partie* partie, int ligne, int colonne) {
    // si jouer dans la case permet à l’IA de gagner
    if (calculerEtat(partie) == VICTOIRE_J2) {
        return 1000;
    }
    // si jouer dans la case permet à l’IA d’empêcher le joueur d’aligner 4 pions
    if (calculerEtat(partie) == VICTOIRE_J1) {
        return 900;
    }
    return verifierAlignementIA(partie, ligne, colonne);
}

//Choix de la case à jouer par l'IA
int choixIA(Partie* partie, int ligne, int colonne) {
    int nbAlignementCase = 0;

    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; ++j) {
            while (partie->plateau[i][j] != VIDE) // se positionne dans le première case vide de la colonne
                i++;

            nbAlignementCase = evaluationCase(partie, i, j);
            if (nbAlignementCase == 1000 ) { // IA choisit la case qui gagne
                return j;
            } else if (nbAlignementCase == 900) { // IA choisit la case qui bloque la vitoire de J1
                return j;
            }
        }
    }
}

int evaluation(Partie* partie) {
    return 0;
}

Arbre* minmax(Partie* partie, int profondeur, int IA) {
    return NULL;
}

void detruireArbre(Arbre* arbre) {
    
}