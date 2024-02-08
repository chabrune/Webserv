
#pragma once

#include "incs.hpp"
#include "Server.hpp"

class Client {
    public:
        Client(int fd, sockaddr_in addr, Server *server);
        ~Client();

        Server *server; // Attached server
        int sockfd;
        sockaddr_in addr;
        long requestLen; // Length of the request
        char buff[HTTP_BUFFER_SIZE]; // buff
};