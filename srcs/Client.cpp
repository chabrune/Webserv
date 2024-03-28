
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server), readyToSend(false), headerSent(false), sent(false), contentSent(0) { }

Client::~Client() {
    if (this->response->getContentFile())
        delete this->response->getContentFile();
}

std::ostream & operator<<(std::ostream & out, const Client & cli) {
    out << inet_ntoa(cli.addr.sin_addr) << ":" << (int)ntohs(cli.addr.sin_port);
    return out;
}

void Client::sendResponse() {
    // Send Header
    if (!this->headerSent) {
        if (send(this->sockfd, &this->response->getHeader()[0], this->response->getHeader().size(), 0) == -1)
            if (DEBUG)
                std::cerr << RED << "TA MERE LE HEADER" << RESET << std::endl;
        this->headerSent = true;
    }
    if (this->response->getGenerated())
        sendGeneratedContent();
    else if (this->server->isCgi(request.getExtension()))
        sendCgiContent();
    else
        sendInfileContent();
}

void Client::sendGeneratedContent() {
    long size = std::min(static_cast<long long>(SND_BUFFER_SIZE), static_cast<long long>(this->response->getContent().size() - this->contentSent));
    if (DEBUG)
        std::cout << "size: " << this->response->getContent().size() << std::endl;
    std::string buffer = this->response->getContent().substr(this->contentSent, size);

    // Send non-file content
    long sent = send(this->sockfd, &buffer[0], size, 0);
    if (sent == -1) {
        std::cerr << RED << "❗ connection lost for " << GREEN << *this << RESET << std::endl;
        this->sent = true;
        return;
    } else if (sent != size) {
        size = sent;
    }
    this->contentSent += size;
    if (this->contentSent >= static_cast<long long>(this->response->getContent().size())) {
        this->sent = true;
        if (DEBUG)
            std::cout << GREEN << "all gen data sent" << RESET << std::endl;
    }
}

void Client::sendCgiContent() {
    // Send file content
    send(this->sockfd, this->response->getContent().c_str(), this->response->getContent().size(), 0);
    this->sent = true;
}

void Client::sendInfileContent() {
    char buffer[SND_BUFFER_SIZE + 1];
    memset(buffer, 0, SND_BUFFER_SIZE + 1);

    // Send file content
    long size = std::min(static_cast<long long>(SND_BUFFER_SIZE), this->response->getContentSize() - this->contentSent);
    this->response->getContentFile()->seekg(this->contentSent);
    this->response->getContentFile()->read(buffer, size);
    long sent = send(this->sockfd, buffer, size, 0);
    if (sent == -1) {
        std::cerr << RED << "❗ connection lost for " << GREEN << *this << RESET << std::endl;
        this->sent = true;
        return;
    } else if (sent != size) {
        size = sent;
    }
    this->contentSent += size;
    if (this->contentSent >= this->response->getContentSize()) {
        this->sent = true;
        if (DEBUG)
            std::cout << GREEN << "all data sent" << RESET << std::endl;
    }
}
