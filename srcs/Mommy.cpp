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

void Mommy::acceptRequest(int fd, Server *server) {
    sockaddr_in cliInfo;
    memset(&cliInfo, 0, sizeof(cliInfo));
    socklen_t addrlen = sizeof(cliInfo);
    int cliFd = accept(fd, (sockaddr*)&cliInfo, &addrlen);
    if (cliFd == -1)
        throw acceptError();
    //fcntl(cliFd, F_SETFL, O_NONBLOCK);
    Client * cli = new Client(cliFd, cliInfo, server);
    this->clients.push_back(cli);
    std::cout << YELLOW << "-new connexion from /" << inet_ntoa(cliInfo.sin_addr) << ":" << (int)ntohs(cliInfo.sin_port) << "\\ accepted" << RESET << std::endl;
}

void Mommy::readRequest(Client *client) {
    char buffer[HTTP_BUFFER_SIZE] = {0};
    long len = recv(client->sockfd, buffer, HTTP_BUFFER_SIZE - 1, 0) ;
    if (len == -1)
        perror("");
    std::cout << BLUE << buffer << std::endl << len << RESET << std::endl;
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
                        acceptRequest((*it)->sockfd, *it);
                        Client *cli = this->clients.back();
                        readRequest(this->clients.back());
                        FD_SET(cli->sockfd, &this->cset);
                    } catch (std::exception &e) {
                        std::cerr << "error: connection received but failed" << std::endl;
                    }
                }
            }
            for (std::vector<Client*>::iterator it = this->clients.begin(); it != this->clients.end();) {
                if (FD_ISSET((*it)->sockfd, &this->cset)) {
                    try {
                        char buff[] = "bonjour";
                        send((*it)->sockfd, buff, 7, 0);
                        close((*it)->sockfd);
                        FD_CLR((*it)->sockfd, &this->cset);
                        delete *it;
                        it = this->clients.erase(it);
                    } catch (std::exception &e) {
                        std::cerr << "error: connection received but failed" << std::endl;
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }
    }
}
