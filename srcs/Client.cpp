
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server) { }

Client::~Client() {}

void Client::readRequest() {
    std::vector<char> buffer(HTTP_BUFFER_SIZE);
    this->req.len = recv(this->sockfd, &(buffer[0]), HTTP_BUFFER_SIZE, 0);
    std::cout << "len: " << this->req.len << std::endl;
    if (this->req.len == -1)
        throw recvFailure();
    else if (this->req.len > HTTP_BUFFER_SIZE) {
//        sendErrorPage()
    }
    this->req.content.append(buffer.begin(), buffer.end());
    std::cout << BLUE << this->req.content << RESET << std::endl;
}

void Client::sendResponse() {
    char buffer[] = "HTTP/1.1 308 Permanent Redirect\r\nLocation: https://puginarug.com/";
    send(this->sockfd, buffer, 65, 0);
}
