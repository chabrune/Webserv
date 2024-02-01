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
#include "Location.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>

class Server
{
    public:
    void inputParsing(std::string argv);
    void check_serv_line(std::string &line, int& one_line);
    void check_serv_name_line(std::string& line, int& one_line);
    Server();
    ~Server();
    Server& operator=(const Server &rhs);
    Server(const Server &src);
    int sock_fd;    // contain the listener socket fd
    sockaddr_in address;    // contain address and the port
    std::map<int, std::string>  server_names;    // contain the server_name
    std::vector<std::string> indexes;    // contain indexes (if several index given, we try to load in the given order)
    std::map<int, std::string> errors_pages; // contain errors page paths
    unsigned long max_body_size;  // HTTP request's body max size (usefull for PUT and POST)
    std::string root; // Define the root directory
    std::vector<Location> locations; // Contain all the locations that are in an other class
    bool autoindex; // Set on/off autoindex (directory listing page. If an index is given and founded, the index is returned, otherwise the directory listing page appear to replace the index)
};
