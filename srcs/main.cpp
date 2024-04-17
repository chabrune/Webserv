#include "../includes/Mommy.hpp"

Mommy frr;
Server Serv;

void quit(int sig) {
    (void)sig;
    FD_ZERO(&frr.lset);
    FD_ZERO(&frr.cset);
    for (std::vector<Server *>::iterator it = frr.servers.begin(); !frr.servers.empty();) {
        for (std::vector<Location *>::iterator itl = (*it)->getLocations().begin(); !(*it)->getLocations().empty();) {
            delete(*itl);
            (*it)->getLocations().erase(itl);
            itl = (*it)->getLocations().begin();
        }
        close((*it)->sockfd);
        delete (*it);
        frr.servers.erase(it);
        it = frr.servers.begin();
    }
    for (std::map<int, Client *>::iterator it = frr.clients.begin(); !frr.clients.empty();) {
        if (it->second) {
            int fdsave = it->second->sockfd;
            close(it->second->sockfd);
            delete(it->second);
            frr.clients.erase(fdsave);
            it = frr.clients.begin();
        }
    }
    if (sig != 0) {
        std::cout << YELLOW << "\n-server killed by user" << RESET << std::endl;
        std::exit(0);
    }
}

int main(int argc, char **argv)
{
    (void)argv;
    if(argc != 2)
    {
        std::cerr << "./webserv [configuration file]" << std::endl;
        return (1);
    }
    int ret = 0;
    signal(SIGINT, quit);
    signal(SIGQUIT, quit);
    signal(SIGPIPE, SIG_IGN);
    if (DEBUG)
        std::cout << GREEN << "-debug is on" << RESET << std::endl;
    try {
        Serv.inputParsing(std::string(argv[1]), frr);
        frr.run();
    } catch (std::exception &e) {
        std::cerr << RED << "error: " << e.what() << RESET << std::endl;
        ret = 1;
        quit(0);
    }
    return (ret);
}
