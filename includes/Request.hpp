#pragma once

#include "incs.hpp"

class Request {
    public:
        Request();
        ~Request();

        long len;
        std::string content; // contain the request

        int Method;

        void parseRequest();
};