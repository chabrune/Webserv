#include "../includes/Cgi.hpp"

Cgi::Cgi(Response &response, Request &request) {
    if (DEBUG)
        std::cout << "New Cgi is under building.." << std::endl;
    cgiBuilder(response, request);
    for (int i = 0; this->_env[i]; i++) {
        free(this->_env[i]);
    }
    delete []this->_env;
}

//env example
/*AUTH_TYPE=Basic
CONTENT_LENGTH=
CONTENT_TYPE=
DOCUMENT_ROOT=./
GATEWAY_INTERFACE=CGI/1.1
HTTP_COOKIE=
PATH_INFO=
PATH_TRANSLATED=.//
QUERY_STRING=
REDIRECT_STATUS=200
REMOTE_ADDR=localhost:8002
REQUEST_METHOD=GET
REQUEST_URI=/cgi-bin/acc.py
        SCRIPT_FILENAME=acc.py
SCRIPT_NAME=cgi-bin/acc.py
SERVER_NAME=localhost
SERVER_PORT=8002
SERVER_PROTOCOL=HTTP/1.1
SERVER_SOFTWARE=AMANIX*/

void Cgi::cgiBuilder(Response &response, const Request &request) {
    std::string runner = "/usr/bin/python3";
    std::string script = "experiment/expe_ali/site/testpy.py";
    std::string scriptname = "testpy.py";

    //argv builder
    char **argv = new char*[3];
    for (int i = 0; i < 3; i++)
        argv[i] = new char[50];
    strcpy(argv[0], runner.c_str());  // Path to runner
    strcpy(argv[1], script.c_str());  // Path to script
    argv[2] = 0;

    //env builder
    std::map<std::string, std::string> env_as_map;
    env_as_map["AUTH_TYPE="] = "Basic";
    env_as_map["CONTENT_LENGTH="] = "";
    env_as_map["CONTENT_TYPE="] = "";       //request.getFileType();
    env_as_map["GATEWAY_INTERFACE="] = "CGI/1.1";
    env_as_map["SCRIPT_NAME="] = script.c_str();
    env_as_map["SCRIPT_FILENAME="] = scriptname.c_str();
    //this->_env["PATH_TRANSLATED"] = it_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
    //this->_env["QUERY_STRING"] = decode(req.getQuery());
    //this->_env["REMOTE_ADDR"] = req.getHeader("host");
    env_as_map["SERVER_NAME="] = "localhost";
    env_as_map["SERVER_PORT="] = "8080";
    env_as_map["REQUEST_METHOD="] = request.getMethod();
    //this->_env["HTTP_COOKIE"] = req.getHeader("cookie");
    //this->_env["DOCUMENT_ROOT"] = it_loc->getRootLocation();
    //this->_env["REQUEST_URI"] = req.getPath() + req.getQuery();*
    env_as_map["SERVER_PROTOCOL="] = "HTTP/1.1";
    env_as_map["REDIRECT_STATUS="] = "200";
    env_as_map["SERVER_SOFTWARE="] = "AMANIX";

    this->_env = new char*[env_as_map.size() + 1];
    std::map<std::string, std::string>::const_iterator it = env_as_map.begin();
    for (int i = 0; it != env_as_map.end(); it++, i++) {
        std::string key_and_value = it->first + it->second;
        this->_env[i] = strdup(key_and_value.c_str());
    }
    this->_env[env_as_map.size()] = 0;

    pipeCreatorAndExec(argv);
    //chdir("experiment/expe_ali/site/");

    //readfrom pipe
    char t[1024];
    read(_pipe_out[0], t, 1024);
    response.setContent(t);
    closeAllPipe();
}

void Cgi::pipeCreatorAndExec(char **argv) {
    if (pipe(this->_pipe_out) < 0) {
        std::cout << "pipe1 marche po" << std::endl;
        exit(1);
    }
    if (pipe(this->_pipe_in) < 0) {
        std::cout << "pipe2 marche po" << std::endl;
        //todo
        exit(1);
    }
    _pid = fork();
    if (_pid < 0) {
        std::cout << "fork marche po" << std::endl;
        exit(1);
    }

    if (_pid == 0) {
        dup2(_pipe_in[0], STDIN_FILENO);
        dup2(_pipe_out[1], STDOUT_FILENO);
        closeAllPipe();
        exit(execve(argv[0], argv, this->_env));
    }
}

void Cgi::closeAllPipe() {
    close(_pipe_in[0]);
    close(_pipe_in[1]);
    close(_pipe_out[0]);
    close(_pipe_out[1]);
}