#include "../includes/Mommy.hpp"

//Creating a test server while parsing isn't finished
void createTestServer(Mommy *frr) {
    frr->servers.push_back(new Server());
    frr->servers.back()->port = 8080;
    frr->servers.back()->server_name = "localhost";
    frr->servers.back()->setup();
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "./webserv [configuration file]" << std::endl;
        return (1);
    }

    Mommy frr;
    try {
        //frr.inputParsing(std::string(argv[1]));
        createTestServer(&frr);
        frr.run();
    } catch (std::exception &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return (1);
    }
    return (0);
}