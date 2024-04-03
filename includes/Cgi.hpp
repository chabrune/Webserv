#pragma once

#include "incs.hpp"
#include "Server.hpp"

class Request;
class Server;
class AResponse;
class Get;
class Post;

class Cgi {
public:
    Cgi(AResponse &response, Request &request, Server &server, const std::string &buffer);
    ~Cgi();

    const std::string &getPathFullName() const;
    void setPathFullName(const std::string &path_full_name);
    const std::vector<const char *> &getArgv() const;
    void setArgv(const std::vector<const char *> &argv);

private:
    void cgiBuilder(Request &request, Server &server);
    void pipeCreatorAndExec(const std::string &buffer);
    void readPipeValue(AResponse &response, Request &request);
    void parseCookieFromCgi(std::string &buffer, Request &request);
    void closeAllPipe();

    std::string _path_full_name;
    int _exit_status;

    std::vector<const char *> _argv;
    std::vector<const char *> _env;
    int _pid;
    int _pipe_in[2]; //Used by server to send data to cgi
    int _pipe_out[2]; // Used by cgi to send data to server
};

std::ostream& operator<<(std::ostream& os, const Cgi& obj);

