//
// Created by Adam HAISSOUBI-VOGIER and Jerome LONGUEBRAY on 13/01/2022.
//

#ifndef BASECODE_CLIENT_H
#define BASECODE_CLIENT_H
int init_peer_to__peer();
void envoyer();
void *receive_thread(void *server_fd);
void receiving(int server_fd);
#endif //BASECODE_CLIENT_H
