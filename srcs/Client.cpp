
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr) : sockfd(fd), addr(addr) {}

Client::~Client() {}