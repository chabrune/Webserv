#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server() {}

Server& Server::operator=(const Server &rhs)
{
    (void)rhs;
    return(*this);
}

Server::Server(const Server &src)
{
    (void)src;
}

void Server::setup(void)
{
    memset(&this->srvaddress, 0, sizeof(this->srvaddress));
    this->srvaddress.sin_family = AF_INET;
    this->srvaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->srvaddress.sin_port = htons(this->port);
    this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw socketCreationError();
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(int)) == -1)
        throw socketConfigError();
    this->timeout.tv_sec = TIMEOUT;
    this->timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketConfigError();
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketConfigError();
    if (bind(this->sockfd, (struct sockaddr *)&this->srvaddress, sizeof(this->srvaddress)) == -1)
        throw addrBindError();
    if (listen(this->sockfd, CONNECTIONS_NUMBER_LIMIT) == -1)
        throw sockListeningError();
    std::cout << GREEN << "-listener socket for " << this->server_name << ":" << this->port << " ready" << RESET << std::endl;
}
