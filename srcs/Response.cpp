#include "../includes/Response.hpp"

Response::Response(Client & client, Request &request) : client(client) {
	std::cout << "New response is under building.." << std::endl;
	std::string tester = "experiment/expe_ali/site" + request.getPathToFile();
    this->_uri = tester.c_str();
    this->_isAutoindex = request.getIsDir();

	std::ifstream file;
	file.open(tester.c_str(), MimeUtils::getOpenMode(request.getExtension()));
	if (file.fail()) {
		std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
		return;
	}

	contentBuilder(request, file, request.getExtension(), request.getIsDir());
	headerBuilder(request.getFileType());
	_response_size = _header.length() + _content.length();
	_response.resize(_response_size);
	_response = _header + _content;
	std::cout << "Response created. Header:" << std::endl << this->_header;
}

void Response::headerBuilder(std::string file_type) {
	std::stringstream header_tmp;

    if (this->_isAutoindex) {
        file_type = "text/html";
    }
	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.length() << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::generateAutoindex(Request & req) {
   DIR* dir = opendir(this->_uri.c_str());
   if (!dir)
       std::cerr << RED << "can't open dir" << RESET << std::endl;
   perror("");
    std::string content;
    content = "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ";
    content += this->_uri;
    content += "</title>\n</head>\n<body>\n<h1>Index of ";
    content += this->_uri;
    content += "</h1>\n<ul>\n";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        content += "<li><a href=\"http://localhost:8080";
        content += req.getPathToFile();
        content += "/";
        content += entry->d_name;
        content += "\">./";
        content += entry->d_name;
        content += "</a></li>\n";
    }
    content += "</ul>\n</body>\n</html>\n\r\n\r\n";
    closedir(dir);
    this->_content = content;
    std::cout << YELLOW << content << RESET << std::endl;
}

void Response::contentBuilder(Request & req, std::ifstream &file, const std::string &extension, const bool isDir) {
	std::string line;

    if (isDir) {
        generateAutoindex(req);
        return;
    } else if (MimeUtils::isImage(extension) || MimeUtils::isVideo(extension)) {
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

void Response::handleRequestError(int sockfd) {
    std::string header = "";
    std::string codePath = "";
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

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
            send(sockfd, &(header[0]), header.length(), 0);
        } catch (std::exception &e) {
            if (DEBUG)
                std::cerr << RED << "error: failed to send response to client" << RESET << std::endl;
        }
    }
}

std::string & Response::getResponse() {
	return this->_response;
}

std::string &Response::getUri() {
    return this->_uri;
}

int Response::getResponseSize() const {
	return this->_response_size;
}
