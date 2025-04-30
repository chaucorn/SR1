/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version ITERATIVE : 1 seul client/joueur à la fois
/* =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5555

/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */
int main(int argc, char **argv) {

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    memset(&server_addr, 0, sizeof(server_addr)); // initialise a clean structure
    memset(&client_addr, 0, sizeof(client_addr)); // initialise a clean structure
    char buffer[100];

    /*Step 1 : Create the server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket creation failed");
        return -1;
    }

    /*Step 2 : Prepare the server address structure */
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT); // Server port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces


    /* Step 3: Bind the socket to the IP address and port*/
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_socket);
        return -1;
    }


    /* Step 4: Listen for incoming connections */

    if (listen(server_socket, 5) < 0) { // 5 is the max number of queued connections (backlog)
        perror("listen failed");
        close(server_socket);
        return -1;
    }
    printf("Server's listening on port %d\n", PORT);
    
    /* Step 5: Accept a client's connection*/

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        perror("accept failed");
        close(server_socket);
        return -1;
    }
    printf("Client connected\n");

    /* Step 6: Receive data from the client */
    while (1) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("recv failed or client disconnected");
            break;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        // Echo the received message back to the client
        printf("%s\n", buffer);
        if (send(client_socket, buffer, bytes_received, 0) < 0) {
            perror("Send failed");
            break;
        }
    }
    return 0;
} // end main
