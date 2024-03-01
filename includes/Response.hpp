#pragma once

#include "incs.hpp"
#include "Request.hpp"

class Client;

class Response {
	public:
        Response();
		Response(Server & server, Request &request);
        ~Response();

		std::string &getHeader();
		std::string &getContent();
        std::string &getUri();
		std::string &getResponse();
		int getResponseSize() const;
        bool getGenerated();
        void handleRequestError(Server* server);


        std::ifstream *_contentFile; //For local files
        long long _contentSize;
	private:
        void headerGenBuilder(std::string file_type);
        void headerFileBuilder(std::string file_type);
		void contentBuilder(Request & req, std::ifstream &file, const std::string &file_type, const bool isDir);
        void generateAutoindex(Request & req);
        static std::string getCodeHeader(std::string * path, Server* server);

        Server * server; // Attached server
        std::string _uri;
		std::string _header;
        std::string _content; // For auto-generated content like autoindex
		std::string _response;
		int _response_size;
        bool _isGenerated;
};
