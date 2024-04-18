
#include "../../includes/Response/Delete.hpp"

Delete::Delete(Server &server, Request &request) : AResponse(server) {
        try {
            request.tryAccess_Delete(&server);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
            this->_uri = removeConsecutivesSlash(_uri);
            this->doDel();
        } catch (std::exception &e) {
            throw ;
        }
}

void Delete::doDel() {
    if (std::remove(this->_uri.c_str()) != 0) {
        perror("");
        throw requestError();
    }
    this->_header = "HTTP/1.1 200 OK\n\r\n\r\n";
    this->_isGenerated = true;
}