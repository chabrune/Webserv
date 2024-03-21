
#pragma once

#include "../incs.hpp"
#include "../utils.hpp"
#include "./AResponse.hpp"
#include "../Request.hpp"

class Delete : public AResponse {
    public:
        Delete(Server & server, Request &request);

    private:
        void doDel();
};