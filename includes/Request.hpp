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
        const bool &getIsDir() const;
		bool isKeepalive() const;
        int tryAccess(Request & req, bool autoindex);

		class recvFailure : public requestError {
			const char * what() const throw() {
				return ("-failed to read / empty request");
			}
		};

		class tooLongRequest : public requestError {
			const char *what() const throw() {
				return ("request exceeded buffer size");
			}
		};
        class accessError : public requestError {
            const char *what() const throw() {
                return ("can't access file or directory");
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
        bool isDir;
};