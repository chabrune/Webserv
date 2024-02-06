#pragma once

#include "ServerConf.hpp"

class Server : public ServerConf
{
    public:
        Server();
        ~Server();
        Server(const Server &src);
        Server& operator=(const Server &rhs);

        void setup();

        sockaddr_in srvaddress; // Server address
        int sockfd;

        class socketCreationError : public std::exception {
            const char * what() const throw() {
                return ("socket creation failed");
            }
        };
        class socketConfigError : public std::exception {
            const char * what() const throw() {
                return ("socket configuration failed");
            }
        };
        class addrBindError : public std::exception {
            const char * what() const throw() {
                return ("socket binding failed");
            }
        };
        class sockListeningError : public std::exception {
            const char * what() const throw() {
                return ("socket listening failed");
            }
        };
};
