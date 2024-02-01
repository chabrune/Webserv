#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server() {}

Server& Server::operator=(const Server &rhs)
{
    (void)rhs;
    return(*this);
}

Server::Server(const Server &src)
{
    (void)src;
}


void Server::check_serv_line(std::string& line, int& one_line)
{
    size_t i = 0;
    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
    line = line.substr(i);
    if(line == "server {")
        one_line += 1;
    if(one_line > 1 || line != "server {")
        throw std::exception();
}

//p-e return int pour check a la fin de la boucle si == 0 = error
void Server::check_serv_name_line(std::string& line, int& one_line)
{
    size_t i = 0;
    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
    line = line.substr(i);
    std::cout << line << std::endl;
    if(line == "server_name localhost;\n")
        one_line += 1;
    if(one_line > 1 || line != "server_name localhost;")
        throw std::exception();
}

void Server::inputParsing(std::string argv)
{
    std::ifstream file(argv.c_str());
    if(!file.is_open())
        std::cerr << "Error: could not open file." << std::endl;
    std::string line;
    int one_line = 0;
    std::cout << one_line << std::endl;
    while(std::getline(file, line))
    {
        if (line.empty())
            continue;
        if(line.find("server {") != std::string::npos)
            check_serv_line(line, one_line);
        if(line.find("server_name localhost;") != std::string::npos)
            check_serv_name_line(line, one_line);
    }

}
