#include "../includes/Cgi.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response/AResponse.hpp"

Cgi::Cgi(AResponse &response, Request &request, Server &server, const std::string &buffer) {
    if (DEBUG)
        std::cout << MAGENTA << "New Cgi is under building.." << std::endl;
    cgiBuilder(request, server);
    if (DEBUG)
        std::cout << GREEN << "Cgi build. " << *this << std::endl;
    pipeCreatorAndExec(buffer);
    if (_exit_status != 0) {
        g_error = INTERNERROR;
        response.setGenerated(false);
        closeAllPipe();
        for (std::vector<const char *>::iterator it = this->_argv.begin(); it != _argv.end(); it++)
            free(const_cast<char*>(*it));
        for (std::vector<const char *>::iterator it = this->_env.begin(); it != _env.end(); it++)
            free(const_cast<char*>(*it));
        throw requestError();
    }
    readPipeValue(response, request);
    closeAllPipe();
    if (DEBUG)
        std::cout << "Cgi execution done" << std::endl;

    response.setGenerated(true);
}

void Cgi::cgiBuilder(Request &request, Server &server) {
    std::string path_full_name;
    path_full_name = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    path_full_name.erase(path_full_name.find_last_of('/'), path_full_name.length());
    setPathFullName(path_full_name);
    std::stringstream int_convertor;
    std::vector<const char *> argv;

    argv.push_back(strdup(server.getCgiPathFromExtension(request.getExtension()).c_str()));
    argv.push_back(strdup(request.getFileName().c_str()));
    argv.push_back(0);
    setArgv(argv);

    _env.push_back(strdup("AUTH_TYPE=Basic"));
    _env.push_back(strdup("CONTENT_LENGTH=3"));
    _env.push_back(strdup("CONTENT_TYPE=text/html"));
    _env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    _env.push_back(strdup(("SCRIPT_NAME=" + getPathFullName()).c_str()));
    _env.push_back(strdup(("QUERY_STRING=" + request.getQuery()).c_str()));
    _env.push_back(strdup(("SERVER_NAME=" + server.getServerName()).c_str()));
    int_convertor << server.getPort();
    _env.push_back(strdup(("SERVER_PORT=" + int_convertor.str()).c_str()));
    _env.push_back(strdup(("REQUEST_METHOD=" + request.getMethod()).c_str()));
    _env.push_back(strdup(("HTTP_COOKIE=" + request.getCookie()).c_str()));
    _env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
    _env.push_back(strdup("REDIRECT_STATUS=200"));
    _env.push_back(strdup("SERVER_SOFTWARE=AMANIX"));
    _env.push_back(0);
}

void Cgi::pipeCreatorAndExec(const std::string &buffer) {
    if (pipe(this->_pipe_out) < 0) {
        if (DEBUG)
            std::cout << "pipe1 marche po" << std::endl;
        g_error = INTERNERROR;
        throw requestError();
    }
    if (pipe(this->_pipe_in) < 0) {
        if (DEBUG)
            std::cout << "pipe2 marche po" << std::endl;
        close(_pipe_out[0]);
        close(_pipe_out[1]);
        g_error = INTERNERROR;
        throw requestError();
    }

    int ret = 0;
    pid_t worker_pid = fork();
    if (worker_pid == 0) {
        signal(SIGALRM, 0);
        alarm(4);

        this->_exit_status = chdir(getPathFullName().c_str());
        if (DEBUG)
            std::cout << "Chdir in folder " << getPathFullName() << " return result: " << this->_exit_status << std::endl;
        dup2(_pipe_in[0], STDIN_FILENO);
        dup2(_pipe_out[1], STDOUT_FILENO);
        if (!buffer.empty())
            write(_pipe_in[1], buffer.c_str(), buffer.length());
        closeAllPipe();
        ret = execve(this->_argv[0], const_cast<char **>(this->_argv.data()), const_cast<char **>(this->_env.data()));
        exit(1);
    }
    wait(&_exit_status);
    if (DEBUG)
        std::cout << "Cgi exec result " << this->_exit_status << std::endl;
}

void Cgi::readPipeValue(AResponse &response, Request &request) {
    std::string buffer;
    buffer.resize(1024);
    if (this->_exit_status < 0) {
        std::cout << "error" << std::endl;
        return;
    }
    read(_pipe_out[0], &buffer[0], 1024);
    if (buffer.find("Set-Cookie:") != std::string::npos)
        parseCookieFromCgi(buffer, request);
    size_t first_line_index = buffer.find_first_of('\n');
    std::string first_line = buffer.substr(0, first_line_index);
    if (first_line.find("Content-Type") == std::string::npos) {
        if (DEBUG)
            std::cout << RED << "Conent-Type in CGI not found, please respect convention and define it." << RESET << std::endl;
        request.setFileType("text/html");
    } else
        request.setFileType(first_line.substr(14, first_line_index - 14));
    buffer.erase(0, first_line_index + 1);
    response.setContent(buffer);
    response.setContentSize(buffer.size());
}

void Cgi::parseCookieFromCgi(std::string &buffer, Request &request) {
    size_t lineStart = buffer.find("Set-Cookie:");
    size_t lineEnd;
    do {
        lineEnd = buffer.find('\n', lineStart);
        request.addCookie("; " +  buffer.substr(lineStart + 12, lineEnd - (lineStart + 12)));
        buffer.erase(lineStart, lineEnd - lineStart);
    } while(!buffer.empty() && (lineStart = buffer.find("Set-Cookie:")) != std::string::npos);
}

void Cgi::closeAllPipe() {
    close(_pipe_in[0]);
    close(_pipe_in[1]);
    close(_pipe_out[0]);
    close(_pipe_out[1]);
}

const std::string &Cgi::getPathFullName() const {
    return this->_path_full_name;
}

void Cgi::setPathFullName(const std::string &path_full_name) {
    this->_path_full_name = path_full_name;
}

const std::vector<const char *> &Cgi::getArgv() const {
    return this->_argv;
}

void Cgi::setArgv(const std::vector<const char *> &argv) {
    this->_argv = argv;
}

std::ostream& operator<<(std::ostream& os, const Cgi& obj)
{
    os << "Cgi executor: " << obj.getArgv()[0] << " Cgi file executed: " << obj.getArgv()[1] << " Path Full Name: " <<
        obj.getPathFullName() << std::endl;
    return os;
}


Cgi::~Cgi() {
    for (std::vector<const char *>::iterator it = this->_argv.begin(); it != _argv.end(); it++)
        free(const_cast<char*>(*it));

    for (std::vector<const char *>::iterator it = this->_env.begin(); it != _env.end(); it++)
        free(const_cast<char*>(*it));
}