#pragma once

#include "incs.hpp"
#include "Request.hpp"

class Response {
	public:
		Response(const Request &request);

		std::string &getHeader() const;
		std::string &getContent() const;
		std::string getResponse() const;
		int getResponseSize() const;
        static void handleRequestError(requestError &e, int sockfd);
	private:
		void headerBuilder(const std::string &file_type);
		void contentBuilder(std::ifstream &file, const std::string &file_type);
		void cgiContentBuilder();
        void contentBuilder(std::fstream &file);
        static std::string getCodeHeader(std::string * path);

		std::string _header;
		std::string _content;
		std::string _response;
		int _response_size;
};
