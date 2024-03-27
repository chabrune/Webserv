
#pragma once

#include "../incs.hpp"
#include "../utils.hpp"
#include "./AResponse.hpp"

class Post : public AResponse {
    public:
        Post(Server & server);
        void doThingsAndLetsSeeWhatHappenMaybeItWillWorkMaybeNotWeWillSeeLetsPrayTogetherAndMakeLoveNotWar___amen(Server & server, Request &request, bool & readyToSend);
    private:
        bool chunked;
        bool done;
        bool processing;
};