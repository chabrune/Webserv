
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server) : AResponse(server), done(false), processing(false)
{ 

}

void Post::doSomeThings(std::string & buffer, Request &request) {
    (void)request;
    while (!buffer.empty()) 
    {
        std::cout << YELLOW << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV" << RESET << std::endl;
        std::cout << YELLOW << buffer << RESET << std::endl;
        std::cout << YELLOW << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV" << RESET << std::endl;
        // file 
        if (buffer.find("--" + request.getBoundary()) == 0) 
        {
            size_t filenamePos = buffer.find("filename=\"");
            if (filenamePos == std::string::npos) {
                // Si pas de nom de fichier trouvé, on saute cette partie
    
                continue;
            } else {
                // Extraction du nom du fichier
                // size_t slash = buffer.find("\"", filenamePos + 10);
                // std::cout << RED << slash << RESET << std::endl;
                _filename = "index(5).html";
                // buffer.substr(filenamePos + 10, slash - filenamePos - 10);
                this->_filePath = this->_uri + '/' + this->_filename;
                
            }
            buffer = buffer.substr(buffer.find("\r\n"), buffer.size());
        }
        if (this->_filePath.empty()) {
            // Gerer erreur psq ya rien a creer
        }
        std::ofstream file(this->_filePath.c_str(), std::ios_base::out | std::ios_base::trunc);
        size_t nextBound = buffer.find("--" + request.getBoundary());
        if (buffer.find("--" + request.getBoundary() + "--") == nextBound) {
            file << buffer.substr(0, request.getBoundary().size() + 4);
            buffer.clear();
            this->done = true;
        } else if (nextBound != std::string::npos) {
            file << buffer.substr(0, nextBound);
            buffer = buffer.substr(nextBound, buffer.size());
        }
        else if (request.getBoundary().size() + 4 < buffer.size()) {
            file << buffer.substr(0, buffer.size() - (request.getBoundary().size() + 4));
            buffer = buffer.substr(buffer.size() - (request.getBoundary().size() + 4), buffer.size());
        } else {
            file << buffer;
        }
        file.close();

        // if (nouveau_boundary)
        //  break;

        // buffer.substr(content_readed, buffer.size());
    }
}

void Post::doThingsAndLetsSeeWhatHappenMaybeItWillWorkMaybeNotWeWillSeeLetsPrayTogetherAndMakeLoveNotWar___amen(Server & server, Request &request, bool & readyToSend) {
        try {
            _filename.resize(strlen("index(5).html"));
            _filename = "index(5).html";
        std::string buffer;
        if (!this->processing) {
//            this->fileExist = tryAccess_Post(&server);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
            buffer = request.getBody();
           // std::cout << YELLOW << buffer.substr(0, 200) << RESET << std::endl;
        }

    
        // std::cout << RED << buffer << RESET << std::endl;
        // std::cout << GREEN << request.getBoundary() << RESET << std::endl;
        // if(buffer.find("--" + request.getBoundary() + "--") != std::string::npos)
        // {
        //     //boucle sur boundary pour gerer multi files + creer file si multi
        //     doSomeThings(buffer, request);
        // }
        if (!this->done && this->processing) { // Si le dernier boundary est toujours pas arrive
            long tmp;
            buffer.resize(HTTP_BUFFER_SIZE);
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp <= 0) {
                std::cout << YELLOW << "recv failed or enmpty, exiting POST" << RESET << std::endl;
                readyToSend = true;
                throw Request::recvFailure();
            }
            std::cout << MAGENTA << "another recv done" << RESET << std::endl;
            buffer.resize(tmp);
            request.len += tmp;

        }
        doSomeThings(buffer, request);
        // if(buffer.find("--" + request.getBoundary() + "--") != std::string::npos)
        //     this->done = true;

        if (this->done) {
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