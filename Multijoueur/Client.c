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
#include "Erreurs.h"

char nom[20];
// variable globale : Port d'écoute pour établir la connexion
int PORT = 11006;
char* IPbuffer;
//#pragma pack(1)
Partie jeu;
//#pragma pack(0)
int coups[200];
int MODE_DE_JEU_MULTI = -1;
bool EN_JEU = false;
// Adresse du destinataire
char RAdresse[sizeof(char) * 100];

/* Initialisation de la connexion paire à paire
 * Elle permet à chaque joueur d'être client et serveur
 * Ainsi, cette fonction crée un socket serveur pour écouter et accepter les connexions distantes
 * Le PORT sera par défaut le 8888 et tout est fait en IPV4
 * NOTE: Tout se fera en local ; si le joueur n'est pas dans la même LAN il n'y aura pas connexion !
 */
int init_peer_to__peer(Partie partie) {
    // On créé une partie locale --> afficher les coups de J1 et J2
    setPartie(partie);
    // on récupère l'IP de la machine runnant le script (vous)
    getIPAddr();
    int ok = 1;
    int serveur_fd, new_socket, valread;
    struct sockaddr_in address;
    int k = 0;

    // paramètrage de la structure socket (addr INET, etc...)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // on créé le socket serveur
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

    // on affiche l'@ip et le port ==> debug
    printf("IP: %s\n", inet_ntoa(address.sin_addr));
    printf("Port: %d\n", (int) ntohs(address.sin_port));
    // on bind le socket
    if (bind(serveur_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Impossible de bind le socket créé");
        exit(EXIT_FAILURE);
    }
    // on écoute le socket pour une connexion
    if (listen(serveur_fd, 5) < 0) {
        perror("Erreur: écoute de la socket impossible");
        exit(EXIT_FAILURE);
    }
    int ch;
    // On créé un système de threads pour les sockets
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread,
                   &serveur_fd); //Creating thread to keep receiving message in real time
    printf("\n***** Mode Multijoueur (LAN uniquement) *****\n1.Héberger\n2.Rejoindre\n");
    printf("\nVotre choix:");

    do {
    // Tant que c'est vrai
        scanf("%d", &ch);
        MODE_DE_JEU_MULTI = ch;
        // on récupère le choi du mode de multijoueur : héberger/rejoindre
        switch (ch) {
            // Cas 1 : Héberger
            case 1:
                printf("En attente d'un deuxième joueur...");
                sleep(3); // On attend 3sec
                // On écoute le port pour une connexion
                recevoir(serveur_fd);
                break;
            // Cas 2 : Rejoindre une partie hébergée
            case 2:
                printf("IP du joueur J2 : ");
                // On demande l'@ip de la machine distante
                scanf("%s", RAdresse);
                // On envoie les données
                envoyer();
                break;
            // Sinon: on demande de rejoindre une partie
            default:
                envoyer();
                break;
        }
    } while (ch);
    // Fermeture de la connexion principale
    close(serveur_fd);
    return 0;
}

// On reçoit les données envoyées par la machine source, et on les affiche
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
            perror("Erreur lors de la sélection du socket...");
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
                        printf("\nJ2 a rejoint la partie !");
                        FD_SET(client_socket, &current_sockets);
                    }

                } else {
                    checkEtatConnection(server_fd);
                    //printf("Lecture des données reçues...");
                    recv(i, &buffer, sizeof(int), 0);
                    jeu.tour = 1;
                    jouerCoup(&jeu, buffer-1);
                    afficher(&jeu);
                    printf("\nJ2: colonne %d", buffer);

                    //envoyer();
                    int message;
                    printf("\nColonne :");
                    scanf("%d", &message);
                    //scanf("%d", &test);
                    //sprintf(buffer, "%s[PORT:%d] says: %c", nom, PORT, message);
                    int errorEntier = checkErrorInput(message);
                    while (errorEntier == 1) {
                        printf("Vous (J%d): choisissez un nombre entier entre 1 et 7 : ", jeu.tour);
                        scanf("%d", &message); // récupère la colonne voulue
                        errorEntier = checkErrorInput(message);
                    }

                    // vérifie si une colonne est pleine
                    int errorColonne = checkErrorFullColonne(&jeu, message);
                    while (errorColonne == 1) {
                        printf("Joueur %d, cette colonne est pleine, veuillez choisir une autre colonne : ", jeu.tour);
                        scanf("%d", &message); // récupère la colonne voulue
                        errorColonne = checkErrorFullColonne(&jeu, message);
                    }

                    send(i, &message, sizeof(int), 0);
                    jeu.tour = 2;
                    jouerCoup(&jeu, message-1);
                    int etat =  checkEtatPartie(jeu, server_fd);
                    if(etat != -1){
                        shutdown(server_fd, 2);
                        close(server_fd);
                    }
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

// Envoie des messages via les sockets à l'hôte distant
void envoyer() {
    EN_JEU = true;
    char buffer[2000] = {0};
    // on assigne le port d'écoute
    int PORT_server = PORT;
    int ok = 1;
    int rep;

    int sock, valread;
    struct sockaddr_in serv_addr;
    int test;


    while(EN_JEU){
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Création de la connexion par socket échouée \n");
            return;
        }
        checkEtatConnection(sock);
        serv_addr.sin_family = AF_INET;
         //IPar défaut: 0.0.0.0
        serv_addr.sin_port = htons(PORT_server);
        inet_aton(RAdresse, &serv_addr.sin_addr);

        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnexion avec le joueur distant échouée \n");
            exit(EXIT_FAILURE);
            return;
        }
         // On check l'état de la socket de connection
        int message; // Le message à envoyer : la colonne sur laquelle on veut jouer
        printf("\nColonne :"); // on demande la colonne à jouer
        scanf("%d", &message);
        int errorEntier = checkErrorInput(message);
        while (errorEntier == 1) {
            printf("Vous (J%d): choisissez un nombre entier entre 1 et 7 : ", jeu.tour);
            scanf("%d", &message); // récupère la colonne voulue
            errorEntier = checkErrorInput(message);
        }

        // vérifie si une colonne est pleine
        int errorColonne = checkErrorFullColonne(&jeu, message);
        while (errorColonne == 1) {
            printf("Joueur %d, cette colonne est pleine, veuillez choisir une autre colonne : ", jeu.tour);
            scanf("%d", &message); // récupère la colonne voulue
            errorColonne = checkErrorFullColonne(&jeu, message);
        }
        //scanf("%d", &test);
        //sprintf(buffer, "%s[PORT:%d] says: %c", nom, PORT, message);

        send(sock, &message, sizeof(int), 0);
        jeu.tour = 2; // Tour du J2

        printf("\nVous: Colonne %d\nEn attente du coup du J2 \n", message); // On demande la colonne
        jouerCoup(&jeu, message-1);

        //afficher(&jeu);
        recv(sock, &rep, sizeof(int), 0);
        jeu.tour = 1;
        jouerCoup(&jeu, rep-1);
        int etat = checkEtatPartie(jeu, sock); // on check l'état de la partie
        if(etat != -1){
            shutdown(sock, 2); // shutdown du socket
            close(sock); // fermeture complète du socket
        }
        printf("\n");

        afficher(&jeu);
        printf("J2 a joué : Colonne %d", rep);
    }
}

//On envoie des datas toutes les 3secondes (gère les threads)
void *receive_thread(void *server_fd) {
    int s_fd = *((int *) server_fd);
    while (1) {
        sleep(3);
        recevoir(s_fd);
    }
}

// Donne l'IP locale de la machine qui run le programme (vous)
int getIPAddr(){
    char hostbuffer[256];
    struct hostent *host_entry;
    int hostname;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    host_entry = gethostbyname(hostbuffer);

    IPbuffer = inet_ntoa(*((struct in_addr*)
            host_entry->h_addr_list[0]));

    return 0;
}

int setPartie(Partie partie){
    memcpy(&partie, &jeu, sizeof(Partie));
    return 0;
}

int checkEtatPartie(Partie partie, int sock){
    if(calculerEtat(&partie) == VICTOIRE_J1){
        printf("J2 A GAGNÉ ! \n");
        exit(EXIT_SUCCESS);
        return VICTOIRE_J1;
    }
    else if(calculerEtat(&partie) == VICTOIRE_J2){
        printf("J1 A GAGNÉ ! \n");
        exit(EXIT_SUCCESS);
        return VICTOIRE_J2;
    }
    else if(calculerEtat(&partie) == EGALITE){
        printf("Fin de la partie: ÉGALITÉ\n");
        exit(EXIT_SUCCESS);
        return EGALITE;
    }
    else{
        return -1;
    }
}

void checkEtatConnection(int sock){
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (sock, SOL_SOCKET, SO_ERROR, &error, &len);

    if (retval != 0) {
        /* there was a problem getting the error code */
        fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        shutdown(sock, 3);
        close(sock);
        return;
    }

    if (error != 0) {
        /* socket has a non zero error status */
        fprintf(stderr, "socket error: %s\n", strerror(error));
        shutdown(sock, 3);
        close(sock);
        return;
    }
}


