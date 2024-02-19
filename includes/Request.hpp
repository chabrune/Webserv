#pragma once

#include "incs.hpp"

class Request {
    public:
		Request();
		Request(int sockfd);
        ~Request();

        long len;

		const std::string &getMethod() const;
		const std::string &getPathToFile() const;
		const std::string &getFileType() const;
		const std::string &getExtension() const;
		const std::string &getHost() const;
		bool isKeepalive() const;

		class recvFailure : public std::exception {
			const char * what() const throw() {
				return ("-failed to read / empty request");
			}
		};

		class tooLongRequest : public std::exception {
			const char *what() const throw() {
				return ("request exceeded buffer size");
			}
		};

	private:
        void parseRequest(std::string &str);
		void setFileType();
		std::string method; //GET, POST..
		std::string path_to_file;
		std::string file_type; //text/css, text/js, image/jpg...
		std::string extension; //jpg, png, css...
		std::string host;
		bool keepalive;
};