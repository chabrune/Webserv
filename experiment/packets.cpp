#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <fstream>
#include <cstdio>
#include <vector>
#include <exception>

#define PORT 8080
#define BACKLOG 100
#define BUFFER_SIZE 1000000

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        return 1;
    }

    // Configuration du socket pour réutiliser l'adresse et le port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        std::cerr << "Erreur lors de la configuration du socket." << std::endl;
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Liaison du socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Erreur lors de la liaison du socket." << std::endl;
        return 1;
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, BACKLOG) == -1) {
        std::cerr << "Erreur lors de l'écoute du socket." << std::endl;
        return 1;
    }

    while (1) {
        // Acceptation d'une nouvelle connexion
        new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen);
        if (new_socket == -1) {
            std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
            continue;
        }

        std::cout << "Client connecté: " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

        // Lecture de la requête HTTP (non traitée dans cet exemple)

        std::ifstream file("video3.mp4", std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier vidéo." << std::endl;
            close(new_socket);
            continue;
        }

        long long size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::cout << "Fichier ouvert, taille: " << size << " octets." << std::endl;

        // Envoi de l'en-tête HTTP
        std::string header = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(size) + "\nContent-Type: video/mp4\n\n";
        std::cout << header << std::endl;
        if (send(new_socket, header.c_str(), header.size(), 0) == -1) {
            std::cerr << "Erreur lors de l'envoi de l'en-tête HTTP." << std::endl;
        }

        // Envoi du contenu du fichier
        long long bytesSent = 0;
        char buffer[BUFFER_SIZE];
        std::cout << "starting to send" << std::endl;

        long long sent = 0;
        while (sent < size) {
            int bytesRead = std::min(static_cast<long long>(BUFFER_SIZE), size - sent);
            char buffer[bytesRead];
            file.read(buffer, bytesRead);
            if (send(new_socket, buffer, bytesRead, 0) == -1) {
                perror("");
            }
            sent += bytesRead;
        }
        if (file.eof())
            std::cout << "eof reached" << std::endl;
        else
            perror("error: ");
        std::string end = "\r\n\r\n";
        send(new_socket, &end[0], end.size(), 0);
        std::cout << "Fichier envoyé. Taille: " << sent << std::endl;

        file.close();
        close(new_socket);
    }

    close(server_fd);
    return 0;
}