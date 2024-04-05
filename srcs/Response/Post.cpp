
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

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
        if (this->_filePath.empty()) {
            this->done = true;
            return;
        }
        std::ofstream file(this->_filePath.c_str(), std::ios_base::out | std::ios_base::app);
        size_t nextBound = buffer.find("--" + request.getBoundary());
        std::string content;
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
            file.close();
            return;
        }
        file << content;
        file.close();
    }
}

void Post::execPost(Server & server, Request &request, bool & readyToSend) {
    if (DEBUG)
        std::cout << MAGENTA << "starting POST" << RESET << std::endl;
    try {
        std::string buffer;
        if (!this->processing) {
            request.tryAccess_Post(&server, &request);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile())) + server.getUploadFolderFrom(request.getPathToFile());
            buffer = request.getBody();
        }
        if (!this->done && this->processing) { // Si le dernier boundary est toujours pas arrive
            long tmp;
            buffer.resize(HTTP_BUFFER_SIZE);
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp < 0) {
                if (DEBUG)
                    std::cerr << YELLOW << "recv failed or empty" << tmp << RESET << std::endl;
                readyToSend = true;
                throw Request::recvFailure();
            }
            buffer.resize(tmp);
            request.len += tmp;
        }
        if (server.isCgi(request.getExtension())) {
            buffer.push_back('\n');
            Cgi(*this, request, server, buffer);
            this->headerFileBuilder(request.getFileType(), request);
            readyToSend = true;
            return;
        }
        treatBuffer(buffer, request);
        if (this->done) {
            if (DEBUG)
                std::cout << MAGENTA << "Post is done" << RESET << std::endl;
            if (g_error == TOOLARGEENTITY)
                throw Request::tooLongRequest();
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