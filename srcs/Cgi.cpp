#include "../includes/Cgi.hpp"
#include "../includes/Server.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response/AResponse.hpp"

Cgi::Cgi(AResponse &response, Request &request, Server &server) {
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
    this->_path_full_name = server.getRootFrom(request.getPathToFile());
    std::stringstream int_convertor;

    this->_argv.push_back(strdup(server.getCgiPathFromExtension(request.getExtension()).c_str()));
    this->_argv.push_back(strdup(request.getFileName().c_str()));
    this->_argv.push_back(0);

    _env.push_back(strdup("AUTH_TYPE=Basic"));
    _env.push_back(strdup("CONTENT_LENGTH="));
    _env.push_back(strdup("CONTENT_TYPE="));
    _env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    _env.push_back(strdup(("SCRIPT_NAME=" + this->_path_full_name).c_str()));
    //this->_env["PATH_TRANSLATED"] = it_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
    //this->_env["QUERY_STRING"] = decode(req.getQuery());
    //this->_env["REMOTE_ADDR"] = //ip du client
    _env.push_back(strdup(("SERVER_NAME=" + server.getServerName()).c_str()));
    int_convertor << server.getPort();
    _env.push_back(strdup(("SERVER_PORT=" + int_convertor.str()).c_str()));
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
        this->_exit_status = chdir(_path_full_name.c_str());
        if (DEBUG)
            std::cout << "Chdir in folder " << _path_full_name << " return result: " << this->_exit_status << std::endl;
        std::cout << this->_argv[0] << std::endl;
        dup2(_pipe_in[0], STDIN_FILENO);
        dup2(_pipe_out[1], STDOUT_FILENO);
        closeAllPipe();
        execve(this->_argv[0], const_cast<char **>(this->_argv.data()), const_cast<char **>(this->_env.data()));
    }
    wait(&this->_exit_status);

}

void Cgi::readPipeValue(AResponse &response, Request &request) {
    std::string buffer;
    buffer.resize(1024);
    //char c;

    if (this->_exit_status < 0) {
        std::cout << "error" << std::endl;
        return;
    }
    read(_pipe_out[0], &buffer[0], 1024);
    /*while (read(_pipe_out[0], &c, 1) > 0) {
        buffer += c;
    }*/
    size_t first_line_index = buffer.find_first_of('\n');
    request.setFileType(buffer.substr(14, first_line_index - 14));
    buffer.erase(0, first_line_index + 1);
    response.setContent(buffer);
    response.setContentSize(buffer.size());
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