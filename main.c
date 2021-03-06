#include "Jeu.h"
#include "IA.h"
#include <stdlib.h>
#include <stdio.h>
#include "Multijoueur/Client.h"
#include "UI/Fenetre.h"

int main(int argc, char** argv){

    Partie partie;
    //creerFenetre();
    int choix_menu_principal = menu(&partie);
    int tour = choixPremierJoueur();
    if(choix_menu_principal == 1){

        do{
            partie.tour = tour;
            for (int i=0; i<6; i++){
                for (int j=0; j<7; j++){
                    partie.plateau[i][j] = 0;
                }
            }
            tour = 2 - tour + 1;
        } while(bouclePrincipale(&partie));

    }else if(choix_menu_principal == 2){
        init_peer_to__peer(partie);
    }else{
        return EXIT_FAILURE;
    }

    return 0;
}