#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "incs.hpp"

class Server;

class Client {
    private:
        void sendGeneratedContent();
        void sendInfileContent();
        void sendCgiContent();
    public:
        Client(int fd, sockaddr_in addr, Server *server);
        ~Client();

        int sockfd;
        sockaddr_in addr;
        Server *server; // Attached server
        Response response;
        Request request;

        void readRequest();
        void sendResponse();

        bool readyToSend;
        bool headerSent;
        bool sent;
        long long contentSent;
};

std::ostream & operator<<(std::ostream & out, const Client & cli);
