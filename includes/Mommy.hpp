#pragma once

#include "Server.hpp"
#include "Client.hpp"

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
        bool canDel;

        std::vector<int> toDelete;

        void run(void);
        int load_LFdSet(void); // Loading listeners socks fd's
        Client * acceptRequest(int fd, Server *server);

        class selectError : public std::exception {
            const char * what() const throw() {
                return ("select() failure");
            }
        };
};
