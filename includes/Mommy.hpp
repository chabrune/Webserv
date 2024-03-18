#pragma once

#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;
class requestError;

class Mommy
{
    public:
        Mommy();
        ~Mommy();

        fd_set sset;    // Servers sockets
        fd_set lset;    // Listener sockets set
        fd_set cset;    // Client sockets set
        std::vector<Server *> servers;
        std::map<int, Client *> clients;
        timeval timeout;
        bool running;

        std::vector<int> toDelete;
        void run(void);
        int load_LFdSet(void); // Loading listeners socks fd's
        Client * acceptRequest(int fd, Server *server);
        void treatRequest(Server * server, Client * cli);

        class selectError : public std::exception {
            const char * what() const throw() {
                return ("select() failure");
            }
        };
        class acceptError : public std::exception {
            const char *what() const throw() {
                return ("connexion received but failed");
            }
        };

        class badHeader : public requestError {
            const char *what() const throw() {
                return ("bad header");
            }
        };
};
