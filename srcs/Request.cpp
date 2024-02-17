#include "../includes/Request.hpp"

Request::Request(int sockfd) {
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
	if (this->path_to_file.length() != 1)
		this->file_type = this->path_to_file.substr(this->path_to_file.find_first_of('.') + 1, this->path_to_file.length());

	//remove the first line (we already have the necessary information)
	str.erase(0, str.find_first_of('\n') + 1);

	first_space_index = str.find_first_of(' ');
	this->host = str.substr(first_space_index + 1, str.find_first_of('\n') - first_space_index);

	if (str.find("keep-alive") != std::string::npos)
		this->keepalive = true;
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

const std::string &Request::getHost() const {
	return host;
}

bool Request::isKeepalive() const {
	return keepalive;
}

