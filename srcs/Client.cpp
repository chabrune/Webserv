
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server) { }

Client::~Client() {}