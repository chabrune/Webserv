#pragma once

#include "incs.hpp"
#include "Request.hpp"
#include "Cgi.hpp"

class Client;
class Cgi;

class Response {
	public:
        Response();
		Response(Server & server, Request &request);

		std::string &getHeader();
		std::string &getContent();
        void setContent(const std::string &content);
        std::string &getUri();
        bool getGenerated();
        void handleRequestError(Server* server, const std::string & uri);
        bool handleReturn(Server *server, Request& request);
        bool findServerStatusCode(std::map<unsigned int, std::string>::iterator itf);
        bool findLocationStatusCode(Server *server, std::string ptf);
        bool findReturnLocations(Server* server);
        bool isCgi(const std::string &extension);

        std::ifstream *_contentFile; //For local files
        long long _contentSize;
	private:
        void defaultFileBuilder(const Request &request);
        void headerGenBuilder(std::string file_type);
        void headerFileBuilder(std::string file_type);
        void generateAutoindex(Request & req);
        void redirectWellSlashed(const std::string & uri);
        void cgiBuilder(const Request &request);
        void pipeCreatorAndExec(char **argv);
        void closeAllPipe();
        std::string getCodeHeader(std::string * path, Server* server, const std::string & uri);

        Server *server; // Attached server
        std::string _uri;
		std::string _header;
        std::string _content; // For auto-generated content like autoindex
        bool _isGenerated;

        char **_env;
        int _pid;
        int _pipe_in[2]; //Used by server to send data to cgi
        int _pipe_out[2]; // Used by cgi to send data to server
};
