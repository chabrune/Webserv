#pragma once

#include "Mommy.hpp"
#include "Location.hpp"

class Server
{
    public:
        // Funcs
        void inputParsing(std::string argv);
        void check_serv_line(std::string &line, int& one_line);
        void check_serv_name_line(std::string& line, int& one_line);
        void setup_socket();
        void run();
        Server();
        ~Server();
        Server(const Server &src);

        // Vars
        bool running;
        int sock_fd;    // contain the listener socket fd
        sockaddr_in address;    // contain address and the port
        socklen_t addrlen;
        std::vector<std::string> server_names;    // contain the server_name
        std::vector<std::string> indexes;    // contain indexes (if several index given, we try to load in the given order)
        std::map<int, std::string> errors_pages; // contain errors page paths
        unsigned long max_body_size;  // HTTP request's body max size (usefull for PUT and POST)
        std::string root; // Define the root directory
        std::vector<Location> locations; // Contain all the locations that are in an other class
        bool autoindex; // Set on/off autoindex (directory listing page. If an index is given and founded, the index is returned, otherwise the directory listing page appear to replace the index)
        timeval timeout;

        // Exceptions
        class socketCreationError : public std::exception {
            const char * what() const throw() {
                return ("can't create listener socket");
            }
        };
        class addrBindError : public std::exception {
            const char * what() const throw() {
                return ("can't bind socket to address and/or port");
            }
        };
        class sockListeningError : public std::exception {
            const char * what() const throw() {
                return ("failed to start socket listening");
            }
        };

        //Operators overloads
        Server& operator=(const Server &rhs);
};
