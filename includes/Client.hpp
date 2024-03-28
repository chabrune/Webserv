
#pragma once

#include "Request.hpp"
#include "Response/AResponse.hpp"
#include "Server.hpp"
#include "incs.hpp"
#include "Response/Post.hpp"

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
        AResponse response;
        Post p;
        Request request;

        void readRequest();
        void sendResponse();

        bool readyToSend;
        bool headerSent;
        bool sent;
        long long contentSent;
};

std::ostream & operator<<(std::ostream & out, const Client & cli);
