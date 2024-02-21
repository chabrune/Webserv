#include "../includes/ServerConf.hpp"
#include "../includes/Server.hpp"
#include "../includes/Mommy.hpp"

ServerConf::ServerConf() {
    this->autoindex = false;
    this->max_body_size = 100000000;
}

ServerConf::~ServerConf() {}

ServerConf& ServerConf::operator=(const ServerConf& rhs)
{
    (void)rhs;
    return(*this);
}

ServerConf::ServerConf(const ServerConf &src)
{
    (void)src;
}

void ServerConf::check_serv_line(std::string& line)
{
   size_t i = 0;
   for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
   line = line.substr(i);
   if(line != "server {")
       throw std::exception();
}

// p-e return int pour check a la fin de la boucle si == 0 = error
void ServerConf::check_serv_name_line(std::string& line, size_t currentServerIndex, Mommy& frr) 
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
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
		if(line != "server_name " + server_name)
			throw std::logic_error("config file : check server_name");
		frr.servers[currentServerIndex]->server_name = server_name;
		i++;
	}
}

void ServerConf::check_serv_port(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 7;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string sport = line.substr(start, i - start);
		if(line != "listen " + sport)
			throw std::logic_error("config file : check port");
		int port = atoi(sport.c_str());
		if (!port || port <= 0 || port > 65535)
            throw std::invalid_argument("Invalid port number.");
		frr.servers[currentServerIndex]->port = port;
		i++;
	}
}

void ServerConf::check_serv_root(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 5;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string root = line.substr(start, i - start);
		if(line != "root " + root)
			throw std::logic_error("config file : check root path");
		frr.servers[currentServerIndex]->root = root;
		i++;
	}
}

void ServerConf::check_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 21;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string smax_body_size = line.substr(start, i - start);
		if(line != "client_max_body_size " + smax_body_size)
			throw std::logic_error("config file : check client_max_body_size");
    	unsigned long max_body_size = 0;
    	char* endPtr = NULL;
    	errno = 0;
    	max_body_size = strtoul(smax_body_size.c_str(), &endPtr, 10);
    	if(*endPtr != '\0' || (max_body_size == ULONG_MAX && errno == ERANGE))
		{
        	throw std::invalid_argument("Invalid value for max_body_size.");
		}
		frr.servers[currentServerIndex]->max_body_size = max_body_size;
		i++;
	}
}


void ServerConf::inputParsing(std::string argv, Mommy& frr) 
{
    std::ifstream file(argv.c_str());
    if (!file.is_open()) 
	{
        std::cerr << "Error: could not open file.\n";
        return;
    }
    std::string line;
    bool isInsideServerSection = false;
    bool isInsideLocationSection = false;
    size_t currentServerIndex = frr.servers.size();
    while (std::getline(file, line)) 
	{
        if (line.empty() || line.find("#") == 0 || line.find("//") == 0) continue;
		if(line.find("location") != std::string::npos)
		{
			isInsideLocationSection = true;
			//LOCATION A FAIRE ......
			continue;
		}
        if (line.find("server {") == 0)
		{
			check_serv_line(line);
			Server* server = new Server;
			frr.servers.push_back(server);
            isInsideServerSection = true;
            continue;
        }
		if(line.find("    }") == 0)
		{
			isInsideLocationSection = false;
			continue;
		}
        if (line.find("}") == 0) 
		{
            isInsideServerSection = false;
            ++currentServerIndex;
            continue;
        }
        if (isInsideServerSection && line.find("server_name") != std::string::npos)
            check_serv_name_line(line, currentServerIndex, frr);
		else if(isInsideServerSection && line.find("listen") != std::string::npos)
            check_serv_port(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("root") != std::string::npos)
			check_serv_root(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("client_max_body_size") != std::string::npos)
			check_client_max_body_size(line, currentServerIndex, frr);
    }
}