#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "Client.h"
#include "Jeu.h"
#include <stdbool.h>

char nom[20];
int PORT = 11006;
char* IPbuffer;
#pragma pack(1)
Partie jeu;
#pragma pack(0)
int coups[200];
int MODE_DE_JEU_MULTI = -1;
bool EN_JEU = false;
char RAdresse[sizeof(char) * 100];
/* Initialisation de la connexion paire à paire
 * Elle permet à chaque joueur d'être client et serveur
 * Ainsi, cette fonction crée un socket serveur pour écouter et accepter les connexions distantes
 * Le PORT sera par défaut le 8888 et tout est fait en IPV4
 * NOTE: Tout se fera en local ; si le joueur n'est pas dans la même LAN il n'y aura pas connexion !
 */

int init_peer_to__peer(Partie partie) {
    setPartie(partie);
    getIPAddr();
    int ok = 1;
    int serveur_fd, new_socket, valread;
    struct sockaddr_in address;
    int k = 0;
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    inet_aton(IPbuffer, &address.sin_addr);
    // Creating socket file descriptor
    if ((serveur_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Connexion échouée : erreur socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serveur_fd,SOL_SOCKET,SO_REUSEADDR,&ok,sizeof(int)) == -1)
    {
        perror("Setsockopt");
        pthread_exit(NULL);
        exit(1);
    }
    // Forcefully attaching socket to the port



    //Printed the server socket addr and port
    printf("IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("port is: %d\n", (int) ntohs(address.sin_port));

    if (bind(serveur_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Impossible de bind le socket créé");
        exit(EXIT_FAILURE);
    }
    if (listen(serveur_fd, 5) < 0) {
        perror("Erreur: écoute de la socket impossible");
        exit(EXIT_FAILURE);
    }
    int ch;
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread,
                   &serveur_fd); //Creating thread to keep receiving message in real time
    printf("\n***** Mode Multijoueur (LAN uniquement) *****\n1.Héberger\n0.Rejoindre\n");
    printf("\nVotre choix:");

    do {

        scanf("%d", &ch);
        MODE_DE_JEU_MULTI = ch;


        switch (ch) {
            case 1:
                printf("En attente d'un deuxième joueur...");
                sleep(3);
                recevoir(serveur_fd);
                break;
            case 0:
                printf("IP du joueur J2 : ");
                scanf("%s", RAdresse);
                envoyer();
                break;
            default:
                envoyer();
                break;
        }
    } while (ch);
    close(serveur_fd);

    return 0;
}
void recevoir(int server_fd) {
    struct sockaddr_in address;
    int valread;
    int buffer;
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1) {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {

                if (i == server_fd) {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *) &address,
                                                (socklen_t *) &addrlen)) < 0) {
                        perror("Erreur: la socket n'a pas été acceptée...");
                        exit(EXIT_FAILURE);
                    }else{

                        printf("J2 a rejoint la partie !");
                        FD_SET(client_socket, &current_sockets);
                    }

                } else {
                    //printf("Lecture des données reçues...");
                    recv(i, &buffer, sizeof(int), 0);

                    printf("\nJ2: colonne %d", buffer);
                    
                    //envoyer();
                    int message;
                    printf("\nColonne :");
                    scanf("%d", &message);
                    //scanf("%d", &test);
                    //sprintf(buffer, "%s[PORT:%d] says: %c", nom, PORT, message);
                    send(i, &message, sizeof(int), 0);
                    printf("\nVous: Colonne %d\nEn attente du coup du J2 \n", message);
                    /*if( 0 != afficher(buffer)){
                        perror("Impossible d'afficher la grille de jeu...");
                    }*/

                    //printf("\n%s\n", buffer);
                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}
// Envoie des messages via les sockets
void envoyer() {
    EN_JEU = true;
    char buffer[2000] = {0};
    //Fetching port number
    int PORT_server = PORT;
    int ok = 1;
    int rep;
    //IN PEER WE TRUST
    //printf("Entrer le port du joueur distant:"); //Considering each peer will enter different port
    //scanf("%d", &PORT_server);

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    int test;


    while(EN_JEU){
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Création de la connexion par socket échouée \n");
            return;
        }

        serv_addr.sin_family = AF_INET;
         //INADDR_ANY always gives an IP of 0.0.0.0
        serv_addr.sin_port = htons(PORT_server);
        inet_aton(RAdresse, &serv_addr.sin_addr);

        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnexion avec le joueur distant échouée \n");
            exit(EXIT_FAILURE);
            return;
        }

        int message;
        printf("\nColonne :");
        scanf("%d", &message);
        //scanf("%d", &test);
        sprintf(buffer, "%s[PORT:%d] says: %c", nom, PORT, message);
        send(sock, &message, sizeof(int), 0);
        printf("\nVous: Colonne %d\nEn attente du coup du J2 \n", message);
        jouerCoup(&jeu, message);
        afficher(&jeu);
        recv(sock, &rep, sizeof(int), 0);
        jouerCoup(&jeu, rep);
        afficher(&jeu);
        printf("J2 a joué : Colonne %d", rep);
        /*shutdown(sock, 1);
        close(sock);

        int serveur_fd, new_socket;
        struct sockaddr_in address;
        int k = 0;

        // Creating socket file descriptor
        if ((serveur_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Connexion échouée : erreur socket");
            exit(EXIT_FAILURE);
        }


        // Forcefully attaching socket to the port

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        sleep(5);
        address.sin_port = htons(PORT);
        //EN_JEU = true;
        if (bind(serveur_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("Impossible de bind le socket créé");
            exit(EXIT_FAILURE);
        }
        if (listen(sock, 5) < 0) {
            perror("Erreur: écoute de la socket impossible");
            exit(EXIT_FAILURE);
        }
        recevoir(serveur_fd);*/
    }

    // On recréé une socket d'écoute pour la réponse du joueur J2

    //recevoir(serveur_fd);

}

//On envoie des datas toutes les 3secondes
void *receive_thread(void *server_fd) {
    int s_fd = *((int *) server_fd);
    while (1) {
        sleep(3);
        recevoir(s_fd);
    }
}

int getIPAddr(){
    char hostbuffer[256];

    struct hostent *host_entry;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    //checkHostName(hostname);

    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);
    //checkHostEntry(host_entry);

    // To convert an Internet network
    // address into ASCII string
    IPbuffer = inet_ntoa(*((struct in_addr*)
            host_entry->h_addr_list[0]));

    //printf("Hostname: %s\n", hostbuffer);
    //printf("Host IP: %s", IPbuffer);

    return 0;
}

int setPartie(Partie partie){
    jeu = partie;
    return 0;
}
