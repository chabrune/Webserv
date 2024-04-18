
#include "../includes/Client.hpp"

Client::Client(int fd, sockaddr_in addr, Server *server) : sockfd(fd), addr(addr), server(server), response(NULL), readyToSend(false), headerSent(false), sent(false), contentSent(0) { }

Client::~Client() {
    if (this->response && this->response->getContentFile())
        delete this->response->getContentFile();
    if (this->response)
        delete this->response;
}

std::ostream & operator<<(std::ostream & out, const Client & cli) {
    out << inet_ntoa(cli.addr.sin_addr) << ":" << (int)ntohs(cli.addr.sin_port);
    return out;
}

void Client::sendResponse() {
    // Ne pas couper le client s'il continu d'envoyer du body meme si on a detecter une erreur sinon NS_ERROR_NET_RESET
    if (this->request.getMethod() == "POST") {
        std::string buffer;
        buffer.resize(HTTP_BUFFER_SIZE);
        if (recv(this->request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0) > 0)
            return;
    }
    // Send Header
    if (!this->headerSent) {
        if (send(this->sockfd, &this->response->getHeader()[0], this->response->getHeader().size(), 0) == -1) {
            std::cerr << RED << "❗ connection lost for " << GREEN << *this << RESET << std::endl;
            this->sent = true;
            return;
        }
        this->headerSent = true;
    }
    else if (this->response->getGenerated())
        sendGeneratedContent();
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

void Client::sendInfileContent() {
    char buffer[SND_BUFFER_SIZE + 1];
    memset(buffer, '\0', SND_BUFFER_SIZE + 1);

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
