#pragma once

#include "incs.hpp"
#include "Request.hpp"

class Client;

class Response {
	public:
        Response();
		Response(Server & server, Request &request);

		std::string &getHeader() const;
		std::string &getContent() const;
        std::string &getUri();
		std::string &getResponse();
		int getResponseSize() const;
        static void handleRequestError(int sockfd);
	private:
		void headerBuilder(std::string file_type);
		void contentBuilder(Request & req, std::ifstream &file, const std::string &file_type, const bool isDir);
        void generateAutoindex(Request & req);
        static std::string getCodeHeader(std::string * path);

        Server * server; // Attached server
        std::string _uri;
		std::string _header;
		std::string _content;
		std::string _response;
		int _response_size;
        bool _isAutoindex;
};
