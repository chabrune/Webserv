
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server) { }

Client::~Client() {}

std::ostream & operator<<(std::ostream & out, const Client & cli) {
    out << inet_ntoa(cli.addr.sin_addr) << ":" << (int)ntohs(cli.addr.sin_port);
    return out;
}

/*void Client::readRequest() {
    //std::vector<char> buffer(HTTP_BUFFER_SIZE);
	std::string buffer;
	buffer.resize(HTTP_BUFFER_SIZE);
    this->req.len = recv(this->sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
    if (this->req.len <= 0) {
        throw recvFailure();
    }
    else if (this->req.len >= HTTP_BUFFER_SIZE) {
        throw tooLongRequest();
    }
	Request r = Request(buffer);
	//this->req.content = buffer;
    //this->req.content.append(buffer.begin(), buffer.end());
    //std::cout << BLUE << this->req.content << RESET << std::endl;
    //std::cout << GREEN << this->req.len << RESET << std::endl;
}*/

void Client::sendResponse() {
    char buffer[] = "HTTP/1.1 200 OK\r\n\r\nbonjour\r\n\r\n";
    send(this->sockfd, buffer, strlen(buffer), 0);
}


