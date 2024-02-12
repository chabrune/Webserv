#include "../includes/Mommy.hpp"

Mommy::Mommy() {
    this->timeout.tv_sec = TIMEOUT;
    this->timeout.tv_usec = 0;
}

Mommy::~Mommy() {}

int Mommy::load_LFdSet(void) {
    int maxFd = -1;

    FD_ZERO(&this->lset);
    FD_ZERO(&this->cset);
    for (std::vector<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
        FD_SET((*it)->sockfd, &this->lset);
        if ((*it)->sockfd > maxFd)
            maxFd = (*it)->sockfd;
    }
    return (maxFd + 1);
}

Client * Mommy::acceptRequest(int fd, Server *server) {
    sockaddr_in cliInfo;
    memset(&cliInfo, 0, sizeof(cliInfo));
    socklen_t addrlen = sizeof(cliInfo);
    int cliFd = accept(fd, (sockaddr*)&cliInfo, &addrlen);
    if (cliFd == -1)
        throw acceptError();
    //fcntl(cliFd, F_SETFL, O_NONBLOCK);
    Client * cli = new Client(cliFd, cliInfo, server);
    this->clients[cliFd] = cli;
    std::cout << YELLOW << "-new connexion from /" << inet_ntoa(cliInfo.sin_addr) << ":" << (int)ntohs(cliInfo.sin_port) << "\\ accepted" << RESET << std::endl;
    return cli;
}

void Mommy::run(void) {

    while (1) {
        int sval = 0;

        while (sval == 0) {
            int maxFd = load_LFdSet();
            sval = select(maxFd, &this->lset, &this->cset, NULL, &this->timeout);
        }
        if (sval == -1) {
            throw selectError();
        } else {
            for (std::vector<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
                if (FD_ISSET((*it)->sockfd, &this->lset)) {
                    try {
                        Client * cli = acceptRequest((*it)->sockfd, *it);
                        cli->readRequest();
                        cli->req.parseRequest();
                        FD_SET(cli->sockfd, &this->cset);
                    } catch (std::exception &e) {
                        std::cerr << RED << "error: " << e.what() << RESET << std::endl;
                    }
                }
            }
            for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
                if (FD_ISSET(it->second->sockfd, &this->cset)) {
                    try {
                        try {
                            it->second->sendResponse();
                        } catch (std::exception &e) {
                            std::cerr << RED << "error: " << e.what() << RESET << std::endl;
                        }
                        if (close(it->second->sockfd) == -1)
                            std::cerr << RED << "error: failed to close fd after sending response" << RESET << std::endl;
                        FD_CLR(it->second->sockfd, &this->cset);
                        FD_CLR(it->second->sockfd, &this->lset);
                        this->toDelete.push_back(it->first);
                    } catch (std::exception &e) {
                        std::cerr << "error: connection received but failed" << std::endl;
                    }
                }
            }
            while (!this->toDelete.empty()) {
                std::map<int, Client*>::iterator it = this->clients.find(this->toDelete.front());
                if (it == this->clients.end())
                    continue;
                delete it->second;
                this->clients.erase(it);
                this->toDelete.erase(this->toDelete.begin());
            }
        }
    }
}
