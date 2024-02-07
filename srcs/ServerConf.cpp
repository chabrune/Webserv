#include "../includes/ServerConf.hpp"

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

//void ServerConf::check_serv_line(std::string& line, int& one_line)
//{
//    size_t i = 0;
//    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
//    line = line.substr(i);
//    if(line == "server {")
//        one_line += 1;
//    if(one_line > 1 || line != "server {")
//        throw std::exception();
//}
//
////p-e return int pour check a la fin de la boucle si == 0 = error
//void ServerConf::check_serv_name_line(std::string& line, int& one_line)
//{
//    one_line += 1;
//    if (one_line > 1)
//        throw std::exception();
//    size_t i = 0;
//    for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {} // remove leading whitespaces
//    line = line.substr(i);                                                                // update line without leading whitespaces
//    i = 12;
//    while (i < line.length())
//    {
//        int start = i;
//        while (i < line.length() && !isspace(line[i]))
//            i++;
//        std::string server_name;
//        server_name = line.substr(start, i - start);
//        if(server_name.find(';') != std::string::npos)
//            server_name.pop_back();
//        std::cout << server_name << std::endl;
//        this->server_name = server_name;
//        while (i < line.length() && std::isspace(line[i]))
//            i++;
//    }
//}
//
//
//void ServerConf::inputParsing(std::string argv)
//{
//    std::ifstream file(argv.c_str());
//    if(!file.is_open())
//        std::cerr << "Error: could not open file." << std::endl;
//    std::string line;
//    int one_name = 0;
//    int one_serv = 0;
//    while(std::getline(file, line))
//    {
//        if (line.empty()) // ou commentaires
//            continue;
//        if(line.find("server {") != std::string::npos)
//            check_serv_line(line, one_serv);
//        if(line.find("server_name") != std::string::npos)
//            check_serv_name_line(line, one_name);
//    }
//
//}
