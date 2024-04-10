
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server) : AResponse(server), done(false), processing(false)
{

}

void Post::treatBuffer(Server &server, std::string & buffer, Request &request) {
    (void)server;
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
            this->_filePath = this->_uri;
            this->done = true;
        }
        size_t nextBound = buffer.find("--" + request.getBoundary());
        std::string content;
        size_t extension_index =this->_filename.find_last_of('.');
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
        std::cout << request.len << " " << this->server->getMaxBodySizeFrom(this->_uri) << std::endl;
        if (static_cast<unsigned long>(request.len) > this->server->getMaxBodySizeFrom(this->_uri)) {
            g_error = TOOLARGEENTITY;
            return;
        }
        std::ofstream file(this->_filePath.c_str(), std::ios_base::out | std::ios_base::app);
        file << content;
        file.close();
    }
}

void Post::execPost(Server & server, Request &request, bool & readyToSend) {
    try {
        std::string buffer;
        if (!this->processing) {
            request.tryAccess_Post(&server, &request);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile())) + server.getUploadFolderFrom(request.getPathToFile());
            if (_uri[_uri.length() - 1] == '/')
                _uri.erase(_uri.length() - 1, 1);
            buffer = request.getBody();
            request.len = static_cast<long>(buffer.size());
        }
        if (!this->done && this->processing) { // Si le dernier boundary est toujours pas arrive
            long tmp;
            buffer.resize(HTTP_BUFFER_SIZE);
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp < 0) {
                if (DEBUG)
                    std::cerr << YELLOW << "recv failed or empty" << tmp << RESET << std::endl;
                if (request.getContentType().find("multipart") == std::string::npos) {
                    buffer.resize(0);
                    this->done = true;
                } else {
                    readyToSend = true;
                    throw Request::recvFailure();
                }
            } else {
                buffer.resize(tmp);
                request.len += tmp;
            }
        }
        if (!buffer.empty())
            treatBuffer(server, buffer, request);
        if (this->done) {
            if (DEBUG)
                std::cout << MAGENTA << "Post is done" << RESET << std::endl;
            if (g_error == TOOLARGEENTITY) {
                std::cout << "bah ouai\n";
                throw Request::tooLongRequest();
            }
            this->_isGenerated = true;
            this->_content = "<h1 style=\"font-family: sans-serif; color: #343434;\">Upload Successfull</h1>";
            this->headerGenBuilder("");
            readyToSend = true;
        }
        this->processing = true;
    } catch (std::exception &e) {
        throw;
    }
}

/*
    Voir pour gerer le max body size (+ renvoyer le bon code erreur)
    Implementer le allow_upload pour les routes (allowed ou non) (idem renvoyer le bon code)
    POST CGI ??? 
    Try_AccessPost ?


    S'arreter a l'header pour les autres requetes et body pour post
    C LE BORDEL AVEC LES DEUX MERGE MAIS JE DOIS ALLER GRIMPER BISOUS NOUTNOUT
    check path si allowed || A VOIR
    separer body header dans request || OK
    parse header content type : (pour upload) multipart/form-data avec numero boundary || OK
    utiliser ce numero pour split tout les body
    non bloquant ?
    parser file name || OK
    supprimmer "header" boundary pour parser le body pour le fichier
    verifier max body size
    lire le fichier upload
    std::fstream() ecraser si existe TRONC
    sinon le creer et write le contenu dans le nouveau fichier
    si pas allowed ou si erreur return code erreur HTTP correspondant pour POST
*/






















// J'ai regle 2 3 ptits trucs dont la size qui etait bizarre