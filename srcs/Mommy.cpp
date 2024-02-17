#include "../includes/Mommy.hpp"

Mommy::Mommy() {
    this->timeout.tv_sec = TIMEOUT;
    this->timeout.tv_usec = 0;
    this->running = true;
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
    Client * cli = NULL;
    cli = new Client(cliFd, cliInfo, server);
    if (!cli)
        throw std::bad_alloc();
    this->clients[cliFd] = cli;
    std::cout << BLUE << "-new connexion from "  << *cli << RESET << std::endl;
    return cli;
}

void Mommy::run(void) {
	Request request;

    while (this->running) {
        int maxFd = load_LFdSet();
        int sval = select(maxFd, &this->lset, &this->cset, NULL, &this->timeout);
        if (sval == -1) {
            if (this->running)
                throw selectError();
            this->running = false;
        } else {
            for (std::vector<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
                if (FD_ISSET((*it)->sockfd, &this->lset)) {
                    Client *cli;
                    try {
                        cli = acceptRequest((*it)->sockfd, *it);
						request = Request(cli->sockfd);
                        /*cli->readRequest();
                        cli->req.parseRequest();*/
                        FD_SET(cli->sockfd, &this->cset);
                    } catch (Request::tooLongRequest &e) {
                        std::cerr << RED << "ALLLLEEEEEEEERRRRRRRRRRRTTTTTTTTTTT"<< RESET << std::endl;
                    } catch (std::exception &e) {
                        if (cli) {
                            FD_CLR((*it)->sockfd, &this->lset);
                            this->clients.erase(cli->sockfd);
                            close(cli->sockfd);
                            delete cli;
                        }
                        if (DEBUG)
                            std::cerr << YELLOW << e.what() << RESET << std::endl;
                    }
                }
            }
            for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
                if (FD_ISSET(it->second->sockfd, &this->cset)) {
                    try {
                        try {
							Response response(request.getPathToFile(), request.getFileType());
							send(it->second->sockfd, &(response.getResponse()[0]), response.getResponseSize(), 0);
                            //it->second->sendResponse();
                        } catch (std::exception &e) {
                            if (DEBUG)
                                std::cerr << RED << "error: " << e.what() << RESET << std::endl;
                        }
                        if (close(it->second->sockfd) == -1)
                            std::cerr << RED << "error: failed to close fd after sending response" << RESET << std::endl;
                        std::cout << BLUE << "-" << *(it->second) << " connexion closed" << RESET << std::endl;
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
