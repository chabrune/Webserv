#include "../includes/Mommy.hpp"

Mommy::Mommy() {
    this->timeout.tv_sec = TIMEOUT;
    this->timeout.tv_usec = 0;
}

Mommy::~Mommy() {}

int Mommy::load_LFdSet(void) {
    int maxFd = -1;

    FD_ZERO(&this->lset);
    for (std::vector<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
        FD_SET((*it)->sockfd, &this->lset);
        if ((*it)->sockfd > maxFd)
            maxFd = (*it)->sockfd;
    }
    return (maxFd + 1);
}

void Mommy::acceptRequest(int fd) {
    sockaddr_in cliInfo;
    memset(&cliInfo, 0, sizeof(cliInfo));
    socklen_t addrlen = sizeof(cliInfo);
    int cliFd = accept(fd, (sockaddr*)&cliInfo, &addrlen);
    if (cliFd == -1)
        throw acceptError();
    fcntl(cliFd, F_SETFL, O_NONBLOCK);
    Client * cli = new Client(cliFd, cliInfo);
    std::cout << YELLOW << "-new connexion from /" << inet_ntoa(cliInfo.sin_addr) << ":" << (int)ntohs(cliInfo.sin_port) << "\\ accepted" << RESET << std::endl;

    std::string msg = "bonjour";
    send(cliFd, msg.c_str(), strlen(msg.c_str()), 0);
    close(cliFd);
    delete(cli);
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
                        acceptRequest((*it)->sockfd);
                    } catch (std::exception &e) {
                        std::cerr << "error: connection received but failed" << std::endl;
                    }
                }
            }
        }
    }
}
