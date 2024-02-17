#pragma once

#include "Request.hpp"
#include "Server.hpp"

class Client {
    public:
        Client(int fd, sockaddr_in addr, Server *server);
        ~Client();

        int sockfd;
        sockaddr_in addr;
        Server *server; // Attached server

        void readRequest();
        void sendResponse();
};

std::ostream & operator<<(std::ostream & out, const Client & cli);
