
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
#include <list>
#include <sstream>
#include <ctype.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <sys/stat.h>
#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include "MimeUtils.hpp"

# ifndef DEBUG
#  define DEBUG 0 // false by default, automatically set to true with "make debug"
# endif

# define HTTP_BUFFER_SIZE 4096
# define SND_BUFFER_SIZE 16384
# define CONNECTIONS_NUMBER_LIMIT 2000
# define TIMEOUT 1 // Seconds before select exit

// errno custom values
# define ISDIRECTORY 500
# define NOTALLOWEDMETHOD 501
# define MISSINGSLASH 502

class   requestError : public std::exception {};

enum Methods
{
    POST,
    GET,
    HEAD,
    DELETE,
    PUT,
    METHODS_END // usefull to stop an incrementation on an Enum
};

class defaultErrorPages {
private:
    std::map<unsigned int, std::string> errorPages;
public:
    defaultErrorPages() {
        errorPages[400] = "ressources/default/400.html";
        errorPages[403] = "ressources/default/403.html";
        errorPages[404] = "ressources/default/404.html";
        errorPages[405] = "ressources/default/405.html";
        errorPages[409] = "ressources/default/409.html";
        errorPages[414] = "ressources/default/414.html";
        errorPages[500] = "ressources/default/500.html";
    }
    class indexOutOfBounds : public std::exception
    {
        const char *what(void) const throw()
        {
            return ("Index out of bounds");
        }
    };
    std::string operator[](unsigned int index) {
        try {
            return (this->errorPages[index]);
        } catch (std::exception &e) {
            throw indexOutOfBounds();
        }
    }
};

extern defaultErrorPages __defaultErrorPages;

# define RESET	"\033[0m"
# define RED 	"\033[31m"
# define GREEN  "\033[32m"
# define BLUE   "\033[34m"
# define YELLOW	"\033[33m"
# define MAGENTA "\033[35m"
# define BOLD   "\033[1m"
# define RESET_BOLD "\033[21m"
