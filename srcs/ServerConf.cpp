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

void ServerConf::serv_line(std::string& line)
{
   size_t i = 0;
   for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
   line = line.substr(i);
   if(line != "server {")
       throw std::exception();
}

void ServerConf::serv_name_line(std::string& line, size_t currentServerIndex, Mommy& frr)
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
			throw std::logic_error("config file : server : check server_name");
		frr.servers[currentServerIndex]->server_name = server_name;
		i++;
	}
}

void ServerConf::serv_port(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 7;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string sport = line.substr(start, i - start);
	if(line != "listen " + sport)
		throw std::logic_error("config file : server : check port");
	int port = atoi(sport.c_str());
	if (!port || port <= 0 || port > 65535)
		throw std::invalid_argument("Invalid port number.");
	frr.servers[currentServerIndex]->port = port;
}

void ServerConf::serv_root(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 5;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string root = line.substr(start, i - start);
	if(line != "root " + root)
		throw std::logic_error("config file : server : check root path");
	frr.servers[currentServerIndex]->root = root;
}

void ServerConf::serv_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr)
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
			throw std::logic_error("config file : server : check client_max_body_size");
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

void ServerConf::serv_index(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string index = line.substr(start, i - start);
	if(line != "index " + index)
		throw std::logic_error("config file : server : check index");
	frr.servers[currentServerIndex]->index = index;
}

void ServerConf::serv_return(std::string &line, size_t currentServerIndex, Mommy& frr)
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
		std::string sreturn = line.substr(start, i - start);
		if(line != "return " + sreturn)
			throw std::logic_error("config file : server : check return");
		frr.servers[currentServerIndex]->to_return = sreturn;
		i++;
	}
}

void ServerConf::serv_error_page(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 11;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string serror_nb = line.substr(start, i - start);
	int error_nb = atoi(serror_nb.c_str());
	if(error_nb < 100 || error_nb > 505)
		throw std::logic_error("config file : server : check error_page number");
	while(i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string error_path = line.substr(start, i - start);
	if(line != "error_page " + serror_nb + " " + error_path)
		throw std::logic_error("config file : server : check error_page path");
	frr.servers[currentServerIndex]->errors_pages[error_nb] = error_path;
}

void ServerConf::serv_CGI(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 4;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
			i++;
	std::string cgi_extension = line.substr(start, i - start);
	if(cgi_extension[0] != '.' || cgi_extension != ".py")
		throw std::logic_error("config file : server : check cgi");
	frr.servers[currentServerIndex]->cgi_extensions.push_back(cgi_extension);
	while(isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string cgi_path = line.substr(start, i - start);
	if(line != "cgi " + cgi_extension + " " + cgi_path)
		throw std::logic_error("config file : server : check cgi");
	frr.servers[currentServerIndex]->cgi_paths.push_back(cgi_path);
}

void ServerConf::serv_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string methods = line.substr(start, i - start);
		if(methods != "GET" && methods != "HEAD" && methods != "POST" && methods != "DELETE" && methods != "PUT")
			throw std::logic_error("config file : server : check allowed methods");
		frr.servers[currentServerIndex]->allowed_methods.push_back(methods);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->allowed_methods.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->allowed_methods.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "allow " + check)
		throw std::logic_error("config file : server : check allow methods");
}

void ServerConf::serv_autoindex(std::string &line, size_t currentServerIndex, Mommy& frr)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 10;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string autoindex = line.substr(start, i - start);
	if(autoindex != "on" && autoindex != "off")
		throw std::logic_error("config file : server : check autoindex");
	if(autoindex == "on")
		frr.servers[currentServerIndex]->autoindex = true;
	else
		frr.servers[currentServerIndex]->autoindex = false;
}

void ServerConf::location_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 9;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string location_path = line.substr(start, i - start);
	if(line != "location " + location_path + " {")
		throw std::logic_error("config file : location : check path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex].path = location_path;
}

void ServerConf::location_root(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 5;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string root = line.substr(start, i - start);
	if(line != "root " + root)
		throw std::logic_error("config file : location : check root path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex].root = root;
}

void ServerConf::location_allowed_methods(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 14;
	while (i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
			i++;
		std::string methods = line.substr(start, i - start);
		if(methods != "GET" && methods != "HEAD" && methods != "POST" && methods != "DELETE" && methods != "PUT")
			throw std::logic_error("config file : location : check allowed methods");
		frr.servers[currentServerIndex]->locations[currentLocationIndex].allowed_methods.push_back(methods);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex].allowed_methods.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex].allowed_methods.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "allow_methods " + check)
		throw std::logic_error("config file : location : check allow_methods");
}

void ServerConf::location_CGI_path(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 9;
	while(i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
				i++;
		std::string cgi_path = line.substr(start, i - start);
		frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_paths.push_back(cgi_path);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_paths.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_paths.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "cgi_path " + check)
		throw std::logic_error("config file : location : check CGI path");
}

void ServerConf::location_CGI_ext(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 8;
	while(i < line.length())
	{
		int start = i;
		while (i < line.length() && !isspace(line[i]))
				i++;
		std::string cgi_extension = line.substr(start, i - start);
		if(cgi_extension != ".py")
			throw std::logic_error("config file : location : check CGI ext (only python ? bonus ?)");
		frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_extensions.push_back(cgi_extension);
		i++;
	}
	std::vector<std::string>::iterator it = frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_extensions.begin();
	std::vector<std::string>::iterator itend = frr.servers[currentServerIndex]->locations[currentLocationIndex].cgi_extensions.end();
	std::string check;
	while(it != itend)
	{
		check += *it + " ";
		++it;
	}
	check.erase(check.length() - 1);
	if(line != "cgi_ext " + check)
		throw std::logic_error("config file : location : check CGI ext");
}

void ServerConf::location_index(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 6;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string index = line.substr(start, i - start);
	if(line != "index " + index)
		throw std::logic_error("config file : location : check index");
	frr.servers[currentServerIndex]->locations[currentLocationIndex].index = index;
}

void ServerConf::location_error_page(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
{
	size_t i = 0;
	for (; i < line.length() && std::isspace(static_cast<unsigned char>(line[i])); i++) {}
	line = line.substr(i);
	i = 11;
	int start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string serror_nb = line.substr(start, i - start);
	int error_nb = atoi(serror_nb.c_str());
	if(error_nb < 100 || error_nb > 505)
		throw std::logic_error("config file : location : check error_page number");
	while(i < line.length() && isspace(line[i]))
		i++;
	start = i;
	while (i < line.length() && !isspace(line[i]))
		i++;
	std::string error_path = line.substr(start, i - start);
	if(line != "error_page " + serror_nb + " " + error_path)
		throw std::logic_error("config file : location : check error_page path");
	frr.servers[currentServerIndex]->locations[currentLocationIndex].errors_pages[error_nb] = error_path;
}

void ServerConf::location_client_max_body_size(std::string &line, size_t currentServerIndex, Mommy& frr, size_t currentLocationIndex)
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
			throw std::logic_error("config file : server : check client_max_body_size");
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
    size_t currentLocationIndex = 0;
    while (std::getline(file, line))
	{
        if (line.empty() || line.find("#") == 0 || line.find("//") == 0) continue;
		if(line.find("location") != std::string::npos)
		{
			isInsideLocationSection = true;
			Location location;
			frr.servers[currentServerIndex]->locations.push_back(location);
			location_path(line, currentServerIndex, frr, currentLocationIndex);
			continue;
		}
        if (line.find("server {") == 0)
		{
			serv_line(line);
			Server* server = new Server;
			frr.servers.push_back(server);
            isInsideServerSection = true;
            continue;
        }
		if(line.find("    }") == 0)
		{
			isInsideLocationSection = false;
			++currentLocationIndex;
			continue;
		}
        if (line.find("}") == 0)
		{
            isInsideServerSection = false;
            ++currentServerIndex;
            continue;
        }
        if (isInsideServerSection && !isInsideLocationSection && line.find("server_name") != std::string::npos)
            serv_name_line(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("listen") != std::string::npos)
            serv_port(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("root") != std::string::npos)
			serv_root(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("client_max_body_size") != std::string::npos)
			serv_client_max_body_size(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("index") != std::string::npos && line.find("autoindex") == std::string::npos)
			serv_index(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("return") != std::string::npos)
			serv_return(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("error_page") != std::string::npos)
			serv_error_page(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("cgi") != std::string::npos)
			serv_CGI(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("allow") != std::string::npos)
			serv_allowed_methods(line, currentServerIndex, frr);
		else if(isInsideServerSection && !isInsideLocationSection && line.find("autoindex") != std::string::npos)
			serv_autoindex(line, currentServerIndex, frr);
		else if(isInsideLocationSection && line.find("root") != std::string::npos)
			location_root(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("allow_methods") != std::string::npos)
			location_allowed_methods(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("cgi_path") != std::string::npos)
			location_CGI_path(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("cgi_ext") != std::string::npos)
			location_CGI_ext(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("index") != std::string::npos && line.find("autoindex") == std::string::npos)
			location_index(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("error_page") != std::string::npos)
			location_error_page(line, currentServerIndex, frr, currentLocationIndex);
		else if(isInsideLocationSection && line.find("client_max_body_size") != std::string::npos)
			location_client_max_body_size(line, currentServerIndex, frr, currentLocationIndex);
    }
}
