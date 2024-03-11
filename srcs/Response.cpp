#include "../includes/Response.hpp"

Response::Response() : _contentFile(0), _isGenerated(false) {}
#include <errno.h>
Response::Response(Server & server, Request &request) : _contentFile(0), server(&server), _isGenerated(false) {
	std::cout << "New response is under building.." << std::endl;
    std::string tester = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    this->_uri = tester.c_str();
    if (request.getIsDir()) {
        generateAutoindex(request);
        return;
    }

	if (isCgi(request.getFileType())) {
        cgiBuilder(request);
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

	headerFileBuilder(request.getFileType());
	std::cout << "Response created. Header:" << std::endl << this->_header;
}

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
}

//env example
/*AUTH_TYPE=Basic
CONTENT_LENGTH=
CONTENT_TYPE=
DOCUMENT_ROOT=./
GATEWAY_INTERFACE=CGI/1.1
HTTP_COOKIE=
PATH_INFO=
PATH_TRANSLATED=.//
QUERY_STRING=
REDIRECT_STATUS=200
REMOTE_ADDR=localhost:8002
REQUEST_METHOD=GET
REQUEST_URI=/cgi-bin/acc.py
        SCRIPT_FILENAME=acc.py
SCRIPT_NAME=cgi-bin/acc.py
SERVER_NAME=localhost
SERVER_PORT=8002
SERVER_PROTOCOL=HTTP/1.1
SERVER_SOFTWARE=AMANIX*/

void Response::cgiBuilder(const Request &request) {
    std::string runner = "/usr/bin/python3";
    std::string script = "experiment/expe_ali/site/testpy.py";
    std::string scriptname = "testpy.py";

    //argv builder
    char **argv = new char*[3];
    for (int i = 0; i < 3; i++)
        argv[i] = new char[50];
    strcpy(argv[0], runner.c_str());  // Path to runner
    strcpy(argv[1], script.c_str());  // Path to script
    argv[2] = 0;

    //env builder
    std::map<std::string, std::string> env_as_map;
    env_as_map["AUTH_TYPE="] = "Basic";
    env_as_map["CONTENT_LENGTH="] = "";
    env_as_map["CONTENT_TYPE="] = "";       //request.getFileType();
    env_as_map["GATEWAY_INTERFACE="] = "CGI/1.1";
    env_as_map["SCRIPT_NAME="] = script.c_str();
    env_as_map["SCRIPT_FILENAME="] = scriptname.c_str();
    //this->_env["PATH_TRANSLATED"] = it_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
    //this->_env["QUERY_STRING"] = decode(req.getQuery());
    //this->_env["REMOTE_ADDR"] = req.getHeader("host");
    env_as_map["SERVER_NAME="] = "localhost";
    env_as_map["SERVER_PORT="] = "8080";
    env_as_map["REQUEST_METHOD="] = request.getMethod();
    //this->_env["HTTP_COOKIE"] = req.getHeader("cookie");
    //this->_env["DOCUMENT_ROOT"] = it_loc->getRootLocation();
    //this->_env["REQUEST_URI"] = req.getPath() + req.getQuery();*
    env_as_map["SERVER_PROTOCOL="] = "HTTP/1.1";
    env_as_map["REDIRECT_STATUS="] = "200";
    env_as_map["SERVER_SOFTWARE="] = "AMANIX";

    this->_env = new char*[env_as_map.size() + 1];
    std::map<std::string, std::string>::const_iterator it = env_as_map.begin();
    for (int i = 0; it != env_as_map.end(); it++, i++) {
        std::string key_and_value = it->first + it->second;
        this->_env[i] = strdup(key_and_value.c_str());
    }
    this->_env[env_as_map.size()] = 0;

    pipeCreatorAndExec(argv);
    //chdir("experiment/expe_ali/site/");

    //readfrom pipe
    char t[1024];
    read(_pipe_out[0], t, 1024);
    std::cout << "res= " << t << std::endl;
    closeAllPipe();
}

bool Response::isCgi(const std::string &file_type) {
	//temp system
	if (file_type.find("py") != std::string::npos)
		return true;
	return false;
}

void Response::pipeCreatorAndExec(char **argv) {
    if (pipe(this->_pipe_out) < 0) {
        std::cout << "pipe1 marche po" << std::endl;
        exit(1);
    }
    if (pipe(this->_pipe_in) < 0) {
        std::cout << "pipe2 marche po" << std::endl;
        exit(1);
    }
    _pid = fork();
    if (_pid < 0) {
        std::cout << "fork marche po" << std::endl;
        exit(1);
    }

    if (_pid == 0) {
        dup2(_pipe_in[0], STDIN_FILENO);
        dup2(_pipe_out[1], STDOUT_FILENO);
        closeAllPipe();
        exit(execve(argv[0], argv, this->_env));
    }
}

void Response::closeAllPipe() {
    close(_pipe_in[0]);
    close(_pipe_in[1]);
    close(_pipe_out[0]);
    close(_pipe_out[1]);
}

std::string intToString(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string Response::getCodeHeader(std::string * path, Server* server) {
    (void)server;
    // if(!server->to_return.empty())
    // {
    //     std::string newPath;
    //     std::map<int, std::string>::iterator it = server->to_return.begin();
    //     size_t pos = it->second.find("ressources");
    //     if(pos != std::string::npos)
    //     {
    //         size_t i = pos;
    //         while(i < it->second.length())
    //             i++;
    //         newPath = it->second.substr(pos, i);
    //     }
    //     *path = newPath;
    //     return("HTTP/1.1 404 Not Found\n");
    // }
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

void Response::handleRequestError(Server* server) {
    std::stringstream tmphead;
    std::string codePath;

    if (DEBUG)
        std::cout << RED << "Sending error code, reason: " << errno << RESET << std::endl;

    tmphead << getCodeHeader(&codePath, server);
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

std::string &Response::getUri() {
    return this->_uri;
}

std::string &Response::getHeader() {
    return this->_header;
}

bool Response::getGenerated() {
    return this->_isGenerated;
}
