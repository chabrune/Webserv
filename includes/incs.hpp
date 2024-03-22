
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
#include <cstdio>
#include "MimeUtils.hpp"

# ifndef DEBUG
#  define DEBUG 0 // false by default, automatically set to true with "make debug"
# endif

# define HTTP_BUFFER_SIZE 4096
# define SND_BUFFER_SIZE 16384
# define CONNECTIONS_NUMBER_LIMIT 2000
# define TIMEOUT 1 // Seconds before select exit

class   requestError : public std::exception {};
class   taMereEnSlip : public std::exception {};

extern int g_error;

# define ISDIRECTORY 1
# define NOTALLOWEDMETHOD 2
# define MISSINGSLASH 3
# define INVALIDSLASH 4
# define TOOLONGREQUEST 5
# define BADHEADER 6
# define DIRNOTEMPTY 7
# define NOTFOUND 8
# define FORBIDDEN 9
# define CONFLICT 10

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

class defaultErrorCodes {
public:
    std::map<unsigned int, std::string> errorCodes;
    defaultErrorCodes() {
        errorCodes[100] = "Continue";
        errorCodes[101] = "Switching Protocols";
        errorCodes[200] = "OK";
        errorCodes[201] = "Created";
        errorCodes[202] = "Accepted";
        errorCodes[203] = "Non-Authoritative Information";
        errorCodes[204] = "No Content";
        errorCodes[205] = "Reset Content";
        errorCodes[206] = "Partial Content";
        errorCodes[300] = "Multiple Choices";
        errorCodes[301] = "Moved Permanently";
        errorCodes[302] = "Found";
        errorCodes[303] = "See Other";
        errorCodes[304] = "Not Modified";
        errorCodes[305] = "Use Proxy";
        errorCodes[307] = "Temporary Redirect";
        errorCodes[400] = "Bad Request";
        errorCodes[401] = "Unauthorized";
        errorCodes[402] = "Payment Required";
        errorCodes[403] = "Forbidden";
        errorCodes[404] = "Not Found";
        errorCodes[405] = "Method Not Allowed";
        errorCodes[406] = "Not Acceptable";
        errorCodes[407] = "Proxy Authentication Required";
        errorCodes[408] = "Request Timeout";
        errorCodes[409] = "Conflict";
        errorCodes[410] = "Gone";
        errorCodes[411] = "Length Required";
        errorCodes[412] = "Precondition Failed";
        errorCodes[413] = "Payload Too Large";
        errorCodes[414] = "URI Too Long";
        errorCodes[415] = "Unsupported Media Type";
        errorCodes[416] = "Range Not Satisfiable";
        errorCodes[417] = "Expectation Failed";
        errorCodes[418] = "I'm a teapot";
        errorCodes[421] = "Misdirected Request";
        errorCodes[422] = "Unprocessable Entity";
        errorCodes[423] = "Locked";
        errorCodes[424] = "Failed Dependency";
        errorCodes[426] = "Upgrade Required";
        errorCodes[428] = "Precondition Required";
        errorCodes[429] = "Too Many Requests";
        errorCodes[431] = "Request Header Fields Too Large";
        errorCodes[500] = "Internal Server Error";
        errorCodes[501] = "Not Implemented";
        errorCodes[502] = "Bad Gateway";
        errorCodes[503] = "Service Unavailable";
        errorCodes[504] = "Gateway Timeout";
        errorCodes[505] = "HTTP Version Not Supported";
        errorCodes[506] = "Variant Also Negotiates";
        errorCodes[507] = "Insufficient Storage";
        errorCodes[508] = "Loop Detected";
        errorCodes[510] = "Not Extended";
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
            return (this->errorCodes[index]);
        } catch (std::exception &e) {
            throw indexOutOfBounds();
        }
    }
};

extern defaultErrorCodes __defaultErrorCodes;


# define RESET	"\033[0m"
# define RED 	"\033[31m"
# define GREEN  "\033[32m"
# define BLUE   "\033[34m"
# define YELLOW	"\033[33m"
# define MAGENTA "\033[35m"
# define BOLD   "\033[1m"
# define RESET_BOLD "\033[21m"
