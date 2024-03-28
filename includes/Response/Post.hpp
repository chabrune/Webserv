
#pragma once

#include "../incs.hpp"
#include "../utils.hpp"
#include "./AResponse.hpp"

class Post : public AResponse {
    public:
        Post() {};
        Post(Server & server);
        void doSomeThings(std::string & buffer, Request &request);
        void doThingsAndLetsSeeWhatHappenMaybeItWillWorkMaybeNotWeWillSeeLetsPrayTogetherAndMakeLoveNotWar___amen(Server & server, Request &request, bool & readyToSend);
    private:
        bool done;
        bool processing;
        std::string _filename;
        std::string _filePath;
};