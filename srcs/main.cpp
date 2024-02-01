#include "../includes/Location.hpp"
#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "./webserv [configuration file]" << std::endl;
        return(1);
    }
    Server frr;
    frr.inputParsing(std::string(argv[1]));
}