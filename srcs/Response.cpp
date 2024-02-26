#include "../includes/Response.hpp"

Response::Response(int sockfd, Server & server, Request &request) : server(server) {
	std::cout << "New response is under building.." << std::endl;
	//std::string tester = "experiment/expe_ali/site" + request.getPathToFile();
    std::string tester = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    //std::cout << YELLOW << tester << RESET << std::endl;
    this->_uri = tester.c_str();
    this->_isAutoindex = request.getIsDir();

	//if false faire une erreur
	fileManager(tester.c_str(), request.getExtension());

	headerManager(sockfd, request.getFileType());

	if (MimeUtils::isMedia(request.getExtension()))
		mediaContentManager(sockfd);
	else if (request.getIsDir())
		generateAutoindex(sockfd, request);
	else
		contentManager(sockfd);

	//std::cout << "Response created. Header:" << std::endl << this->_header;
}

bool Response::fileManager(const char *path, const std::string &extension) {
	_file.open(path, MimeUtils::getOpenMode(extension));
	if (_file.fail()) {
		std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
		return false;
	}
	_file.seekg(0, std::ifstream::end);
	_content_size = _file.tellg();
	_file.seekg(0, std::ifstream::beg);
	return true;
}

void Response::headerManager(int sockfd, std::string file_type) const {
	std::stringstream header;

    if (this->_isAutoindex) {
        file_type = "text/html";
    }
	header << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content_size << "\r\n\r\n";
	send(sockfd, &(header.str()[0]), header.str().length(), 0);
}

void Response::generateAutoindex(int sockfd, Request & req) {
   DIR* dir = opendir(this->_uri.c_str());
   std::cout << "dddddd: " << this->_uri.c_str() << std::endl;
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
        content += "<li><a href=\"http://";
        content += this->server.getServerName();
        content += ":";
        std::ostringstream tstring;
        tstring << this->server.getPort();
        content+= tstring.str();
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
	send(sockfd, &(this->_content[0]), this->_content_size, 0);
	//std::cout << YELLOW << content << RESET << std::endl;
}

void Response::mediaContentManager(int sockfd) {
	while (_content_size > 0) {
		size_t bytes_to_read = std::min(static_cast<size_t>(BUFFER_SIZE), static_cast<size_t>(_content_size));
		this->_content.resize(bytes_to_read);
		_file.read(&this->_content[0], bytes_to_read);
		ssize_t bytes_sent = send(sockfd, &(this->_content[0]), bytes_to_read, 0);
		if (bytes_sent <= 0) {
			std::cout << "err" << std::endl;
			return;
		}
		_content_size -= bytes_sent;
	}
}

void Response::contentManager(int sockfd) {
	std::string line;
	while(_file.good()) {
		std::getline(_file, line);
		//if line is CGI...
		this->_content.append(line);
	}
	send(sockfd, &(this->_content[0]), this->_content_size, 0);
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
    if (errno == NOTALLOWEDMETHOD) {
        *path = "ressources/default/405.html";
        return ("HTTP/1.1 405 Method Not Allowed\n");
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

std::string &Response::getUri() {
    return this->_uri;
}
