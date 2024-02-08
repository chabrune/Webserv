#pragma once

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Server;

class Mommy
{
    public:
        Mommy();
        ~Mommy();

        fd_set lset;    // Listener sockets set
        fd_set cset;    // Client sockets set
        std::vector<Server *> servers;
        std::vector<Client *> clients;
        timeval timeout;

        void run(void);
        int load_LFdSet(void); // Loading listeners socks fd's
        void acceptRequest(int fd, Server *server);
        void readRequest(Client *client);

        class selectError : public std::exception {
            const char * what() const throw() {
                return ("select() failure");
            }
        };
        class acceptError : public std::exception {
            const char * what() const throw() {
                return ("accept() failure");
            }
        };
};