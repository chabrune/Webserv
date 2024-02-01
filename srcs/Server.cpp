#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server() {}

Server& Server::operator=(const Server &rhs)
{
    return(*this);
}

Server::Server(const Server &src)
{}


bool Server::check_serv_line(std::string& line)
{
    int i = 0;
    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
    line = line.substr(i);
    if(line != "server {")
        return (false);
    std::cout << line << std::endl;
    return(true);
}


void Server::inputParsing(std::string argv)
{
    std::ifstream file(argv.c_str());
    if(!file.is_open())
        std::cerr << "Error: could not open file." << std::endl;
    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty())
            continue;
        if(line.find("server {") != std::string::npos)
            check_serv_line(line);
    }

}
