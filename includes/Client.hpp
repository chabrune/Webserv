
#pragma once

#include "incs.hpp"

class Client {
    public:
        Client(int fd, sockaddr_in addr);
        ~Client();

        int sockfd;
        sockaddr_in addr;
};