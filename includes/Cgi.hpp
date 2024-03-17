#pragma once

#include "incs.hpp"
#include "Response.hpp"

class Response;

class Cgi {
public:
    Cgi(Response &response, Request &request, const Server &server);
    ~Cgi();
private:
    void cgiBuilder(const Request &request, const Server &server);
    void pipeCreatorAndExec();
    void readPipeValue(Response &response, Request &request);
    void closeAllPipe();

    std::string _cgi_executor; //Example "/usr/bin/python3";
    std::string _script_path; //Example "experiment/expe_ali/site/my_file.py";
    std::string _script_name; //Example "my_file.py";

    std::vector<const char *> _argv;
    std::vector<const char *> _env;
    int _pid;
    int _pipe_in[2]; //Used by server to send data to cgi
    int _pipe_out[2]; // Used by cgi to send data to server
};

