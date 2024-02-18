#include "../includes/Response.hpp"

Response::Response(const std::string &path_to_file, const std::string &file_type) {
	std::string tester = "experiment/expe_ali/site/" + path_to_file;

	std::fstream file;
	if (file_type == "jpg") {
		file.open(tester.c_str(), std::fstream::binary);
	} else
		file.open(tester.c_str(), std::fstream::in);
	if (file.fail())
		printf("err file");

	contentBuilder(file);
	headerBuilder(file_type);
	_response_size = _header.length() + _content.length();
	_response.resize(_response_size);
	_response = _header + _content;
}

void Response::headerBuilder(const std::string &file_type) {
	std::stringstream header_tmp;

	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.length() << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::contentBuilder(std::fstream &file) {
	std::string line;

	while(file.good()) {
		std::getline(file, line);
		//if line is CGI...
		this->_content.append(line);
	}
}

std::string Response::getResponse() const {
	return this->_response;
}

int Response::getResponseSize() const {
	return this->_response_size;
}