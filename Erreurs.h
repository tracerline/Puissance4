//
// Created by Adam HAISSOUBI-VOGIER and Jerome LONGUEBRAY on 13/01/2022.
//

#ifndef BASECODE_ERREURS_H
#define BASECODE_ERREURS_H
#include "Jeu.h"

int checkErrorInput(int input);
int checkErrorFullColonne(Partie* partie, int colonne);
int checkFullMap(Partie* partie);

#endif //BASECODE_ERREURS_H
