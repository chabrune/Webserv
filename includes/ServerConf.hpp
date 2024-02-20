#pragma once

#include "Location.hpp"
#include "Mommy.hpp"

class ServerConf
{
    public:
        ServerConf();
        ~ServerConf();
        ServerConf(const ServerConf &src);
        ServerConf& operator=(const ServerConf &rhs);

        void inputParsing(std::string argv, Mommy& frr);
        void check_serv_line(std::string &line, Mommy& frr);
        void check_serv_name_line(std::string& line, int& one_line);

        std::string root; // Define the root directory
        std::string server_name;    // contain the server_name
        int port; // contain ports to listen to
        std::map<Methods, bool> allowed_methods; // Set HTTP methods to true or false (methods will be an enum)
        std::vector<std::string> indexes;    // contain indexes (if several index given, we try to load in the given order)
        std::map<int, std::string> errors_pages; // contain errors page paths
        unsigned long max_body_size;  // HTTP request's body max size (usefull for PUT and POST)
        std::vector<Location> locations; // Contain all the locations that are in an other class
        std::vector<std::string> cgi_paths; // Define cgi path in the same order than cgi_extensions;
        std::vector<std::string> cgi_extensions; // Define cgi_extensions
        bool autoindex; // Set on/off autoindex (directory listing page. If an index is given and founded, the index is returned, otherwise the directory listing page appear to replace the index)
        std::string to_return; // Set what to return
};
