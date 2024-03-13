#pragma once

#include "incs.hpp"
#include "Response.hpp"

class Response;

class Cgi {
public:
    Cgi(Response &response, Request &request);
private:
    void cgiBuilder(Response &response, const Request &request);
    void pipeCreatorAndExec(char **argv);
    void closeAllPipe();

    char **_env;
    int _pid;
    int _pipe_in[2]; //Used by server to send data to cgi
    int _pipe_out[2]; // Used by cgi to send data to server

};

