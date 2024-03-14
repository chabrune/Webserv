#pragma once

#include "incs.hpp"
#include "Response.hpp"

class Response;

class Cgi {
public:
    Cgi(Response &response, Request &request);
    ~Cgi();
private:
    void cgiBuilder(const Request &request);
    void pipeCreatorAndExec();
    void closeAllPipe();

    std::vector<const char *> _argv;
    std::vector<const char *> _env;
    int _pid;
    int _pipe_in[2]; //Used by server to send data to cgi
    int _pipe_out[2]; // Used by cgi to send data to server

};

