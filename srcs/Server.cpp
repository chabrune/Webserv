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
    one_line += 1;
    if (one_line > 1)
        throw std::exception();
    size_t i = 0;
    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {} // remove leading whitespaces
    line = line.substr(i);                                                                // update line without leading whitespaces
    i = 12;
    while (i < line.length()) 
    {
        int start = i;
        while (i < line.length() && !isspace(line[i]))
            i++;
        std::string server_name;
        server_name = line.substr(start, i - start);
        if(server_name.find(';') != std::string::npos)
            server_name.pop_back();
        std::cout << server_name << std::endl;
        this->server_names.push_back(server_name);
        while (i < line.length() && std::isspace(line[i]))
            i++;
    }
}


void Server::inputParsing(std::string argv)
{
    std::ifstream file(argv.c_str());
    if(!file.is_open())
        std::cerr << "Error: could not open file." << std::endl;
    std::string line;
    int one_name = 0;
    int one_serv = 0;
    while(std::getline(file, line))
    {
        if (line.empty()) // ou commentaires 
            continue;
        if(line.find("server {") != std::string::npos)
            check_serv_line(line, one_serv);
        if(line.find("server_name") != std::string::npos)
            check_serv_name_line(line, one_name);
    }

}

void Server::setup_socket(void)
{
    this->sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
        throw socketCreationError();
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 1, sizeof(int)) == -1)
        throw socketCreationError();
    this->timeout.tv_sec = 10;
    this->timeout.tv_usec = 0;
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketCreationError();
    if (setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketCreationError();
    if (bind(this->sock_fd, (struct sockaddr *)&this->address, sizeof(this->address)) == -1)
        throw addrBindError();
    if (listen(this->sock_fd, 2000) == -1)
        throw sockListeningError();

    // A deplacer dans le parsing de l'addresse
    this->addrlen = (socklen_t)(sizeof(this->address));

    std::cout << GREEN << "-listener socket ready-" << RESET << std::endl;
}

void Server::run(void)
{
    int nclient;

    this->running = true;
    while (this->running) {
        nclient = accept(this->sock_fd, (struct sockaddr *)&this->address, &this->addrlen);
        getRequest(nclient);
    }
}
