#include "../includes/Response.hpp"

Response::Response(const Request &request) {
	std::cout << "New response is under building.." << std::endl;
	std::string tester = "experiment/expe_ali/site" + request.getPathToFile();

	std::ifstream file;
	file.open(tester.c_str(), MimeUtils::getOpenMode(request.getExtension()));
	if (file.fail()) {
		std::cerr << "Error cant open the file.." << std::endl;
		return;
	}

	contentBuilder(file, request.getExtension());
	headerBuilder(request.getFileType());
	_response_size = _header.length() + _content.length();
	_response.resize(_response_size);
	_response = _header + _content;
	std::cout << "Response created. Header:" << std::endl << this->_header;
}

void Response::headerBuilder(const std::string &file_type) {
	std::stringstream header_tmp;

	header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_content.length() << "\r\n\r\n";
	this->_header = header_tmp.str();
}

void Response::contentBuilder(std::ifstream &file, const std::string &extension) {
	std::string line;
	if (MimeUtils::isImage(extension) || MimeUtils::isVideo(extension)) {
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		file.seekg(0, std::ios::beg);
		this->_content.resize(length);
		file.read(&this->_content[0], length);
		return ;
	}

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