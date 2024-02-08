
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server) {
    std::cout << "Client server port: " << (int)ntohs(server->srvaddress.sin_port) << std::endl;
}

Client::~Client() {}