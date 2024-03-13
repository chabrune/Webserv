#include "../includes/Response.hpp"

defaultErrorPages __defaultErrorPages;
defaultErrorCodes __defaultErrorCodes;

Response::Response() : _contentFile(0), _isGenerated(false) {}
Response::Response(Server & server, Request &request) : _contentFile(0), server(&server), _isGenerated(false) {
    if (DEBUG)
	    std::cout << "New response is under building.." << std::endl;
    std::string tester = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    this->_uri = tester.c_str();
    if(!server.to_return.empty() || findReturnLocations(&server))
    {
        if(handleReturn(&server, request))
            return;
    }
    if (request.getIsDir()) {
        generateAutoindex(request);
        return;
    }
	if (isCgi(request.getFileType())) {
        cgiBuilder(request);
	}

    this->_contentFile = new std::ifstream;
	this->_contentFile->open(tester.c_str(), MimeUtils::getOpenMode(request.getExtension()));
	if (!this->_contentFile->is_open()) {
		std::cerr << RED << "Error: " << strerror(errno) << RESET << std::endl;
		return;
	}
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);

	headerFileBuilder(request.getFileType());
    if (DEBUG)
	    std::cout << "Response created. Header:" << std::endl << this->_header;
}

bool Response::findServerStatusCode(std::map<unsigned int, std::string>::iterator itf)
{
    std::map<unsigned int, std::string>::iterator it = __defaultErrorCodes.errorCodes.find(itf->first);
    return it != __defaultErrorCodes.errorCodes.end();
}

//Jpense que ca n'ira pas voir pour modif
bool Response::findReturnLocations(Server* server)
{
    std::vector<Location*>::iterator itl = server->locations.begin();
    for(; itl != server->locations.end(); itl++)
    {
        if(!(*itl)->to_return.empty())
            return true;
    }
    return false;
}


bool Response::findLocationStatusCode(Server *server, std::string ptf)
{
    bool uri, found;
    std::vector<Location*>::iterator itl = server->locations.begin();
    for(; itl != server->locations.end(); itl++)
    {
        std::map<unsigned int, std::string>::iterator it = (*itl)->to_return.begin();
        for (; it != (*itl)->to_return.end(); it++) 
        {
            std::map<unsigned int, std::string>::iterator it2 = __defaultErrorCodes.errorCodes.find(it->first);
            if (it2 != __defaultErrorCodes.errorCodes.end())
                found = true;
            if(!(*itl)->path.empty())
                if(ptf.find((*itl)->path) == 0)
                    uri = true;
        } 
    }
    return (uri && found);
}

bool Response::handleReturn(Server *server, Request& request)
{
    std::string ptf = request.getPathToFile();
    if (findServerStatusCode(server->to_return.begin())) 
    {
        this->_isGenerated = true;
        std::map<unsigned int, std::string>::iterator it = server->to_return.begin();
        if(it->first == 301 || it->first == 302 || it->first == 303 || it->first == 307)
        {
            if(it->second.find("http://") == std::string::npos && it->second.find("https://") == std::string::npos)
                return false;
            std::stringstream ss;
            ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
            ss << "Location: " << it->second << "\r\n";
            ss << "\r\n";
            this->_header = ss.str();
        }
        else
        {
            std::stringstream ss;
            ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
            ss << "\r\n";
            this->_header = ss.str();
            this->_content = server->to_return[it->first];
            this->_contentSize = server->to_return[it->first].length();
        }
    }
    if(findLocationStatusCode(server, ptf))
    {
        this->_isGenerated = true;
        Location* location = server->getLocationFrom(ptf);
        if(!location)
        {
            this->_isGenerated = false;
            return false;
        }
        std::map<unsigned int, std::string>::iterator it = location->to_return.begin();
        if (location->path == ptf)
        {
            if(it->first == 301 || it->first == 302 || it->first == 303 || it->first == 307)
            {
                std::cout << RED << it->first << it->second << std::endl;
                if(it->second.find("http://") == std::string::npos && it->second.find("https://") == std::string::npos)
                    return false;
                std::stringstream ss;
                ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
                ss << "Location: " << it->second << "\r\n";
                ss << "\r\n";
                this->_header = ss.str();
            }
            else
            {
                std::stringstream ss;
                ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
                ss << "\r\n";
                this->_header = ss.str();
                this->_content = server->to_return[it->first];
                this->_contentSize = server->to_return[it->first].length();
            }
        }
        else
            this->_isGenerated = false;
    }
    else
        this->_isGenerated = false;
    return(this->_isGenerated);
}

void Response::headerFileBuilder(std::string file_type) {
	std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_contentSize << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::headerGenBuilder(std::string file_type) {
    std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content << "\r\n\r\n";
    this->_header = header_tmp.str();
}

void Response::generateAutoindex(Request & req) {
   DIR* dir = opendir(this->_uri.c_str());
   if (!dir && DEBUG)
       std::cerr << RED << "can't open dir" << RESET << std::endl;
    std::stringstream buff;

    // WARNING!!! MOST BEAUTIFUL LINE IN THE WORLD
    buff << "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " << this->_uri << "</title>\n<style>* {box-sizing: border-box;}  html {height: 150%;width: 100%;background: linear-gradient(0deg, rgb(39, 16, 16) 0%, rgb(93, 35, 55) 100%);}  .cybr-btn{margin-top: 50px;width: 500px;--primary: hsl(var(--primary-hue), 85%, calc(var(--primary-lightness, 50) * 1%));--shadow-primary: hsl(var(--shadow-primary-hue), 90%, 50%);--primary-hue: -15;--primary-lightness: 50;--color: hsl(0, 0%, 100%);--font-size: 26px;--shadow-primary-hue: 180;--label-size: 9px;--shadow-secondary-hue: 60;--shadow-secondary: hsl(var(--shadow-secondary-hue), 90%, 60%);--clip: polygon(0 0, 100% 0, 100% 100%, 95% 100%, 95% 90%, 85% 90%, 85% 100%, 8% 100%, 0 70%);--border: 4px;--shimmy-distance: 5;--clip-one: polygon(0 2%, 100% 2%, 100% 95%, 95% 95%, 95% 90%, 85% 90%, 85% 95%, 8% 95%, 0 70%);--clip-two: polygon(0 78%, 100% 78%, 100% 100%, 95% 100%, 95% 90%, 85% 90%, 85% 100%, 8% 100%, 0 78%);--clip-three: polygon(0 44%, 100% 44%, 100% 54%, 95% 54%, 95% 54%, 85% 54%, 85% 54%, 8% 54%, 0 54%);--clip-four: polygon(0 0, 100% 0, 100% 0, 95% 0, 95% 0, 85% 0, 85% 0, 8% 0, 0 0);--clip-five: polygon(0 0, 100% 0, 100% 0, 95% 0, 95% 0, 85% 0, 85% 0, 8% 0, 0 0);--clip-six: polygon(0 40%, 100% 40%, 100% 85%, 95% 85%, 95% 85%, 85% 85%, 85% 85%, 8% 85%, 0 70%);--clip-seven: polygon(0 63%, 100% 63%, 100% 80%, 95% 80%, 95% 80%, 85% 80%, 85% 80%, 8% 80%, 0 70%);font-family: 'Cyber', sans-serif;color: var(--color);background: transparent;text-transform: uppercase;font-size: var(--font-size);outline: transparent;letter-spacing: 2px;position: relative;font-weight: 700;border: 0;min-width: 300px;height: 75px;line-height: 75px;}  .cybr-btn:after, .cybr-btn:before {content: '';position: absolute;top: 0;left: 0;right: 0;bottom: 0;clip-path: var(--clip);z-index: -1;}  .cybr-btn:before {background: var(--shadow-primary);transform: translate(var(--border), 0);}.cybr-btn:after {background: var(--primary);}  .cybr-btn__glitch {position: absolute;top: calc(var(--border) * -1);left: calc(var(--border) * -1);right: calc(var(--border) * -1);bottom: calc(var(--border) * -1);background: var(--shadow-primary);text-shadow: 2px 2px var(--shadow-primary), -2px -2px var(--shadow-secondary);clip-path: var(--clip);animation: glitch 2s infinite;}  .cybr-btn:hover .cybr-btn__glitch {display: block;}.cybr-btn__glitch:before {content: '';position: absolute;top: calc(var(--border) * 1);right: calc(var(--border) * 1);bottom: calc(var(--border) * 1);left: calc(var(--border) * 1);clip-path: var(--clip);background: var(--primary);z-index: -1;}@keyframes glitch { 0% {clip-path: var(--clip-one);} 2%, 8% {clip-path: var(--clip-two);transform: translate(calc(var(--shimmy-distance) * -1%), 0);} 6% {clip-path: var(--clip-two);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 9% {clip-path: var(--clip-two);transform: translate(0, 0);} 10% {clip-path: var(--clip-three);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 13% {clip-path: var(--clip-three);transform: translate(0, 0);} 14%, 21% {clip-path: var(--clip-four);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 25% {clip-path: var(--clip-five);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 30% {clip-path: var(--clip-five);transform: translate(calc(var(--shimmy-distance) * -1%), 0);} 35%, 45% {clip-path: var(--clip-six);transform: translate(calc(var(--shimmy-distance) * -1%));} 40% {clip-path: var(--clip-six);transform: translate(calc(var(--shimmy-distance) * 1%));} 50% {clip-path: var(--clip-six);transform: translate(0, 0);} 55% {clip-path: var(--clip-seven);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 60% {clip-path: var(--clip-seven);transform: translate(0, 0);} 31%, 61%, 100% {clip-path: var(--clip-four);} }  .autoindex-title {font-family: 'Cyber', sans-serif;font-size: 25px;color: #fefefe;margin-top: 100px;}.container {display: flex;justify-content: center;}.index {font-size: 20px;display: flex;flex-direction: column;text-decoration: none;font-family: 'Cyber', sans-serif;color: whitesmoke;list-style-type: none;}.index a {color: inherit;text-decoration: none;}  .index li {position: relative;color: #ffffff;background-color: #3C0D0F;border-radius: 10px 2px;padding: 10px;border: 2px solid #AA2824;margin: 10px;right: 3%;min-width: 450px;filter: drop-shadow(0 0 0.25rem #a01336);transition: background-color 1s ease;}.index li::before, .index li::after {content: '';position: absolute;top: 0;bottom: 0;width: 0%;background: linear-gradient(to right, transparent, #AA2824);transition: width 1s ease;z-index: -1;}.index li::before{left: 0;}.index li::after{right: 0;}.index li:hover::before,.index li:hover::after {width: 50%;}.index li:hover {filter: drop-shadow(0 0 0.75rem #a01336);background-color: #AA2824;}</style></head>\n<body><center><h1 class=\"cybr-btn\">Webserv, a 42 project<span aria-hidden class=\"cybr-btn__glitch\">Webserv, a 42 project</span></h1><h2 class=\"autoindex-title\">Autoindex of ressources/assets</h2></center><div class=\"container\"><ul class=\"index\">";

    struct dirent *entry;
    entry = readdir(dir);
    while (entry != NULL) {
        buff << "<a class=\"text\" href=\"http://" << this->server->getServerName() << ":";
        std::ostringstream tstring;
        tstring << this->server->getPort();
        buff << tstring.str() << req.getPathToFile();
        buff << (req.getPathToFile()[req.getPathToFile().size() - 1] == '/' ? "" : "/");
        buff << entry->d_name << "\"><li>./" << entry->d_name << "</li></a>\n";
        entry = readdir(dir);
    }
    buff << "</ul></div></body></html>";
    closedir(dir);

    this->_content = buff.str();
    headerGenBuilder("text/html");
    this->_isGenerated= true;
    if (DEBUG)
        std::cout << "Autoindex Generated" << std::endl;
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

void Response::cgiBuilder(const Request &request) {
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
    std::cout << "res= " << t << std::endl;
    closeAllPipe();
}

bool Response::isCgi(const std::string &file_type) {
	//temp system
	if (file_type.find("py") != std::string::npos)
		return true;
	return false;
}

void Response::pipeCreatorAndExec(char **argv) {
    if (pipe(this->_pipe_out) < 0) {
        std::cout << "pipe1 marche po" << std::endl;
        exit(1);
    }
    if (pipe(this->_pipe_in) < 0) {
        std::cout << "pipe2 marche po" << std::endl;
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

void Response::closeAllPipe() {
    close(_pipe_in[0]);
    close(_pipe_in[1]);
    close(_pipe_out[0]);
    close(_pipe_out[1]);
}

std::string intToString(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string Response::getCodeHeader(std::string * path, Server* server,  const std::string & uri) {
    (void)server;
    *path = server->getRootFrom(uri) + "/";
    // if(!server->to_return.empty())
    // {
    //     std::string newPath;
    //     std::map<int, std::string>::iterator it = server->to_return.begin();
    //     size_t pos = it->second.find("ressources");
    //     if(pos != std::string::npos)
    //     {
    //         size_t i = pos;
    //         while(i < it->second.length())
    //             i++;
    //         newPath = it->second.substr(pos, i);
    //     }
    //     *path = newPath;
    //     return("HTTP/1.1 404 Not Found\n");
    // }
    if (errno == EACCES || errno == EROFS) {
        try {
            *path += server->getErrorPage(403, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[403];
        }
        return ("HTTP/1.1 403 Forbidden\n");
    } else if (errno == ENAMETOOLONG) {
        try {
            *path += server->getErrorPage(414, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[414];
        }
        return ("HTTP/1.1 414 Uri Too Long\n");
    } else if (errno == ENOENT) {
        try {
            *path += server->getErrorPage(404, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[404];
            std::cout << RED << *path << RESET << std::endl;
        }
        return ("HTTP/1.1 404 Not Found\n");
    }else if (errno == ENOTDIR || errno == EINVAL || errno == EROFS || errno == ISDIRECTORY) {
        try {
            *path += server->getErrorPage(400, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[400];
        }
        return ("HTTP/1.1 400 Bad Request\n");
    }else if (errno == ETXTBSY) {
        try {
            *path += server->getErrorPage(409, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[409];
        }
        return ("HTTP/1.1 409 Conflict\n");
    }else if (errno == NOTALLOWEDMETHOD) {
        try {
            *path += server->getErrorPage(405, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[403];
        }
        return ("HTTP/1.1 405 Method Not Allowed\n");
    } else {
        try {
            *path += server->getErrorPage(500, uri);
        } catch (std::exception &e) {
            *path = __defaultErrorPages[500];
        }
        return ("HTTP/1.1 500 Internal Server Error\n");
    }
}

void Response::handleRequestError(Server* server, const std::string & uri) {
    std::stringstream tmphead;
    std::string codePath;

    if (DEBUG)
        std::cout << RED << "Sending error code, reason: " << errno << RESET << std::endl;

    tmphead << getCodeHeader(&codePath, server, uri);
    if (this->_contentFile)
        delete this->_contentFile;
    this->_contentFile = new std::ifstream;
    this->_contentFile->open(codePath.c_str(),std::ifstream::in);
    if (!this->_contentFile->is_open()) {
        if (DEBUG)
            std::cerr << RED << "error: can't access error page" << RESET << std::endl;
        this->_header = tmphead.str() + "Content-Length: 0\n\r\n\r\n";
        return;
    }
    this->_contentFile->seekg(0, std::ios::end);
    this->_contentSize = this->_contentFile->tellg();
    this->_contentFile->seekg(0, std::ios::beg);
    tmphead << "Content-Length: " << this->_contentSize << "\nContent-Type: text/html\n\n";
    this->_header = tmphead.str();
}

std::string & Response::getContent() {
    return this->_content;
}

std::string &Response::getUri() {
    return this->_uri;
}

std::string &Response::getHeader() {
    return this->_header;
}

bool Response::getGenerated() {
    return this->_isGenerated;
}
