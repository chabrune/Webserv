#pragma once

#include "incs.hpp"

class Request {
    public:
		Request();
		Request(int sockfd);
        ~Request();

        long len;
        //std::string content;

		const std::string &getMethod() const;
		const std::string &getPathToFile() const;
		const std::string &getFileType() const;
		const std::string &getHost() const;
		bool isKeepalive() const;

	private:
        void parseRequest(std::string &str);
		std::string method; //GET, POST..
		std::string path_to_file;
		std::string file_type; //css, js, html
		std::string host;
		bool keepalive;
};