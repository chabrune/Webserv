#pragma once

#include "incs.hpp"

enum Methods
{
    POST,
    GET,
    HEAD,
    DELETE,
    PUT
};

class Location
{
    public:
        Location();
        ~Location();
        Location(const Location &src);
        Location& operator=(const Location &rhs);

        std::string path;    // The location's path
        std::string root;    // Set a new root folder / Set the final path to PATH=root+uri
        std::map<Methods, bool> allowed_methods; // Set HTTP methods to true or false (Methods will be an enum)
        std::vector<std::string> cgi_paths; // Define cgi path in the same order than cgi_extensions;
        std::vector<std::string> cgi_extensions; // Define cgi_extensions
        std::vector<std::string> indexes;
        std::vector<std::string> errors_pages;
        unsigned long max_body_size;
        std::string to_return; // Set what to return
        bool autoindex;
};