#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

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
    Location& operator=(const Location &rhs);
    Location(const Location &src);
    std::string path;    // The location's path
    std::string root;    // Set a new root folder / Set the final path to PATH=root+location+uri
    std::string alias;     // Set the final path to PATH=alias+uri. ALIAS GET PRIORITY ON ROOT
    std::map<Methods, bool> allowed_methods; // Set HTTP methods to true or false (Methods will be an enum)
    std::vector<std::string> cgi_paths; // Define cgi path in the same order than cgi_extensions;
    std::vector<std::string> cgi_extensions; // Define cgi_extensions
    std::string to_return; // Set what to return
    std::vector<std::string> indexes;
    std::vector<std::string> errors_pages;
    unsigned long max_body_size;
    bool autoindex;
};