
#include "../../includes/Response/Post.hpp"

Post::Post(Server & server) : AResponse(server), done(false), processing(false)
{

}


void Post::treatBuffer(std::string & buffer, Request &request) {
    size_t baseSize = buffer.size();
    bool looped = false;
    while (!buffer.empty())
    {
        if (looped && baseSize == buffer.size()) {
            g_error = BADREQUEST;
            throw requestError();
        } else if (looped) {
            baseSize = buffer.size();
            looped = false;
        } else {
            looped = true;
        }
        if (buffer.find("--" + request.getBoundary()) == 0) 
        {
            size_t filenamePos = buffer.find("filename=\"");
            if (filenamePos == std::string::npos) {
                continue;
            } else {
                size_t slash = buffer.find("\"", filenamePos + 10);
                this->_filename = buffer.substr(filenamePos + 10, slash - filenamePos - 10);
                this->_filePath = this->_uri + '/' + this->_filename;
            }
            buffer = buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.size());
        }
        if (this->_filePath.empty() && request.getContentType().find("multipart") == std::string::npos) {
            size_t slash2 = this->_uri.find_last_of('/');
            this->_filename = this->_uri .substr(slash2 + 1, this->_uri.length() - slash2);
            this->_filePath = this->_uri;
            this->done = true;
        }
        size_t nextBound = buffer.find("--" + request.getBoundary());
        std::string content;
        size_t extension_index = this->_filename.find_last_of('.');
        std::string extension;
        if (extension_index != std::string::npos)
            extension = this->_filename.substr(extension_index + 1, this->_filename.size() - extension_index);
        if (buffer.find("--" + request.getBoundary() + "--") == nextBound && nextBound != std::string::npos) {
            content = buffer.substr(0, buffer.size() - (request.getBoundary().size() + 8));
            buffer.clear();
            this->done = true;
        } else if (nextBound != std::string::npos) {
            content = buffer.substr(0, nextBound - 2);
            buffer = buffer.substr(nextBound, buffer.size() - 2);
        } else {
            content = buffer;
            buffer.clear();
        }
        if (static_cast<unsigned long>(request.len) > this->server->getMaxBodySizeFrom(this->_uri)) {
            g_error = TOOLARGEENTITY;
            return;
        }
        if (this->server->isCgi(extension)) {
            try {
                request.tryExecAccess(server->getRootFrom(request.getPathToFile()) + request.subLocation(server->getLocationFrom(request.getPathToFile())));
            } catch (std::exception &e) {
                return;
            }
            request.setFileName(this->_filename);
            request.setExtension(extension);
            content += '\n';
            Cgi(*this, request, *this->server, content);
        } else {
            std::ofstream file(this->_filePath.c_str(), std::ios_base::out | std::ios_base::app);
            file << content;
            file.close();
        }
    }
}

void Post::execPost(Server & server, Request &request, bool & readyToSend, fd_set * lset) {
    try {
        std::string buffer;
        if (!this->processing) {
            request.tryAccess_Post(&server, &request);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile())) + server.getUploadFolderFrom(request.getPathToFile());
            this->_uri = removeConsecutivesSlash(_uri);
            if (_uri.length() > 1 && _uri[_uri.length() - 1] == '/')
                _uri.erase(_uri.length() - 1, 1);
            buffer = request.getBody();
            request.len = static_cast<long>(buffer.size());
            this->_content = "<h1 style=\"font-family: sans-serif; color: #343434;\">Upload Successfull</h1>";
        }
        if (!this->done && this->processing) { // Si le dernier boundary est toujours pas arrive
            if (!FD_ISSET(request.getSockfd(), lset)) {
                return;
            }
            long tmp;
            buffer.resize(HTTP_BUFFER_SIZE);
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp < 0) {
                std::cerr << RED << "❗ connection lost" << RESET << std::endl;
                this->done = true;
                return;
            } else {
                buffer.resize(tmp);
                request.len += tmp;
            }
        }
        if (!buffer.empty())
            treatBuffer(buffer, request);
        if (this->done) {
            if (DEBUG)
                std::cout << MAGENTA << "Post is done" << RESET << std::endl;
            if (g_error == TOOLARGEENTITY) {
                throw Request::tooLongRequest();
            } else if (g_error != 0) {
                throw requestError();
            }
            this->_isGenerated = true;
            this->headerGenBuilder("");
            readyToSend = true;
        }
        this->processing = true;
    } catch (std::exception &e) {
        throw;
    }
}
