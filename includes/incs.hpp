
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
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <sys/stat.h>
#include <cerrno>
#include <dirent.h>
#include "MimeUtils.hpp"

# ifndef DEBUG
#  define DEBUG 0 // false by default, automatically set to true with "make debug"
# endif

# define HTTP_BUFFER_SIZE 4096
# define CONNECTIONS_NUMBER_LIMIT 2000
# define TIMEOUT 1 // Seconds before select exit
# define ISDIRECTORY 500

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

# define RESET	"\033[0m"
# define RED 	"\033[31m"
# define GREEN  "\033[32m"
# define BLUE   "\033[34m"
# define YELLOW	"\033[33m"
# define MAGENTA "\033[35m"
# define BOLD   "\033[1m"
# define RESET_BOLD "\033[21m"
