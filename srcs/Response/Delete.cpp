
#include "../../includes/Response/Delete.hpp"
#include <unistd.h>

Delete::Delete(Server &server, Request &request) : AResponse(server) {
        try {
            request.tryAccess_Delete(&server);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
            this->doDel();
        } catch (std::exception &e) {
            throw ;
        }
}

void Delete::doDel() {
    int pid = fork();
    int w;
    if (pid == 0) {
        const char *argv[] = { "rm", "-rf", this->_uri.c_str(), NULL};
        execve("/bin/rm", const_cast<char *const *>(argv), NULL);
    } else {
        wait(&w);
    }
    this->_header = "HTTP/1.1 200 OK\n\r\n\r\n";
    this->_isGenerated = true;
}