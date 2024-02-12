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
        Request req;

        void readRequest();
        void sendResponse();

        class recvFailure : public std::exception {
            const char * what() const throw() {
                return ("failed to read request");
            }
        };
};