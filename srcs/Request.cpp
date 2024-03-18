#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

Request::Request() : isDir(false) {}

Request::Request(Server *server, int sockfd) : isDir(false) {
    if (DEBUG)
	    std::cout << "New request receive.. Check for errors" << std::endl;
	std::string buffer;
	buffer.resize(HTTP_BUFFER_SIZE);
	this->len = recv(sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
	if (this->len <= 0)
		throw recvFailure();
	else if (this->len >= HTTP_BUFFER_SIZE)
        throw tooLongRequest();
    if (DEBUG)
	    std::cout << "No errors found, starting to parse.." << std::endl;
	this->parseRequest(server, buffer);
    if (DEBUG)
	    std::cout << "New request created. Method: " << this->method << " file path: " << this->path_to_file << " file type: " << this->file_type << " host: " << this->host  << " keep-alive: " << this->keepalive << std::endl;
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
//    while (this->path_to_file.size() > 2 && this->path_to_file[this->path_to_file.size() - 2] == '/') {
//        this->path_to_file.erase(this->path_to_file.size() - 2);
//    }
	//If the path to file = / set the default page (index.html for example), define in the server config.
	if (this->path_to_file == "/") {
        if (!server->getIndexFrom(this->path_to_file).empty())
            this->path_to_file = server->getIndexFrom(this->path_to_file);
    }
    defineFileType();

	//remove the first line (we already have the necessary information)
	str.erase(0, str.find_first_of('\n') + 1);

	first_space_index = str.find_first_of(' ');
	this->host = str.substr(first_space_index + 1, str.find_first_of('\n') - first_space_index);

	if (str.find("keep-alive") != std::string::npos)
		this->keepalive = true;
}

void Request::defineFileType() {
	this->file_type = this->path_to_file.substr(this->path_to_file.find_first_of('.') + 1, this->path_to_file.length());
	if (this->file_type == "js")
		this->file_type = "javascript";
	this->extension = this->file_type;
	this->file_type.insert(0, MimeUtils::getTypeOfContent(this->file_type) + "/");
}

void Request::tryAccess(Server *server) {
    std::string tester = server->getRootFrom(this->getPathToFile()) + this->subLocation(server->getLocationFrom(this->getPathToFile()));
    //std::cout << YELLOW << this->subLocation(server->getLocationFrom(this->getPathToFile())) << RESET << std::endl;
    if (access(tester.c_str(), F_OK) != 0)
    {
        throw accessError();
    }
    if (access(tester.c_str(), R_OK) != 0)
    {
        throw accessError();
    }
    struct stat filestat;
    if (stat(tester.c_str(), &filestat) == 0) {
        if (S_ISDIR(filestat.st_mode)){
            if (this->path_to_file[this->path_to_file.size() - 1] != '/') {
                errno = MISSINGSLASH;
                this->isDir = true;
                throw dirDoesNotEndWithSlash();
            }
            if (!server->getIndexFrom(this->path_to_file).empty() &&
                    erasesSidesChar(this->path_to_file, '/') == erasesSidesChar(server->getPathFrom(this->path_to_file), '/')) {
                std::cout << MAGENTA << "yep" << RESET << std::endl;
                this->path_to_file += this->path_to_file[this->path_to_file.size() - 1] == '/' ? "" : "/";
                this->path_to_file += server->getIndexFrom(this->path_to_file);
                this->file_type = "text/html";
                return;
            }
            this->isDir = true;
            if (!server->getAutoindexFrom(this->path_to_file)) {
                errno = ISDIRECTORY;
                throw accessError();
            }
        }
    }
}

int isWellSlashed(std::string & str) {
    for (size_t i = 0; str[i + 1]; i++) {
        if (str[i] == '/' && str[i + 1] == '/')
            return -1;
    }
    return 0;
}

void Request::isAllowed(Server *server) {
    if (this->tooLong) {
        errno = TOOLONGREQUEST;
        throw tooLongRequest();
    }
    if (isWellSlashed(this->path_to_file) == -1) {
        errno = INVALIDSLASH;
        throw invalidSlash();
    }
    if (DEBUG)
        std::cout << GREEN << "start check meths" << RESET << std::endl;
    std::vector<std::string> & methods = server->getAllowedMethodsFrom(this->path_to_file);
    std::vector<std::string>::iterator it = methods.begin();
    while (it != methods.end()) {
        if (DEBUG)
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

void Request::setFileType(const std::string &filetype) {
    this->file_type = filetype;
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
