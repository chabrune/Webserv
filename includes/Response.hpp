#pragma once

#include "incs.hpp"
#include "Request.hpp"
#include "Client.hpp"

class Client;

#define BUFFER_SIZE 1024

class Response {
	public:
		Response(int sockfd, Server & server, Request &request);

		std::string &getHeader() const;
		std::string &getContent() const;
        std::string &getUri();
		std::string &getResponse();
		int getResponseSize() const;
        static void handleRequestError(int sockfd);
	private:
		void headerBuilder(std::string file_type);
		void mediaContentManager(int sockfd, std::ifstream &file, const std::string &file_type);
		void contentManager(int sockfd, std::ifstream &file, const std::string &file_type);
		void generateAutoindex(int sockfd, Request & req, const std::string &file_type);
		ssize_t sendResponse(int sockfd, const std::string &file_type);
		void setupResponse();
		void cgiBuilder();
        static std::string getCodeHeader(std::string * path);

        Server & server; // Attached server
        std::string _uri;
		std::string _header;
		std::string _content;
		std::string _response;
		int _response_size;
        bool _isAutoindex;
};
