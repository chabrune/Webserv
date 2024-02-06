#include "../includes/Location.hpp"
#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "./webserv [configuration file]" << std::endl;
        return (1);
    }

    Mommy frr;
    try {
        frr.inputParsing(std::string(argv[1]));
    } catch (std::excpetion &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}