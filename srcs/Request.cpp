#include "../includes/Request.hpp"
#include "../includes/utils.hpp"
#include "../includes/Server.hpp"

Request::Request() : isDir(false) {}

Request::Request(Server *server, int sockfd) : tooLong(false), isDir(false) {
    if (DEBUG)
	    std::cout << "New request receive.. Check for errors" << std::endl;
	std::string buffer;
	buffer.resize(HTTP_BUFFER_SIZE);
	this->len = recv(sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
	if (this->len <= 0)
		throw recvFailure();
	// else if (this->len >= HTTP_BUFFER_SIZE)
    //     throw tooLongRequest();
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << MAGENTA << buffer << RESET << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    if (DEBUG)
	    std::cout << "No errors found, starting to parse.." << std::endl;
	this->parseRequest(server, buffer);
    if (DEBUG)
	    std::cout << "New request created. Method: " << this->method << " file path: " << getPathToFile() << " file-name " << getFileName() << " query " << getQuery() << " file type: " << this->file_type << " host: " << this->host << std::endl;
}

Request::~Request() {}

std::string Request::subLocation(Location *location) {
    if (!location || this->_path_to_file.length() < location->path.length())
        return (this->_path_to_file);
    std::string lpath = location->path;
    if (this->_path_to_file.compare(0, lpath.length(), location->path) == 0) {
        return (this->_path_to_file.substr(lpath.length()));
    }
    return (this->_path_to_file);
}

void Request::parseRequest(Server *server, std::string &str) {
	unsigned int first_space_index = str.find_first_of(' ');
	this->len = str.length();

	this->method = str.substr(0, first_space_index);
    setPathToFile(str.substr(first_space_index + 1, str.find_first_of(' ', first_space_index + 1) - (first_space_index + 1)));
	if (this->_path_to_file == "/") {
        if (!server->getIndexFrom(this->_path_to_file).empty())
            setPathToFile(server->getIndexFrom(getPathToFile()));
    }
    if (getPathToFile().find('?') != std::string::npos) {
        setQuery(getPathToFile().substr(getPathToFile().find_last_of('?') + 1, getPathToFile().length()));
        setPathToFile(getPathToFile().substr(0, getPathToFile().find_first_of('?')));
    }
    setFileName(getPathToFile().substr(getPathToFile().find_last_of('/') + 1, getPathToFile().length()));
    defineFileType();

	//remove the first line (we already have the necessary information)
	str.erase(0, str.find_first_of('\n') + 1);

	first_space_index = str.find_first_of(' ');
	this->host = str.substr(first_space_index + 1, str.find_first_of('\n') - first_space_index);
    this->parseHeaders(str);
    //parse body
    size_t start = str.find("\r\n\r\n");
    if (start != std::string::npos) {
        this->_body = str.substr(start + 4);
        str.erase(start, str.length() - start);
    }
}

void Request::defineFileType() {
    this->setFileType(this->_path_to_file.substr(this->_path_to_file.find_first_of('.') + 1, this->_path_to_file.length()));
	if (this->file_type == "js")
        this->setFileType("javascript");
	this->extension = this->file_type;
	this->file_type.insert(0, MimeUtils::getTypeOfContent(this->file_type) + "/");
}

void Request::parseHeaders(const std::string& headers)
{
    size_t pos = headers.find("Content-Type: ");
    if (pos != std::string::npos) {
        std::string value = headers.substr(pos + 14);
        this->_contentType = value.substr(0, value.find_first_of(';'));
    }
    pos = headers.find("boundary=");
    if (pos != std::string::npos) {
        std::string value = headers.substr(pos + 9);
        this->_boundary = value.substr(0, value.find_first_of('\n'));
    }
    pos = headers.find("filename=");
    if (pos != std::string::npos) {
        std::string value = headers.substr(pos + 10);
        int i = 0;
        for(; value[i] != '"'; i++) {}
        this->_Postfilename = value.substr(0, i);
    }
}


void Request::tryAccess_Post(Server *server)
{
    std::string tester = server->getRootFrom(this->getPathToFile()) + this->subLocation(server->getLocationFrom(this->getPathToFile()));
    if (access(tester.c_str(), F_OK) != 0)
    {
        g_error = NOTFOUND;
        throw accessError();
    }

}

void Request::tryAccess_Delete(Server *server) {
    std::string tester = server->getRootFrom(this->getPathToFile()) + this->subLocation(server->getLocationFrom(this->getPathToFile()));
    if (access(tester.c_str(), F_OK) != 0)
    {
        g_error = NOTFOUND;
        throw accessError();
    }
    if (access(tester.c_str(), W_OK) != 0)
    {
        g_error = FORBIDDEN;
        throw accessError();
    }
    struct stat filestat;
    if (stat(tester.c_str(), &filestat) != 0) {
        throw requestError();
    } else {
        if (S_ISDIR(filestat.st_mode)) {
            DIR *dir = opendir(tester.c_str());
            if (!dir)
                throw requestError();
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                    closedir(dir);
                    g_error = DIRNOTEMPTY;
                    throw dirNotEmpty();
                }
            }
            closedir(dir);
        }
    }
}

void Request::tryAccess_Get(Server *server) {
    std::string tester = server->getRootFrom(this->getPathToFile()) + this->subLocation(server->getLocationFrom(this->getPathToFile()));

    if (access(tester.c_str(), F_OK) != 0)
    {
        g_error = NOTFOUND;
        throw accessError();
    }
    if (access(tester.c_str(), R_OK) != 0)
    {
        g_error = FORBIDDEN;
        throw accessError();
    }
    struct stat filestat;
    if (stat(tester.c_str(), &filestat) == 0) {
        if (S_ISDIR(filestat.st_mode)){
            if (this->_path_to_file[this->_path_to_file.size() - 1] != '/') {
                g_error = MISSINGSLASH;
                this->isDir = true;
                throw dirDoesNotEndWithSlash();
            }
            if (!server->getIndexFrom(this->_path_to_file).empty() &&
                erasesSidesChar(this->_path_to_file, '/') == erasesSidesChar(server->getPathFrom(this->_path_to_file), '/')) {
                this->_path_to_file += this->_path_to_file[this->_path_to_file.size() - 1] == '/' ? "" : "/";
                this->_path_to_file += server->getIndexFrom(this->_path_to_file);
                this->file_type = "text/html";
                return;
            }
            this->isDir = true;
            if (!server->getAutoindexFrom(this->_path_to_file)) {
                g_error = ISDIRECTORY;
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
        g_error = TOOLONGREQUEST;
        throw tooLongRequest();
    }
    if (isWellSlashed(this->_path_to_file) == -1) {
        g_error = INVALIDSLASH;
        throw invalidSlash();
    }
    if (DEBUG)
        std::cout << GREEN << "start check meths" << RESET << std::endl;
    std::vector<std::string> & methods = server->getAllowedMethodsFrom(this->_path_to_file);
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
        g_error = NOTALLOWEDMETHOD;
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
    return _path_to_file;
}

void Request::setPathToFile(const std::string &path_to_file) {
    this->_path_to_file = path_to_file;
}

const std::string &Request::getFileName() const {
    return _file_name;
}

void Request::setFileName(const std::string &file_name) {
    this->_file_name = file_name;
}

const std::string &Request::getQuery() const {
    return this->_query;
}

void Request::setQuery(const std::string &query) {
    this->_query = query;
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

const std::string &Request::getBody() const {
    return _body;
}

const std::string& Request::getPostFilename() const
{
    return _Postfilename;
}
