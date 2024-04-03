#pragma once

#include "../incs.hpp"
#include "../Request.hpp"
#include "../Cgi.hpp"

class Client;
class Cgi;

class AResponse {
	public:
        AResponse();
        AResponse(Server & server);
        virtual ~AResponse() {};

		std::string &getHeader();
		std::string &getContent();
        std::string &getUri();
        bool getGenerated();
        bool & modIsPosting();
        std::ifstream * getContentFile();
        long long getContentSize();
        void setContent(const std::string &content);
        void setContentSize(long long size);
        void handleRequestError(Server* server, const std::string & uri);


	protected:
        void defaultFileBuilder(const Request &request);
        void headerGenBuilder(std::string file_type);
        void headerFileBuilder(std::string file_type, Request &request);
        std::string getCodeHeader(std::string * path, Server* server, const std::string & uri);
        void redirectWellSlashed(const std::string & uri);

        Server *server; // Attached server
        std::string _uri;

		std::string _header;
        std::string _content; // For auto-generated content like autoindex
        std::ifstream *_contentFile; //For local files
        long long _contentSize;
        bool _isGenerated;
        bool _isPosting;
};
