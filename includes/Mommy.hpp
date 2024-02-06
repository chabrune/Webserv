#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <sys/time.h>

#include "Server.hpp"

# define RESET	"\033[0m"
# define RED 	"\033[31m"
# define GREEN  "\033[32m"
# define BLUE   "\033[34m"
# define YELLOW	"\033[33m"
# define MAGENTA "\033[35m"

# define CONNECTIONS_NUMBER_LIMIT 2000
# define TIMEOUT 10 // Seconds before timeout

class Mommy
{
    public:
        fd_set lset;    // Listener sockets set
        fd_set cset;    // Client sockets set
        std::vector<Server *> servers;
        timeval timeout;

        void run(void);
        int fillFdSet(void);

        class selectError : public std::exception {
            const char * what() const throw() {
                return ("select() failure");
            }
        };
};