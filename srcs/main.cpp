#include "../includes/Mommy.hpp"
#include "../includes/incs.hpp"
#include "../includes/Server.hpp"
#include "../includes/Location.hpp"

Mommy frr; // Main structure
Server Serv; //Charles test parsing

//Creating a test server while parsing isn't finished
void createTestServer(Mommy *frr) {
    // frr->servers.push_back(new Server());
    // frr->servers.back()->port = 8080;
    // frr->servers.back()->server_name = "localhost";
    // frr->servers.back()->root = "experiment/expe_ali/site"; // la ya pas de /
    frr->servers.back()->index = "/index.html"; // et la yen a un bref a regler psq cest degeulasse
    frr->servers.back()->autoindex = true;
    frr->servers.back()->allowed_methods.push_back("GET");
    frr->servers.back()->setup();
    frr->servers.back()->locations.push_back(Location());
    frr->servers.back()->locations.back().path = "/test";
    frr->servers.back()->locations.back().allowed_methods.push_back("GET");
    frr->servers.back()->locations.back().root = "experiment/expe_ali/site/test";
    frr->servers.back()->locations.back().autoindex = true;
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
    if (DEBUG)
        std::cout << GREEN << "-debug is on" << RESET << std::endl;
    try {
        Serv.inputParsing(std::string(argv[1]), frr);
        createTestServer(&frr);
        frr.run();
    } catch (std::exception &e) {
        std::cerr << RED << "error: " << e.what() << RESET << std::endl;
        ret = 1;
        quit(0);
    }
    return (ret);
}
