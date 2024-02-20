#include "../includes/Request.hpp"

Request::Request() : isDir(false) {}

Request::Request(int sockfd) : isDir(false) {
	std::cout << "New request receive.. Check for errors" << std::endl;
	std::string buffer;
	buffer.resize(HTTP_BUFFER_SIZE);
	this->len = recv(sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
	if (this->len <= 0)
		throw recvFailure();
	else if (this->len >= HTTP_BUFFER_SIZE)
		throw tooLongRequest();
	std::cout << "No errors found, starting to parse.." << std::endl;
	this->parseRequest(buffer);
	std::cout << "New request created. Method: " << this->method << " file path: " << this->path_to_file << " host: " << this->host  << " keep-alive: " << this->keepalive << std::endl;
}

Request::~Request() {}

void Request::parseRequest(std::string &str) {
	unsigned int first_space_index = str.find_first_of(' ');
	this->len = str.length();
	this->keepalive = false;

	this->method = str.substr(0, first_space_index);
	this->path_to_file = str.substr(first_space_index + 1, str.find_first_of(' ', first_space_index + 1) - (first_space_index + 1));
	//If the path to file = / set the default page (index.html for example), define in the server config.
	if (this->path_to_file == "/")
		this->path_to_file = "/index.html";

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

int Request::tryAccess(Request & req) {
    std::string tester = "experiment/expe_ali/site/" + this->path_to_file;
    if (access(tester.c_str(), F_OK) != 0)
        throw accessError();
    if (access(tester.c_str(), R_OK) != 0)
        throw accessError();
    struct stat filestat;
    if (stat(tester.c_str(), &filestat) == 0) {
        if (S_ISDIR(filestat.st_mode)) { // Faire en sorte de rediriger vers autoindex ou non
            req.isDir = true;
            errno = ISDIRECTORY;
            throw accessError();
        }
    }
    return (0);
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
