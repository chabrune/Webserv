#pragma once

#include "incs.hpp"

class Request {
    public:
		Request();
		Request(int sockfd);
        ~Request();

        long len;
        //std::string content;
		std::string method; //GET POST..
		std::string path_to_file;
		std::string file_type; //css, js, html
		std::string host;
		bool keepalive;

	private:

        void parseRequest(std::string &str);
};