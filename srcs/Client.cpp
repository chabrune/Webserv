
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server), readyToSend(false), headerSent(false), sent(false), contentSent(0) { }

Client::~Client() {
    if (this->response._contentFile)
        delete this->response._contentFile;
}

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
    // Send Header
    if (!this->headerSent) {
        if (DEBUG)
            std::cout << MAGENTA << this->response.getHeader() << RESET << std::endl;
        if (send(this->sockfd, &this->response.getHeader()[0], this->response.getHeader().size(), 0) == -1)
            if (DEBUG)
                std::cerr << RED << "TA MERE LE HEADER" << RESET << std::endl;
        this->headerSent = true;
    }
    if (this->response.getGenerated())
        sendGeneratedContent();
    else
        sendInfileContent();
}

void Client::sendGeneratedContent() {
    long size = std::min(static_cast<long long>(SND_BUFFER_SIZE), static_cast<long long>(this->response.getContent().size() - this->contentSent));
    if (DEBUG)
        std::cout << "size: " << this->response.getContent().size() << std::endl;
    std::string buffer = this->response.getContent().substr(this->contentSent, size);

    // Send non-file content
    long sent = send(this->sockfd, &buffer[0], size, 0);
    if (sent == -1) {
        std::cerr << RED << "❗ connexion lost for " << GREEN << *this << RESET << std::endl;
        this->sent = true;
        return;
    } else if (sent != size) {
        size = sent;
    }
    this->contentSent += size;
    if (this->contentSent >= static_cast<long long>(this->response.getContent().size())) {
        this->sent = true;
        if (DEBUG)
            std::cout << GREEN << "all gen data sent" << RESET << std::endl;
    }
}

void Client::sendInfileContent() {
    char buffer[SND_BUFFER_SIZE + 1];
    memset(buffer, 0, SND_BUFFER_SIZE + 1);

    // Send file content
    long size = std::min(static_cast<long long>(SND_BUFFER_SIZE), this->response._contentSize - this->contentSent);
    this->response._contentFile->seekg(this->contentSent);
    this->response._contentFile->read(buffer, size);
    long sent = send(this->sockfd, buffer, size, 0);
    if (sent == -1) {
        std::cerr << RED << "❗ connexion lost for " << GREEN << *this << RESET << std::endl;
        this->sent = true;
        return;
    } else if (sent != size) {
        size = sent;
    }
    this->contentSent += size;
    if (this->contentSent >= this->response._contentSize) {
        this->sent = true;
        if (DEBUG)
            std::cout << GREEN << "all data sent" << RESET << std::endl;
    }
}
