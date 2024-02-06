#include "../includes/Mommy.hpp"

Mommy::Mommy() {
    this->timeout.tv_sec = TIMEOUT;
    this->timeout.tv_usec = 0;
}

~Mommy::Mommy() {}

int Mommy::fillFdSet(void) {
    int maxFd = -1;

    FD_ZERO(this->lset);
    for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
        FD_SET(it->sockfd, &this->lset);
        if (it->sockfd > maxFd)
            maxFd = it->sockfd;
    }
    return (maxFd + 1);
}

void Mommy::run(void) {
    while (1) {
        int maxFd = fillFdSet();
        if (select(maxFd, &this->lset, &this->cset, NULL, &this->timeout) == -1)
            throw selectError();
        for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
            if (FD_ISSET(this->lset, it->sockfd)) {
                acceptrequest(this->lset);
            }
        }
    }
}

