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

#define PORT 8080
#define BACKLOG 10

std::string getcharptrline(char *ptr) {
    std::string buff;

    for (int i = 0; ptr[i] && ptr[i] != '\r'; i++) {
        buff += ptr[i];
    }
    return buff;
}

void executeCGI(const char* scriptPath, int clientSocket) {
    int pipefd[2];
    pipe(pipefd); // Crée un pipe pour la communication

    if (fork() == 0) {
        // Processus enfant
        close(pipefd[0]); // Ferme la lecture du pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirige stdout vers le pipe

        // Définir les variables d'environnement CGI ici
        // setenv("REQUEST_METHOD", "GET", 1); // Exemple

        execl("/usr/bin/python3", "python3", scriptPath, NULL);
        exit(0);
    } else {
        // Processus parent
        close(pipefd[1]); // Ferme l'écriture du pipe
        char buffer[1024];
        int bytesRead;

        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            // Envoyer la sortie du script CGI au client
            send(clientSocket, buffer, bytesRead, 0);
        }

        close(pipefd[0]);
        wait(NULL); // Attendre la fin du processus enfant
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Création du socket serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Liaison du socket à l'adresse et au port
    bind(server_fd, (struct sockaddr *) &address, sizeof(address));

    // Écoute des connexions entrantes
    listen(server_fd, BACKLOG);

    while (1) {
        // Acceptation d'une nouvelle connexion
        new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen);

        // Lecture de la requête HTTP
        read(new_socket, buffer, 1024);
        std::cout << buffer << std::endl;

        // Envoi de la réponse
        std::string line = getcharptrline(buffer);
        bool isBinary = false;
        std::string contentType = "text/html";
        std::string filePath = "404.html";

        // Détermination du fichier à servir
        if (line == "GET / HTTP/1.1" || line == "GET /index.html HTTP/1.1" || line == "GET /index HTTP/1.1")
            filePath = "index.html";
        else if (line == "GET /pouic.html HTTP/1.1" || line == "GET /pouic HTTP/1.1")
            filePath = "pouic.html";
        else if (line == "GET /img.jpg HTTP/1.1") {
            filePath = "img.jpg";
            contentType = "image/jpeg";
            isBinary = true;
        } else if (line == "GET /hello.py HTTP/1.1") {
            filePath = "hello.py";
        }
        // Ouverture et envoi du fichier
        int file_fd = open(filePath.c_str(), O_RDONLY);
        struct stat file_stat;
        fstat(file_fd, &file_stat); // Obtient la taille du fichier
        int file_size = file_stat.st_size;

        char header[1024];
        sprintf(header, "HTTP/1.1 200 OK\nContent-Length: %d\nContent-Type: %s\n\n", file_size, contentType.c_str());
        send(new_socket, header, strlen(header), 0);

        if (isBinary) {
            // Envoi de données binaires
            char file_buffer[1024];
            int bytes_read;
            while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
                send(new_socket, file_buffer, bytes_read, 0);
            }
        } else if (line == "GET /hello.py HTTP/1.1") {
            executeCGI("hello.py", new_socket);
        } else {
            // Envoi de texte
            char file_buffer[1024];
            int bytes_read = read(file_fd, file_buffer, sizeof(file_buffer) - 1);
            file_buffer[bytes_read] = '\0';
            send(new_socket, file_buffer, bytes_read, 0);
        }
        close(file_fd);

        // Fermeture de la connexion
        close(new_socket);
    }
}