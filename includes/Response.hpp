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
	private:
		void headerBuilder(const std::string &file_type);
		void contentBuilder(std::ifstream &file, const std::string &file_type);
		void cgiContentBuilder();

		std::string _header;
		std::string _content;
		std::string _response;
		int _response_size;
};
