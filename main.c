#include "Jeu.h"
#include "IA.h"
#include <stdlib.h>
#include <stdio.h>
#include "Multijoueur/Client.h"

int main(int argc, char** argv){
    Partie partie;
    int choix_menu_principal = menu(&partie);
    if(choix_menu_principal == 0){
        int tour = choixPremierJoueur();
        do{
            partie.tour = tour;
            for (int i=0; i<6; i++){
                for (int j=0; j<7; j++){
                    partie.plateau[i][j] = 0;
                }
            }
            tour = 2 - tour + 1;
        } while(bouclePrincipale(&partie));

    }else{
        init_peer_to__peer();
    }

    return 0;
}