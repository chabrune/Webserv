#include "../includes/Mommy.hpp"
#include "../includes/incs.hpp"
#include "../includes/Server.hpp"

Mommy frr; // Main structure

//Creating a test server while parsing isn't finished
void createTestServer(Mommy *frr) {
    frr->servers.push_back(new Server());
    frr->servers.back()->port = 8080;
    frr->servers.back()->server_name = "localhost";
    frr->servers.back()->autoindex = true;
    frr->servers.back()->setup();
}

void quit(int sig) {
    (void)sig;
    FD_ZERO(&frr.lset);
    FD_ZERO(&frr.cset);
    for (std::vector<Server *>::iterator it = frr.servers.begin(); !frr.servers.empty();) {
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
    std::cout << YELLOW << "\n-server killed by user" << RESET << std::endl;
    std::exit(0);
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
    if (DEBUG)
        std::cout << GREEN << "-debug is on" << RESET << std::endl;
    try {
        //frr.inputParsing(std::string(argv[1]));
        createTestServer(&frr);
        frr.run();
    } catch (std::exception &e) {
        std::cerr << "error: " << e.what() << std::endl;
        ret = 1;
    }
    return (ret);
}
