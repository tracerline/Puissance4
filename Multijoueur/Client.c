#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Client.h"
#include "Jeu.h"

char nom[20];
int PORT = 11000;

/* Initialisation de la connexion paire à paire
 * Elle permet à chaque joueur d'être client et serveur
 * Ainsi, cette fonction crée un socket serveur pour écouter et accepter les connexions distantes
 * Le PORT sera par défaut le 8888 et tout est fait en IPV4
 * NOTE: Tout se fera en local ; si le joueur n'est pas dans la même LAN il n'y aura pas connexion !
 */
int init_peer_to__peer() {

    int serveur_fd, new_socket, valread;
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
    address.sin_port = htons(PORT);

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
    printf("\n*****At any point in time press the following:*****\n1.Send message\n0.Quit\n");
    printf("\nEnter choice:");
    do {

        scanf("%d", &ch);
        switch (ch) {
            case 1:
                envoyer();
                break;
            case 0:
                printf("\nFermeture de la connexion...\n");
                break;
            default:
                printf("\nL'option choisie est inexistante\n");
        }
    } while (ch);

    close(serveur_fd);

    return 0;
}
void recevoir(int server_fd) {
    struct sockaddr_in address;
    int valread;
    char buffer[2000] = {0};
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
                    }
                    FD_SET(client_socket, &current_sockets);
                } else {
                    valread = recv(i, buffer, sizeof(buffer), 0);
                    printf("\n%s\n", buffer);
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

    char buffer[2000] = {0};
    //Fetching port number
    int PORT_server = 11000;

    //IN PEER WE TRUST
    //printf("Entrer le port du joueur distant:"); //Considering each peer will enter different port
    //scanf("%d", &PORT_server);

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char test[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Création de la connexion par socket échouée \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY always gives an IP of 0.0.0.0
    serv_addr.sin_port = htons(PORT_server);

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnexion avec le joueur distant échouée \n");
        return;
    }

    char message;
    printf("Message de test pour connexion socket :");
    scanf("%c", &message);
    scanf("%[^\n]s", test);
    sprintf(buffer, "%s[PORT:%d] says: %s", nom, PORT, test);
    send(sock, buffer, sizeof(buffer), 0);
    printf("\nMessage envoyé\n");
    close(sock);
}

//On envoie des datas toutes les 3secondes
void *receive_thread(void *server_fd) {
    int s_fd = *((int *) server_fd);
    while (1) {
        sleep(3);
        recevoir(s_fd);
    }
}

