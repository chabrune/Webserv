#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    std::string hostname = "www.ngalzand.bonjour.com"; // Remplacer par le nom d'hôte désiré
    std::string service = "https"; // Remplacer par le service désiré (ex. "http", "ftp")

    // Configuration des hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET ou AF_INET6 pour forcer la version
    hints.ai_socktype = SOCK_STREAM;

    // Obtention des informations de l'adresse
    if ((status = getaddrinfo(hostname.c_str(), service.c_str(), &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 2;
    }

    std::cout << "IP addresses for " << hostname << ": " << std::endl;

    // Parcourir la liste des adresses retournées
    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;
        std::string ipver;

        // Obtenir l'adresse IP
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convertir l'adresse IP en chaîne de caractères
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "  " << ipver << ": " << ipstr << std::endl;
    }

    freeaddrinfo(res); // Libérer la mémoire

    return 0;
}
