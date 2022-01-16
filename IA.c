#include "IA.h"
#include <stdlib.h>
#include <stdio.h>

//WARNING : par convention, l'IA est le joueur 2 :)

Partie* copierPartie(Partie* partie) {
    return NULL;
}

// définit le nombre d’alignements de 4 pions d’IA possibles auxquels pourrait participer cette case
int AlignementIA(Partie* partie, int i, int j) {
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
    if (((partie->plateau[i+1][j-1] == J2) || (partie->plateau[i+1][j-1] == VIDE)) && ((partie->plateau[i+2][j-2] == J2) || (partie->plateau[i+2][j-2] == VIDE)) && ((partie->plateau[i+3][j-3] == J2) || (partie->plateau[i+3][j-3] == VIDE))) {
        counter++;
    }
    return counter;
}

// définit le nombre d’alignements de 4 pions du joueur 1 possibles auxquels pourrait participer cette case
int AlignementJ1(Partie* partie, int i, int j) {
    int counter = 0;

    //si alignement horizontal droite
    if (((partie->plateau[i][j+1] == J1) || (partie->plateau[i][j+1] == VIDE)) && ((partie->plateau[i][j+2] == J1) || (partie->plateau[i][j+2] == VIDE)) && ((partie->plateau[i][j+3] == J1) || (partie->plateau[i][j+3] == VIDE))) {
        counter++;
    }
    //si alignement horizontal gauche
    if (((partie->plateau[i][j-1] == J1) || (partie->plateau[i][j-1] == VIDE)) && ((partie->plateau[i][j-2] == J1) || (partie->plateau[i][j-2] == VIDE)) && ((partie->plateau[i][j-3] == J1) || (partie->plateau[i][j-3] == VIDE))) {
        counter++;
    }
    //si alignement vertical haut
    if (((partie->plateau[i+1][j] == J1) || (partie->plateau[i+1][j] == VIDE)) && ((partie->plateau[i+2][j] == J1) || (partie->plateau[i+2][j] == VIDE)) && ((partie->plateau[i+3][j] == J1) || (partie->plateau[i+3][j] == VIDE))) {
        counter++;
    }
    //si alignement vertical bas
    if (((partie->plateau[i-1][j] == J1) || (partie->plateau[i-1][j] == VIDE)) && ((partie->plateau[i-2][j] == J1) || (partie->plateau[i-2][j] == VIDE)) && ((partie->plateau[i-3][j] == J1) || (partie->plateau[i-3][j] == VIDE))) {
        counter++;
    }
    //si alignement diagonale haute haute
    if (((partie->plateau[i+1][j+1] == J1) || (partie->plateau[i+1][j+1] == VIDE)) && ((partie->plateau[i+2][j+2] == J1) || (partie->plateau[i+2][j+2] == VIDE)) && ((partie->plateau[i+3][j+3] == J1) || (partie->plateau[i+3][j+3] == VIDE))) {
        counter++;
    }
    //si alignement diagonale basse
    if (((partie->plateau[i+1][j-1] == J1) || (partie->plateau[i+1][j-1] == VIDE)) && ((partie->plateau[i+2][j-2] == J1) || (partie->plateau[i+2][j-2] == VIDE)) && ((partie->plateau[i+3][j-3] == J1) || (partie->plateau[i+3][j-3] == VIDE))) {
        counter++;
    }
    return counter;
}

// vérifie si IA peut gagner
int checkVictoireIA(Partie* partie, int i, int j) {
    //si alignement horizontal droite
    if ((partie->plateau[i][j+1] == J2) && (partie->plateau[i][j+2] == J2) && (partie->plateau[i][j+3] == J2)) {
        return 1;
    }
    //si alignement horizontal gauche
    if ((partie->plateau[i][j-1] == J2) && (partie->plateau[i][j-2] == J2) && (partie->plateau[i][j-3] == J2)) {
        return 1;
    }
    //si alignement vertical haut
    if ((partie->plateau[i+1][j] == J2) && (partie->plateau[i+2][j] == J2) && (partie->plateau[i+3][j] == J2)) {
        return 1;
    }
    //si alignement vertical bas
    if ((partie->plateau[i-1][j] == J2) && (partie->plateau[i-2][j] == J2) && (partie->plateau[i-3][j] == J2)) {
        return 1;
    }
    //si alignement diagonale haute haute
    if ((partie->plateau[i-1][j-1] == J2) && (partie->plateau[i-2][j-2] == J2) && (partie->plateau[i-3][j-3] == J2)) {
        return 1;
    }
    //si alignement diagonale basse
    if ((partie->plateau[i+1][j+1] == J2) && (partie->plateau[i+2][j+2] == J2) && (partie->plateau[i+3][j+3] == J2)) {
        return 1;
    }
    if ((partie->plateau[i-1][j+1] == J2) && (partie->plateau[i-2][j+2] == J2) && (partie->plateau[i-3][j+3] == J2)) {
        return 1;
    }
    if ((partie->plateau[i+1][j-1] == J2) && (partie->plateau[i+2][j-2] == J2) && (partie->plateau[i+3][j-3] == J2)) {
        return 1;
    }
    return 0;
}

// vérifie si J1 peut gagner
int checkVictoireJ1(Partie* partie, int i, int j) {
    //si alignement horizontal droite
    if ((partie->plateau[i][j+1] == J1) && (partie->plateau[i][j+2] == J1) && (partie->plateau[i][j+3] == J1)) {
        return 1;
    }
    //si alignement horizontal gauche
    if ((partie->plateau[i][j-1] == J1) && (partie->plateau[i][j-2] == J1) && (partie->plateau[i][j-3] == J1)) {
        return 1;
    }
    //si alignement vertical haut
    if ((partie->plateau[i+1][j] == J1) && (partie->plateau[i+2][j] == J1) && (partie->plateau[i+3][j] == J1)) {
        return 1;
    }
    //si alignement vertical bas
    if ((partie->plateau[i-1][j] == J1) && (partie->plateau[i-2][j] == J1) && (partie->plateau[i-3][j] == J1)) {
        return 1;
    }
    //si alignement diagonale haute haute
    if ((partie->plateau[i-1][j-1] == J1) && (partie->plateau[i-2][j-2] == J1) && (partie->plateau[i-3][j-3] == J1)) {
        return 1;
    }
    //si alignement diagonale basse
    if ((partie->plateau[i+1][j+1] == J1) && (partie->plateau[i+2][j+2] == J1) && (partie->plateau[i+3][j+3] == J1)) {
        return 1;
    }
    if ((partie->plateau[i-1][j+1] == J1) && (partie->plateau[i-2][j+2] == J1) && (partie->plateau[i-3][j+3] == J1)) {
        return 1;
    }
    if ((partie->plateau[i+1][j-1] == J1) && (partie->plateau[i+2][j-2] == J1) && (partie->plateau[i+3][j-3] == J1)) {
        return 1;
    }
    return 0;
}

int evaluationCase(Partie* partie, int ligne, int colonne) {
    return 0;
}

// retourne la case ayant la plus haute valeur de la liste des cases évaluées
int trieValeurCase(int *valeurCase) {
    int choixCase = valeurCase[0];
    int meilleureColonne = 0;

    for (int i = 0; i < 7; i++) {
        if (choixCase < valeurCase[i]) {
            choixCase = valeurCase[i];
            meilleureColonne = i;
        }
    }
    return meilleureColonne;
}

// évalue la valeur des cases disponibles pour chacune des colonnes
int evaluation(Partie* partie) {
    int choixCase = 0;
    int *valeurCase = (int*)calloc(7, sizeof(int)); // alloue la mémoire pour la table des valeurs des colonnes
    int counterValeurCase = 0;

    // se positionne dans le première case vide de la colonne
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 5; ++j) {
            if (partie->plateau[j][i] != VIDE) {
                // IA définit la valeur de la case disponible dans une colonne en fonction des possibilités d'alignements de 4 pions pour elle et J1
                valeurCase[counterValeurCase] = AlignementIA(partie, i, j);

                // IA définit la colonne qui bloque la vitoire de J1
                if (checkVictoireJ1(partie, i, j) == 1) {
                    //printf("\nJE TE BLOQUE !\n");
                    valeurCase[counterValeurCase] = 900;
                    break;
                }
                // IA définit la colonne qui gagne
                if (checkVictoireIA(partie, i, j) == 1) {
                    //printf("\nJE GAGNE !\n");
                    valeurCase[counterValeurCase] = 1000;
                    break;
                }
                counterValeurCase++;

                /*for (int i = 0; i <= counterValeurCase; i++) {
                    printf("\nValeur tableau : %d    et le counter : %d\n", valeurCase[counterValeurCase], counterValeurCase);
                }
                 */

                break;
            }
        }
    }
    choixCase = trieValeurCase(valeurCase); // trie les valeurs des colonnes
    valeurCase = NULL;
    free(valeurCase); // libère la mémoire pour la table des valeurs des colonnes
    return choixCase; // renvoie la colonne ayant la plus haute valeur
}

Arbre* minmax(Partie* partie, int profondeur, int IA) {
    return NULL;
}

void detruireArbre(Arbre* arbre) {

}