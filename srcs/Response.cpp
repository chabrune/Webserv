#include "../includes/Response.hpp"
#include <cerrno>
#include <cstring>
Response::Response(const Request &request) {
	std::cout << "New response is under building.." << std::endl;
	std::string tester = "experiment/expe_ali/site" + request.getPathToFile();

	std::ifstream file;
	file.open(tester.c_str(), MimeUtils::getOpenMode(request.getExtension()));
	if (file.fail()) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return;
	}

	contentBuilder(file, request.getExtension());
	headerBuilder(request.getFileType());
	_response_size = _header.length() + _content.length();
	_response.resize(_response_size);
	_response = _header + _content;
	std::cout << "Response created. Header:" << std::endl << this->_header;
}

void Response::headerBuilder(const std::string &file_type) {
	std::stringstream header_tmp;

	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.length() << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::contentBuilder(std::ifstream &file, const std::string &extension) {
	std::string line;
	if (MimeUtils::isImage(extension) || MimeUtils::isVideo(extension)) {
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		file.seekg(0, std::ios::beg);
		this->_content.resize(length);
		file.read(&this->_content[0], length);
		return ;
	}

	while(file.good()) {
		std::getline(file, line);
		//if line is CGI...
		this->_content.append(line);
	}
}

std::string intToString(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string Response::getCodeHeader(std::string * path) {
    if (errno == EACCES || errno == EROFS) {
        *path = "ressources/default/403.html";
        return ("HTTP/1.1 403 Forbidden\n");
    }
    if (errno == ENAMETOOLONG) {
        *path = "ressources/default/414.html";
        return ("HTTP/1.1 414 Uri Too Long\n");
    }
    if (errno == ENOENT) {
        *path = "ressources/default/404.html";
        return ("HTTP/1.1 404 Not Found\n");
    }
    if (errno == ENOTDIR || errno == EINVAL || errno == EROFS || errno == ISDIRECTORY) {
        *path = "ressources/default/400.html";
        return ("HTTP/1.1 400 Bad Request\n");
    }
    if (errno == ETXTBSY) {
        *path = "ressources/default/409.html";
        return ("HTTP/1.1 409 Conflict\n");
    }
    *path = "ressources/default/500.html";
    return ("HTTP/1.1 500 Internal Server Error\n");
}

void Response::handleRequestError(requestError &e, int sockfd) {
    std::string header = "";
    std::string codePath = "";
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    (void)e;
    if (DEBUG)
        std::cout << RED << "Sending error code, reason: " << errno << RESET << std::endl;
    header = getCodeHeader(&codePath);
    std::ifstream file;
    file.open(codePath.c_str(),std::ifstream::in);
    if (file.is_open()) {
        header += intToString(strlen(buffer));
        header += "\nContent-Type: text/html\n\n";
        file.read(buffer, sizeof(buffer));
        header += buffer;
        header += "\r\n\r\n";
    } else if (DEBUG) {
        std::cerr << RED << "Failed to open code path" << RESET << std::endl;
    }
    if (!header.empty()) {
        try {
            std::cout << GREEN << "HEADER:\n" << header << RESET << std::endl;
            send(sockfd, &(header[0]), header.length(), 0);
        } catch (std::exception &e) {
            if (DEBUG)
                std::cerr << RED << "error: failed to send response to client" << RESET << std::endl;
        }
    }
}

std::string Response::getResponse() const {
	return this->_response;
}

int Response::getResponseSize() const {
	return this->_response_size;
}
