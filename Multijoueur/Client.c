//
// Created by Adam HAISSOUBI-VOGIER on 13/01/2022.
//

// Connexion paire à paire (peer-to-peer connection)

#ifdef _WINDOWS_
    #include <process.h>
#else
    #include "Client.h"
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <printf.h>
    #include <string.h>
#endif

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serveur;
    char message[1000], reponse_serveur[2000];

    // On initialise le socket Client
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        printf("Impossible d'initialiser le socket client...");
    }

    puts("Socket ajouté"); // on avertit le client
    // on définit les infos du serveur distant qui est aussi client
    serveur.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(9999);

    if (connect(sock , (struct sockaddr *)&serveur , sizeof(serveur)) < 0)
    {
        perror("Connexion impossible avec le serveur");
        return 1;
    }
    puts("Connecté au serveur !\n");

    while(1){
        // On envoie un message de test
        printf("Message de test à envoyer : ");
        fgets(message, sizeof(message),stdin);

        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("L'envoi du message a échoué");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , reponse_serveur , 2000 , 0) < 0)
        {
            puts("Erreur lors de la reception des données...");
            break;
        }

        printf("Server Reply: %s\n", reponse_serveur);
        reponse_serveur[0]='\0';
    }
}
