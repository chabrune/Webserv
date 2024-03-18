#pragma once

#include "Location.hpp"
#include <cstdlib>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

class Mommy;

class ServerConf
{
    public:
        ServerConf();
        ~ServerConf();
        ServerConf(const ServerConf &src);
        ServerConf& operator=(const ServerConf &rhs);

        void inputParsing(std::string argv, Mommy& frr);
        void serv_line(std::string &line);
        void serv_name_line(std::string& line, size_t currentServerIndex, Mommy& frr);
        void serv_port(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_root(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_index(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_return(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_error_page(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_CGI(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr);
        void serv_autoindex(std::string &line, size_t currentServerIndex, Mommy& frr);
        void location_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_root(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_CGI_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_CGI_ext(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_index(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_error_page(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_return(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void location_autoindex(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex);
        void testparsing(Mommy& frr);
        bool requirements_serv(Mommy& frr, size_t currentServerIndex);

        const std::string& getRoot() const { return this->root; }
        const std::string& getServerName() const { return this->server_name; }
        int getPort() const { return this->port; }
        const std::vector<std::string>& getAllowedMethods() const { return this->allowed_methods; }
        const std::string& getIndex() const { return this->index; }
        const std::map<unsigned int, std::string>& getErrorsPages() const { return this->errors_pages; }
        unsigned long getMaxBodySize() const { return this->max_body_size; }
        const std::vector<Location*>& getLocations() const { return this->locations; }
        const std::string &getCgiPathFromExtension(const std::string &extension) { return this->cgi_values[extension]; }
        bool isCgi(const std::string &extension) const;
        bool getAutoindex() const { return this->autoindex; }
        const std::map<unsigned int, std::string>& getToReturn() const { return this->to_return; }

        bool isReturn;
        std::vector<Location*> locations; // Contain all the locations that are in an other class
        bool autoindex; // Set on/off autoindex (directory listing page. If an index is given and founded, the index is returned, otherwise the directory listing page appear to replace the index)
        std::vector<std::string> allowed_methods; // Set HTTP methods to true or false (methods will be an enum)
        std::map<unsigned int, std::string> errors_pages; // contain errors page paths
        std::map<std::string, std::string> cgi_values; // key is extension value is path
        std::map<unsigned int, std::string> to_return; // Set what to return
        std::string root; // Define the root directory
        std::string index;    // contain indexes (if several index given, we try to load in the given order)
        std::string server_name;    // contain the server_name
        int port; // contain ports to listen to
        unsigned long max_body_size;  // HTTP request's body max size (usefull for PUT and POST)
};