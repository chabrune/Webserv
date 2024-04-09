#include "../../includes/Response/AResponse.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/Server.hpp"

defaultErrorCodes __defaultErrorCodes;
defaultErrorPages __defaultErrorPages;

AResponse::AResponse() : _contentFile(0), _isGenerated(false) {}
AResponse::AResponse(Server & server) : server(&server), _contentFile(0), _isGenerated(false), _isPosting(false)  {
    if (DEBUG)
	    std::cout << "New response is under building.." << std::endl;
}

void AResponse::defaultFileBuilder(const Request &request) {
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(this->_uri.c_str(), MimeUtils::getOpenMode(request.getExtension()));
    if (!this->_contentFile->is_open() && DEBUG) {
        std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
        return;
    }
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
}

void AResponse::headerFileBuilder(std::string file_type, Request &request) {
    std::stringstream header_tmp;
    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_contentSize << "\r\n";
    std::string cookie_line = request.getCookie();
    while (!cookie_line.empty()) {
        size_t equalsChar = cookie_line.find('=');
        size_t colonChar = cookie_line.find(';');
        if (colonChar == std::string::npos)
            colonChar = cookie_line.length();
        header_tmp << "Set-Cookie: " << cookie_line.substr(0, equalsChar + 1) << cookie_line.substr(equalsChar + 1, colonChar - equalsChar) << "\r\n";
        cookie_line.erase(0, colonChar + 2);
    }
    header_tmp << "\r\n";
    this->_header = header_tmp.str();
}

void AResponse::headerGenBuilder(std::string file_type) {
    std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.size() << "\n\n";
    this->_header = header_tmp.str();
}

std::string AResponse::getCodeHeader(std::string * path, Server* server,  const std::string & uri) {
    if (path)
        *path = server->getRootFrom(uri) + "/";
    if (g_error == MISSINGSLASH || g_error == INVALIDSLASH) {
        return ("HTTP/1.1 301 Moved Permanently\n");
    } else if (g_error == TOOLARGEENTITY) {
        try {
            *path += server->getErrorPage(413, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[413];
        }
        return ("HTTP/1.1 413 Request Entity Too Large\n");
    } else if (g_error == FORBIDDEN) {
        try {
            *path += server->getErrorPage(403, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[403];
        }
        return ("HTTP/1.1 403 Forbidden\n");
    } else if (g_error == TOOLONGREQUEST) {
        try {
            *path += server->getErrorPage(414, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[414];
        }
        return ("HTTP/1.1 414 Uri Too Long\n");
    } else if (g_error == NOTFOUND) {
        try {
            *path += server->getErrorPage(404, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[404];
        }
        return ("HTTP/1.1 404 Not Found\n");
    } else if (g_error == ISDIRECTORY || g_error == BADHEADER || g_error == BADREQUEST) {
        try {
            *path += server->getErrorPage(400, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[400];
        }
        return ("HTTP/1.1 400 Bad Request\n");
    }else if (g_error == CONFLICT) {
        try {
            *path += server->getErrorPage(409, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[409];
        }
        return ("HTTP/1.1 409 Conflict\n");
    }else if (g_error == NOTALLOWEDMETHOD || g_error == DIRNOTEMPTY) {
        try {
            *path += server->getErrorPage(405, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[405];
        }
        return ("HTTP/1.1 405 Method Not Allowed\n");
    } else {
        try {
            *path += server->getErrorPage(500, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[500];
        }
        return ("HTTP/1.1 500 Internal Server Error\n");
    }
}

void AResponse::redirectWellSlashed(const std::string & uri) {
    std::string newuri = uri;
    this->_header = getCodeHeader(0, 0, uri);
    if (g_error == INVALIDSLASH) {
        size_t i = 0;
        while (i + 1 < newuri.size()) {
            if (newuri[i] == '/' && newuri[i + 1] == '/') {
                newuri.erase(i, 1);
            } else {
                i++;
            }
        }
    } else {
        newuri += "/";
    }
    this->_header += ("Location: " + newuri + "\n");
    this->_isGenerated = true;
}

void AResponse::handleRequestError(Server* server, const std::string & uri) {
    std::stringstream tmphead;
    std::string codePath;

    if (DEBUG)
        std::cout << RED << "Sending error code, reason: " << g_error << RESET << std::endl;
    if (g_error == MISSINGSLASH || g_error == INVALIDSLASH) {
        redirectWellSlashed(uri);
        return;
    }
    tmphead << getCodeHeader(&codePath, server, uri);
    if (this->_contentFile)
        delete this->_contentFile;
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(codePath.c_str(),std::ifstream::in);
    if (!this->_contentFile->is_open()) {
        if (DEBUG)
            std::cerr << RED << "error: can't access error page" << RESET << std::endl;
        this->_header = tmphead.str() + "Content-Length: 0\n\r\n\r\n";
        return;
    }
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
    tmphead << "Content-Length: " << this->_contentSize << "\nContent-Type: text/html\n\n";
    this->_header = tmphead.str();
}

std::string & AResponse::getContent() {
    return this->_content;
}

void AResponse::setContent(const std::string &content) {
    this->_content = content;
}

void AResponse::setContentSize(long long size) {
    this->_contentSize = size;
}

std::string &AResponse::getUri() {
    return this->_uri;
}

std::string &AResponse::getHeader() {
    return this->_header;
}

bool AResponse::getGenerated() {
    return this->_isGenerated;
}

std::ifstream * AResponse::getContentFile() {
    return this->_contentFile;
}

long long AResponse::getContentSize() {
    return this->_contentSize;
}

bool & AResponse::modIsPosting() {
    return this->_isPosting;
}

const std::string &AResponse::getHeaderCode() {
    return this->_header_code;
}

void AResponse::setHeaderCode(const std::string &header_code) {
    this->_header_code = header_code;
}