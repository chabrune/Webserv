#pragma once

#include "ServerConf.hpp"

class Server : public ServerConf
{
    public:
        Server();
        ~Server();
        Server(const Server &src);
        Server& operator=(const Server &rhs);

        int sockfd;
        sockaddr_in srvaddress; // Server address
        timeval timeout;

        void setup();
        Location * getLocationFrom(const std::string & path);
        std::vector<std::string> & getAllowedMethodsFrom(const std::string & path);
        std::string & getRootFrom(const std::string & path);
        bool getAutoindexFrom(const std::string & path);
        std::string & getIndexFrom(const std::string & path);
        std::string getErrorPage(int err, const std::string & path);
        std::string getPathFrom(const std::string & path);
        std::string getUploadFolderFrom(const std::string & path);
        unsigned long getMaxBodySizeFrom(const std::string & path);

        class socketCreationError : public std::exception {
            const char * what() const throw() {
                return ("socket creation failed");
            }
        };
        class socketConfigError : public std::exception {
            const char * what() const throw() {
                return ("socket configuration failed");
            }
        };
        class addrBindError : public std::exception {
            const char * what() const throw() {
                return ("socket binding failed");
            }
        };
        class sockListeningError : public std::exception {
            const char * what() const throw() {
                return ("socket listening failed");
            }
        };
};
