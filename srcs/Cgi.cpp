#include "../includes/Cgi.hpp"

Cgi::Cgi(Response &response, Request &request, Server &server) {
    if (DEBUG)
        std::cout << "New Cgi is under building.." << std::endl;
    cgiBuilder(request, server);
    pipeCreatorAndExec();
    readPipeValue(response, request);
    closeAllPipe();
    if (DEBUG)
        std::cout << "Successfully cgi execution." << std::endl;
}

void Cgi::cgiBuilder(const Request &request, Server &server) {
    const std::string &runner = server.getCgiPathFromExtension(request.getExtension());
    std::string script = "experiment/expe_ali/site/acc.py";
    this->_script_name = request.getPathToFile().substr(1, request.getPathToFile().size());
    std::stringstream intConvertor;

    this->_argv.push_back(strdup(runner.c_str()));
    this->_argv.push_back(strdup(this->_script_name.c_str()));
    this->_argv.push_back(0);

    _env.push_back(strdup("AUTH_TYPE=Basic"));
    _env.push_back(strdup("CONTENT_LENGTH="));
    _env.push_back(strdup("CONTENT_TYPE="));
    _env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    _env.push_back(strdup(("SCRIPT_NAME=" + script).c_str()));
    //this->_env["PATH_TRANSLATED"] = it_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
    //this->_env["QUERY_STRING"] = decode(req.getQuery());
    //this->_env["REMOTE_ADDR"] = //ip du client
    _env.push_back(strdup(("SERVER_NAME=" + server.getServerName()).c_str()));
    intConvertor << server.getPort();
    _env.push_back(strdup(("SERVER_PORT=" + intConvertor.str()).c_str()));
    _env.push_back(strdup(("REQUEST_METHOD=" + request.getMethod()).c_str()));
    //this->_env["HTTP_COOKIE"] = req.getHeader("cookie");
    //this->_env["REQUEST_URI"] = req.getPath() + req.getQuery();*
    _env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
    _env.push_back(strdup("REDIRECT_STATUS=200"));
    _env.push_back(strdup("SERVER_SOFTWARE=AMANIX"));
    _env.push_back(0);
}

void Cgi::pipeCreatorAndExec() {
    if (pipe(this->_pipe_out) < 0) {
        std::cout << "pipe1 marche po" << std::endl;
        exit(1);
    }
    if (pipe(this->_pipe_in) < 0) {
        std::cout << "pipe2 marche po" << std::endl;
        close(_pipe_out[0]);
        close(_pipe_out[1]);
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
        chdir("experiment/expe_ali/site/");
        exit(execve(this->_argv[0], const_cast<char **>(this->_argv.data()), const_cast<char **>(this->_env.data())));
    }
    wait(0);
}

void Cgi::readPipeValue(Response &response, Request &request) {
    std::string buffer;
    buffer.resize(1024);
    //char c;

    read(_pipe_out[0], &buffer[0], 1024);
    /*while (read(_pipe_out[0], &c, 1) > 0) {
        buffer += c;
    }*/
    size_t first_line_index = buffer.find_first_of('\n');
    request.setFileType(buffer.substr(14, first_line_index - 14));
    buffer.erase(0, first_line_index + 1);
    response.setContent(buffer);
    response._contentSize = buffer.size();
}

void Cgi::closeAllPipe() {
    close(_pipe_in[0]);
    close(_pipe_in[1]);
    close(_pipe_out[0]);
    close(_pipe_out[1]);
}

Cgi::~Cgi() {
    for (std::vector<const char *>::iterator it = this->_argv.begin(); it != _argv.end(); it++)
        free(const_cast<char*>(*it));

    for (std::vector<const char *>::iterator it = this->_env.begin(); it != _env.end(); it++)
        free(const_cast<char*>(*it));
}