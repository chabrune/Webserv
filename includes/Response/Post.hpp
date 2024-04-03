
#pragma once

#include "../incs.hpp"
#include "../utils.hpp"
#include "./AResponse.hpp"

class Post : public AResponse {
    public:
        Post() {};
        Post(Server & server);
        void treatBuffer(std::string & buffer, Request &request);
        void execPost(Server & server, Request &request, bool & readyToSend);
    private:
        bool done;
        bool processing;
        std::string _filename;
        std::string _filePath;
};