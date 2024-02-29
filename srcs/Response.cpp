#include "../includes/Response.hpp"

Response::Response() : _contentFile(0), _isGenerated(false) {}

Response::Response(Server & server, Request &request) : _contentFile(0), server(&server), _isGenerated(false) {
	std::cout << "New response is under building.." << std::endl;
	//std::string tester = "experiment/expe_ali/site" + request.getPathToFile();
    std::string tester = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    //std::cout << YELLOW << tester << RESET << std::endl;
    this->_uri = tester.c_str();
    if (request.getIsDir()) {
        generateAutoindex(request);
        return;
    }

    this->_contentFile = new std::ifstream;
	this->_contentFile->open(tester.c_str(), MimeUtils::getOpenMode(request.getExtension()));
	if (!this->_contentFile->is_open()) {
		std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
		return;
	}
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);

	//contentBuilder(request, file, request.getExtension(), request.getIsDir());
	headerFileBuilder(request.getFileType());
	std::cout << "Response created. Header:" << std::endl << this->_header;
}

Response::~Response() {}

void Response::headerFileBuilder(std::string file_type) {
	std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_contentSize << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::headerGenBuilder(std::string file_type) {
    std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content << "\r\n\r\n";
    this->_header = header_tmp.str();
}

void Response::generateAutoindex(Request & req) {
   DIR* dir = opendir(this->_uri.c_str());
   if (!dir)
       std::cerr << RED << "can't open dir" << RESET << std::endl;
    std::stringstream buff;
    buff << "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " << this->_uri << "</title>\n</head>\n<body>\n<h1>Index of " << this->_uri << "</h1>\n<ul>\n";
    struct dirent *entry;
    entry = readdir(dir);
    while (entry != NULL) {
        buff << "<li><a href=\"http://" << this->server->getServerName() << ":";
        std::ostringstream tstring;
        tstring << this->server->getPort();
        buff << tstring.str() << req.getPathToFile() << "/" << entry->d_name << "\">./" << entry->d_name << "</a></li>\n";
        entry = readdir(dir);
    }
    buff << "</ul>\n</body>\n</html>\n";
    closedir(dir);

    this->_content = buff.str();
    headerGenBuilder("text/html");
    this->_isGenerated= true;
    std::cout << "Generated" << std::endl;
    //std::cout << YELLOW << content << RESET << std::endl;
}

//void Response::contentBuilder(Request & req, std::ifstream &file, const std::string &extension, const bool isDir) {
//	std::string line;
//
//    if (isDir) {
//        generateAutoindex(req);
//        return;
//    } else if (MimeUtils::isImage(extension) || MimeUtils::isVideo(extension) || MimeUtils::isAudio(extension) || MimeUtils::isFont(extension)) {
//		file.seekg(0, std::ios::end);
//		int length = file.tellg();
//		file.seekg(0, std::ios::beg);
//		this->_content->resize(length);
//		file.read(&this->_content[0], length);
//		return ;
//	}
//
//	while(file.good()) {
//		std::getline(file, line);
//		//if line is CGI...
//		this->_content.append(line);
//	}
//}

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

void Response::handleRequestError() {
    std::stringstream tmphead;
    std::string codePath;

    if (1) {
        std::cout << RED << "Sending error code, reason: " << errno << RESET << std::endl;
    }

    tmphead << getCodeHeader(&codePath);
    if (this->_contentFile)
        delete this->_contentFile;
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(codePath.c_str(),std::ifstream::in);
    if (!this->_contentFile->is_open())
        std::cerr << RED << "aled le file code d'errur il est pa ouvert" << RESET << std::endl;

    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
    tmphead << "Content-Length: " << this->_contentSize << "\nContent-Type: text/html\n\n";
    this->_header = tmphead.str();
}

std::string & Response::getContent() {
    return this->_content;
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

std::string &Response::getHeader() {
    return this->_header;
}

bool Response::getGenerated() {
    return this->_isGenerated;
}
