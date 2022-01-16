//
// Created by Adam HAISSOUBI-VOGIER and Jerome LONGUEBRAY on 13/01/2022.
//

#ifndef BASECODE_CLIENT_H
#define BASECODE_CLIENT_H
#include "Jeu.h"

int init_peer_to__peer(Partie partie);
void envoyer();
void *receive_thread(void *server_fd);
void recevoir(int server_fd);
int getIPAddr();
int setPartie(Partie partie);
int checkEtatPartie(Partie partie, int sock);
void checkEtatConnection(int sock);
#endif //BASECODE_CLIENT_H
