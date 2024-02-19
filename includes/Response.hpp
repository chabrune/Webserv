#pragma once

#include "incs.hpp"

class Response {
	public:
		Response(const std::string &path_to_file, const std::string &file_type);

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
