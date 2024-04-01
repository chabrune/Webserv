#pragma once

#include "incs.hpp"
class Server;
class Location;

class Request {
    public:
		Request();
		Request(Server *server, int sockfd);
        ~Request();

        long len;
        bool tooLong;

        void setFileType(const std::string &filetype);
		const std::string &getMethod() const;
		const std::string &getPathToFile() const;
        void setPathToFile(const std::string &path_to_file);
        const std::string &getFileName() const;
        void setFileName(const std::string &file_name);
        const std::string &getQuery() const;
        void setQuery(const std::string &query);
        const std::string &getCookie();
        void setCookie(const std::string &cookie);
        void addCookie(const std::string &value);
		const std::string &getFileType() const;
		const std::string &getExtension() const;
		const int &getContentLenght() const;
		const int &getSockfd() const;
		const std::string &getHost() const;
        const bool &getIsDir() const;
		bool isKeepalive() const;
        void tryAccess_Get(Server *server);
        void tryAccess_Delete(Server *server);
        void tryAccess_Post(Server *server);
        void isAllowed(Server *server);
        const std::string& getBody() const;
        const std::string& getPostFilename() const;
        std::string subLocation(Location *location);
        void parseHeaders(const std::string& headers);
        void parseBodyz(std::string uri, std::string &buffer);
        void handleBody();
        const std::string& getBoundary() const;


		class returnTest : public taMereEnSlip
		{
			const char* what() const throw() {
				return("OK");
			}
		};
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

        class dirDoesNotEndWithSlash : public requestError {
            const char *what() const throw() {
                return ("uri to dir must end with '/'");
            }
        };

        class invalidSlash: public requestError {
            const char *what() const throw() {
                return ("consecutives slash in uri");
            }
        };

        class dirNotEmpty: public requestError {
            const char *what() const throw() {
                return ("dir is not empty");
            }
        };

	private:
        int _sockfd;
        void parseRequest(Server *server, std::string &str);
		void defineFileType();
		std::string method; //GET, POST..
		std::string _path_to_file;
        std::string _file_name; //test.html, example.py...
        std::string _query;
		std::string file_type; //text/css, text/js, image/jpg...
		std::string extension; //jpg, png, css...
		std::string host;
        std::string _header;
        std::string _cookies;
        std::string _body;
        std::string _contentType;
        std::string _boundary;
        std::string _Postfilename;
        std::string _filePath;

        int _contentLength;
        // std::map<std::string, std::string> _headers; A voir
        bool isDir;
};