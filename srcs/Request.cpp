#include "../includes/Request.hpp"

Request::Request() : isDir(false) {}

Request::Request(Server *server, int sockfd) : isDir(false) {
	std::cout << "New request receive.. Check for errors" << std::endl;
	std::string buffer;
	buffer.resize(HTTP_BUFFER_SIZE);
	this->len = recv(sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
	if (this->len <= 0)
		throw recvFailure();
	else if (this->len >= HTTP_BUFFER_SIZE)
		throw tooLongRequest();
	std::cout << "No errors found, starting to parse.." << std::endl;
	this->parseRequest(server, buffer);
	std::cout << "New request created. Method: " << this->method << " file path: " << this->path_to_file << " host: " << this->host  << " keep-alive: " << this->keepalive << std::endl;
}

Request::~Request() {}

std::string Request::subLocation(Location *location) {
    if (!location || this->path_to_file.length() < location->path.length())
        return (this->path_to_file);
    std::string lpath = location->path + "/";
    if (this->path_to_file.compare(0, lpath.length()-1, location->path) == 0) {
        return (this->path_to_file.substr(lpath.length()-1));
    }
    return (this->path_to_file);
}

void Request::parseRequest(Server *server, std::string &str) {
	unsigned int first_space_index = str.find_first_of(' ');
	this->len = str.length();
	this->keepalive = false;

	this->method = str.substr(0, first_space_index);
	this->path_to_file = str.substr(first_space_index + 1, str.find_first_of(' ', first_space_index + 1) - (first_space_index + 1));
	//If the path to file = / set the default page (index.html for example), define in the server config.
	if (this->path_to_file == "/") {
        if (!server->getIndexFrom(this->path_to_file).empty())
            this->path_to_file = server->getIndexFrom(this->path_to_file);
        else if (server->getAutoindexFrom(this->path_to_file))
            this->path_to_file += "";
    }
	setFileType();

	//remove the first line (we already have the necessary information)
	str.erase(0, str.find_first_of('\n') + 1);

	first_space_index = str.find_first_of(' ');
	this->host = str.substr(first_space_index + 1, str.find_first_of('\n') - first_space_index);

	if (str.find("keep-alive") != std::string::npos)
		this->keepalive = true;
}

void Request::setFileType() {
	this->file_type = this->path_to_file.substr(this->path_to_file.find_first_of('.') + 1, this->path_to_file.length());
	if (this->file_type == "js")
		this->file_type = "javascript";
	this->extension = this->file_type;
	this->file_type.insert(0, MimeUtils::getTypeOfContent(this->file_type) + "/");
}

void Request::tryAccess(Server *server) {
    std::string tester = server->getRootFrom(this->getPathToFile()) + this->subLocation(server->getLocationFrom(this->getPathToFile()));
    std::cout << YELLOW << tester << RESET << std::endl;
    if (access(tester.c_str(), F_OK) != 0)
        throw accessError();
    if (access(tester.c_str(), R_OK) != 0)
        throw accessError();
    struct stat filestat;
    if (stat(tester.c_str(), &filestat) == 0) {
        if (S_ISDIR(filestat.st_mode)){
            this->isDir = true;
            if (!server->getAutoindexFrom(this->path_to_file)) {
                errno = ISDIRECTORY;
                throw accessError();
            }
        }
    }
}

void Request::isAllowed(Server *server) {
    std::cout << GREEN << "start check meths" << RESET << std::endl;
    std::vector<std::string> & methods = server->getAllowedMethodsFrom(this->path_to_file);
    std::vector<std::string>::iterator it = methods.begin();
    while (it != methods.end()) {
        std::cout << GREEN << "allowed it: " << *it << RESET << std::endl;
        if (this->method == *it)
            break;
        it++;
    }
    if (it == methods.end())
    {
        if (DEBUG)
            std::cout << RED << "Forbidden method: " << this->method << RESET << std::endl;
        errno = NOTALLOWEDMETHOD;
        throw requestError();
    }
}

const std::string &Request::getMethod() const {
	return method;
}

const std::string &Request::getPathToFile() const {
	return path_to_file;
}

const std::string &Request::getFileType() const {
	return file_type;
}

const std::string &Request::getExtension() const {
	return extension;
}

const bool &Request::getIsDir() const {
    return isDir;
}

const std::string &Request::getHost() const {
	return host;
}

bool Request::isKeepalive() const {
	return keepalive;
}
