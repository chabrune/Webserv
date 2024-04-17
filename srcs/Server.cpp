#include "../includes/Server.hpp"

Server::Server() {}

Server::~Server() {}

Server& Server::operator=(const Server &rhs)
{
    (void)rhs;
    return(*this);
}

Server::Server(const Server &src) : ServerConf()
{
    (void)src;
}

void Server::setup(void)
{
    memset(&this->srvaddress, 0, sizeof(this->srvaddress));
    this->srvaddress.sin_family = AF_INET;
    this->srvaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->srvaddress.sin_port = htons(getPort());
    this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw socketCreationError();
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(int)) == -1)
        throw socketConfigError();
    this->timeout.tv_sec = 0;
    this->timeout.tv_usec = TIMEOUT;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketConfigError();
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&this->timeout, sizeof(this->timeout)) == -1)
        throw socketConfigError();
    if (bind(this->sockfd, (struct sockaddr *)&this->srvaddress, sizeof(this->srvaddress)) == -1)
        throw addrBindError();
    if (listen(this->sockfd, CONNECTIONS_NUMBER_LIMIT) == -1)
        throw sockListeningError();
    fcntl(this->sockfd, SO_SNDTIMEO);
    fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
    std::cout << GREEN << "-listener socket for " << BLUE << getServerName() << ":" << getPort() << GREEN << " ready" << RESET << std::endl;
}

Location * Server::getLocationFrom(const std::string &path) {
    Location * res = NULL;
    for (std::vector<Location*>::iterator it = this->locations.begin(); it != this->locations.end(); it++) {
        std::string itpath = (*it)->path;
        std::string tmppath = path;
        if (tmppath.length() > 1)
            tmppath[tmppath.length() - 1] == '/' ? tmppath : tmppath += "/";
        if (itpath.length() > 1)
            itpath[itpath.length() - 1] == '/' ? itpath : itpath += "/";
        if (tmppath.length() >= itpath.length() && tmppath.compare(0, itpath.length(), itpath) == 0) {
            if (!res || itpath.size() > res->path.size())
                res = *it;
        }
    }
    return (res);
}

std::string & Server::getRootFrom(const std::string & path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->root);
    }
    return (this->root);
}

bool Server::getAutoindexFrom(const std::string & path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->autoindex);
    }
    return (this->autoindex);
}

std::string & Server::getIndexFrom(const std::string & path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->index);
    }
    return (this->index);
}

std::vector<std::string> & Server::getAllowedMethodsFrom(const std::string &path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->allowed_methods);
    }
    return (this->allowed_methods);
}

std::string Server::getErrorPage(int err, const std::string & path) {
    std::string res;
    Location * location = getLocationFrom(path);
    if (location && location->errors_pages.find(err) != location->errors_pages.end()) {
        res = location->errors_pages[err];
        return res;
    }
    if (this->errors_pages.find(err) != this->errors_pages.end()) {
        res = this->errors_pages[err];
        return res;
    } else {
        throw std::exception();
    }
}

std::string Server::getPathFrom(const std::string & path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->path);
    }
    return ("/");
}

unsigned long Server::getMaxBodySizeFrom(const std::string &path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->max_body_size);
    }
    return (this->max_body_size);
}

std::string Server::getUploadFolderFrom(const std::string &path) {
    Location * location = getLocationFrom(path);
    if (location) {
        return (location->upload_path);
    }
    return (this->upload_path);
}