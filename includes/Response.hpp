#pragma once

#include "incs.hpp"
#include "Request.hpp"

class Client;

class Response {
	public:
        Response();
		Response(Server & server, Request &request);

		std::string &getHeader();
		std::string &getContent();
        std::string &getUri();
        bool getGenerated();
        void handleRequestError(Server* server, const std::string & uri);

        std::ifstream *_contentFile; //For local files
        long long _contentSize;
	private:
        void headerGenBuilder(std::string file_type);
        void headerFileBuilder(std::string file_type);
        void generateAutoindex(Request & req);
        void cgiBuilder(const Request &request);
		bool isCgi(const std::string &file_type);
        std::string getCodeHeader(std::string * path, Server* server, const std::string & uri);

        Server *server; // Attached server
        std::string _uri;
		std::string _header;
        std::string _content; // For auto-generated content like autoindex
        bool _isGenerated;

        char **_env;
        //int pipeIn[2]; //Used by server to send data to cgi
        //int pipeOUt[2]; // Used by cgi to send data to server
};
