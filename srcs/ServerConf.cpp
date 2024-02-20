#include "../includes/ServerConf.hpp"
#include "../includes/Server.hpp"
#include "../includes/Mommy.hpp"

ServerConf::ServerConf() {
    this->autoindex = false;
    this->max_body_size = 100000000;
}

ServerConf::~ServerConf() {}

ServerConf& ServerConf::operator=(const ServerConf &rhs)
{
    (void)rhs;
    return(*this);
}

ServerConf::ServerConf(const ServerConf &src)
{
    (void)src;
}

void ServerConf::check_serv_line(std::string& line, Mommy& frr)
{
   size_t i = 0;
   for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
   line = line.substr(i);
   if(line != "server {")
       throw std::exception();
    Server server;
    frr.servers.push_back(new Server(server));
}

//p-e return int pour check a la fin de la boucle si == 0 = error
void ServerConf::check_serv_name_line(std::string& line, int& one_line)
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
        if (server_name.find(';') != std::string::npos)
            server_name.erase(server_name.length() - 1);
       std::cout << server_name << std::endl;
       this->server_name = server_name;
       while (i < line.length() && std::isspace(line[i]))
           i++;
   }
}


void ServerConf::inputParsing(std::string argv, Mommy& frr)
{
   std::ifstream file(argv.c_str());
   if(!file.is_open())
       std::cerr << "Error: could not open file." << std::endl;
   std::string line;
   int one_name = 0;
   while(std::getline(file, line))
   {
       if (line.empty()) // ou commentaires
           continue;
       if(line.find("server {") != std::string::npos)
           check_serv_line(line, frr);
       if(line.find("server_name") != std::string::npos)
           check_serv_name_line(line, one_name);
   }

}
