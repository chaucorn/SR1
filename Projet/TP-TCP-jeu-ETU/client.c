/* =================================================================== */
// Progrmame Client à destination d'un joueur qui doit deviner la case
// du trésor. Après chaque coup le résultat retourné par le serveur est
// affiché. Le coup consite en une abcsisse et une ordonnée (x, y).
/* =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define N 10
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define SERVER_IP "146.59.237.169"
#define SERVER_LOCAL "127.0.0.1"
#define PORT 5555
/* ====================================================================== */
/*                  Affichage du jeu en mode texte brut                   */
/* ====================================================================== */
void afficher_jeu(int jeu[N][N], int res, int points, int coups) {

    printf("\n************ TROUVEZ LE TRESOR ! ************\n");
    printf("    ");
    for (int i=0; i<10; i++)
        printf("  %d ", i+1);
    printf("\n    -----------------------------------------\n");
    for (int i=0; i<10; i++){
        printf("%2d  ", i+1);
        for (int j=0; j<10; j++) {
            printf("|");
            switch (jeu[i][j]) {
                case -1:
                    printf(" 0 ");
                    break;
                case 0:
                    printf(GREEN " T " RESET);
                    break;
                case 1:
                    printf(YELLOW " %d " RESET, jeu[i][j]);
                    break;
                case 2:
                    printf(RED " %d " RESET, jeu[i][j]);
                    break;
                case 3:
                    printf(MAGENTA " %d " RESET, jeu[i][j]);
                    break;
            }
        }
        printf("|\n");
    }
    printf("    -----------------------------------------\n");
    printf("Pts dernier coup %d | Pts total %d | Nb coups %d\n", res, points, coups);
}


/* ====================================================================== */
/*                    Fonction principale                                 */
/* ====================================================================== */
int main(int argc, char **argv) {

    int jeu[N][N];
    int lig, col;
    int res = -1, points = 0, coups = 0;

    /* Init args */
    // TODO. @IP et numéro de port en paramètres du programme
    int client_socket;
    struct sockaddr_in server_addr; // structure to prepare for server address
    
    

    /* Init jeu */
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            jeu[i][j] = -1;

    /* Creation socket TCP */
    // TODO
    client_socket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol (TCP)
    if (client_socket < 0) {
        perror("socket creation failed");
        return -1;
    }  
    //printf ("Socket created successfully\n");

    /* Init caracteristiques serveur distant (struct sockaddr_in) */
    // TODO
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT); // Port du serveur
    server_addr.sin_addr.s_addr = inet_addr(SERVER_LOCAL); // Adresse IP du serveur


    /* Etablissement connexion TCP avec process serveur distant */
    // TODO
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return -1;
    }


    /* Tentatives du joueur : stoppe quand tresor trouvé */
    do {
        afficher_jeu(jeu, res, points, coups);
        printf("\nEntrer le numéro de ligne : ");
        scanf("%d", &lig);
        printf("Entrer le numéro de colonne : ");
        scanf("%d", &col);

        /* Construction requête (serialisation en chaines de caractères) */
        // TODO
        char requete[100]; // Buffer pour la requête
        snprintf(requete, sizeof(requete), "%d %d\n", lig, col); // Formatage de la requête
        /* Envoi de la requête au serveur (send) */
        // TODO
        if (send(client_socket, requete, strlen(requete), 0) < 0) {
            perror("send failed");
            close(client_socket);
            return -1;
        }
        /* Réception du resultat du coup (recv) */
        // TODO
        char buffer[100]; // Buffer pour la réponse
        memset(buffer, 0, sizeof(buffer)); // Initialisation du buffer
        /* Deserialisation du résultat en un entier */
        // TODO
        if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
            perror("recv failed");
            close(client_socket);
            return -1;
        }
        res = atoi(buffer); // Conversion de la chaîne de caractères en entier
        

        /* Mise à jour */
        if (lig>=1 && lig<=N && col>=1 && col<=N)
            jeu[lig-1][col-1] = res;
        points += res;
        coups++;

    } while (res);

    /* Fermeture connexion TCP */
    // TODO
    close(client_socket);
    /* Terminaison du jeu : le joueur a trouvé le tresor */
    afficher_jeu(jeu, res, points, coups);
    printf("\nBRAVO : trésor trouvé en %d essai(s) avec %d point(s)"
            " au total !\n\n", coups, points);
    return 0;
}
