#pragma once

#include "../incs.hpp"
#include "../Request.hpp"
#include "../Cgi.hpp"

class Client;
class Cgi;

class AResponse {
	public:
        AResponse();
        AResponse(Server & server, Request &request);

		std::string &getHeader();
		std::string &getContent();
        std::string &getUri();
        bool getGenerated();
        std::ifstream * getContentFile();
        long long getContentSize();
        void setContent(const std::string &content);
        void setContentSize(long long size);
        void handleRequestError(Server* server, const std::string & uri);

        virtual bool handleReturn(Server *server, Request& request) {(void)server;(void)request;return false;};
	protected:
        void defaultFileBuilder(const Request &request);
        void headerGenBuilder(std::string file_type);
        std::string getCodeHeader(std::string * path, Server* server, const std::string & uri);
        void redirectWellSlashed(const std::string & uri);

        Server *server; // Attached server
        std::string _uri;

		std::string _header;
        std::string _content; // For auto-generated content like autoindex
        std::ifstream *_contentFile; //For local files
        long long _contentSize;

        bool _isGenerated;
};
