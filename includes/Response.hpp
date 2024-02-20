#pragma once

#include "incs.hpp"
#include "Request.hpp"
#include "Client.hpp"

class Client;

class Response {
	public:
		Response(Client & client, Request &request);

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

        Client & client; // Attached client
        std::string _uri;
		std::string _header;
		std::string _content;
		std::string _response;
		int _response_size;
        bool _isAutoindex;
};
