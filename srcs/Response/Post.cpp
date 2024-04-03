
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server) : AResponse(server), done(false), processing(false)
{

}

void Post::treatBuffer(std::string & buffer, Request &request) {
    size_t original_size = buffer.size();
    std::cout << YELLOW << "buffer size: " << original_size << RESET << std::endl;
    // std::cout << YELLOW << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV" << RESET << std::endl;
    // std::cout << YELLOW << buffer << RESET << std::endl;
    // std::cout << YELLOW << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << RESET << std::endl;
    while (!buffer.empty()) 
    {
        // file 
        if (buffer.find("--" + request.getBoundary()) == 0) 
        {
            size_t filenamePos = buffer.find("filename=\"");
            if (filenamePos == std::string::npos) {
                continue;
            } else {
                size_t slash = buffer.find("\"", filenamePos + 10);
                // std::cout << RED << slash << RESET << std::endl;
                this->_filename = buffer.substr(filenamePos + 10, slash - filenamePos - 10);
                this->_filePath = this->_uri + '/' + this->_filename;
                
            }
            buffer = buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.size());
        }
        if (this->_filePath.empty()) {
            // Gerer erreur psq ya rien a creer
        }
        std::ofstream file(this->_filePath.c_str(), std::ios_base::out | std::ios_base::app);
        size_t nextBound = buffer.find("--" + request.getBoundary());
        if (buffer.find("--" + request.getBoundary() + "--") == nextBound && nextBound != std::string::npos) {
            file << buffer.substr(0, buffer.size() - (request.getBoundary().size() + 8));
            buffer.clear();
            std::cout << RED << "DONE TRUE !?" << RESET << std::endl;
            this->done = true;
        } else if (nextBound != std::string::npos) {
            file << buffer.substr(0, nextBound - 2);
            buffer = buffer.substr(nextBound, buffer.size() - 2);
        } else {
            file << buffer;
            buffer.clear();
        }
        file.close();
    }
}

void Post::execPost(Server & server, Request &request, bool & readyToSend) {
        try {
        std::string buffer;
        if (!this->processing) {
//            this->fileExist = tryAccess_Post(&server); // ??
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
            buffer = request.getBody();
            if(buffer.size() > server.max_body_size) // ??
            {
                g_error = TOOLARGEENTITY;
                throw Request::tooLongRequest();
            }
        }
        if (!this->done && this->processing) { // Si le dernier boundary est toujours pas arrive
            long tmp;
            buffer.resize(HTTP_BUFFER_SIZE);
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp < 0) {
                std::cout << YELLOW << "recv failed or enmpty, exiting POST, value: " << tmp << RESET << std::endl;
                perror("");
                readyToSend = true;
                throw Request::recvFailure();
            }
            buffer.resize(tmp);
            request.len += tmp;
            std::cout << RED << "total lu: " << request.len << RESET << std::endl;

        }
        treatBuffer(buffer, request);
        if (this->done) {
            std::cout << MAGENTA << "Post is done" << RESET << std::endl;
            this->_isGenerated = true;
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