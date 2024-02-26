#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const int BUFFER_SIZE = 1024;
const int PORT = 8082;

typedef int SOCKET;

int main() {

    // Create a new TCP socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "LISTEN @@@@@ = " << listen << std::endl;
    if (listener == -1) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    // Bind the socket to a local address and port
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (sockaddr*)&sin, sizeof(sin)) == -1) {
        perror("Failed to bind socket");
        close(listener);
        return EXIT_FAILURE;
    }

    // Start listening for incoming connections
    if (listen(listener, SOMAXCONN) == -1) {
        perror("Failed to start listening");
        close(listener);
        return EXIT_FAILURE;
    }

    // Wait for incoming connections
    fd_set master_fds;
    FD_ZERO(&master_fds);
    FD_SET(listener, &master_fds);


while (true) {
    fd_set read_fds = master_fds;
    if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
        perror("Erreur lors de l'attente de la sélection");
        break;
    }

    for (int i = 0; i < FD_SETSIZE; ++i) {
        if (!FD_ISSET(i, &read_fds)) continue;

        if (i == listener) {
            // Acceptez une nouvelle connexion
            SOCKET accepted_socket = accept(listener, NULL, NULL);
            if (accepted_socket != -1) {
                // Ajoutez le nouveau socket à la liste des sockets actifs
                FD_SET(accepted_socket, &master_fds);
            } else {
                perror("Impossible d'accepter la connexion");
            }
        } else {
            // Traitez les données provenant d'une connexion établie
            char buffer[BUFFER_SIZE];
            ssize_t bytes_received = recv(i, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::cout << "Requete reçue :" << std::endl << buffer << std::endl;

                // Construisez une réponse appropriée
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nBonjour, monde !";

                // Envoyez la réponse au client
                send(i, response.c_str(), response.length(), 0);
            } else if (bytes_received == 0) {
                // Le client a fermé la connexion
                close(i);
                FD_CLR(i, &master_fds);
            } else {
                // Erreur lors de la lecture des données du client
                perror("Erreur lors de la lecture à partir du socket");
            }
        }
    }
}

    // Clean up
    close(listener);

    return EXIT_SUCCESS;
}
