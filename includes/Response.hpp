#pragma once

#include "incs.hpp"
#include "Request.hpp"
#include "Client.hpp"

class Client;

#define BUFFER_SIZE 1024

class Response {
	public:
		Response(int sockfd, Server & server, Request &request);

		std::string &getContent() const;
        std::string &getUri();
        static void handleRequestError(int sockfd);
	private:
		bool fileManager(const char *path, const std::string &extension);
		void headerManager(int sockfd, std::string file_type) const;
		void mediaContentManager(int sockfd);
		void contentManager(int sockfd);
		void generateAutoindex(int sockfd, Request & req);
		void cgiBuilder();
        static std::string getCodeHeader(std::string * path);

        Server & server; // Attached server
        std::string _uri;
		std::ifstream _file;
		std::string _content;
		long long _content_size;
        bool _isAutoindex;
};
