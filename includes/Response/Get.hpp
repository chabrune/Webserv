
#pragma once

#include "../incs.hpp"
#include "../utils.hpp"
#include "./AResponse.hpp"

class Get : public AResponse {
    public:
        Get(Server & server, Request &request);

    private:
        void headerFileBuilder(std::string file_type);
        bool findServerStatusCode(std::map<unsigned int, std::string>::iterator itf);
        bool findReturnLocations(Server* server);
        bool findLocationStatusCode(Server *server, std::string ptf);
        bool handleReturn(Server *server, Request& request);
        void generateAutoindex(Request & req);
};