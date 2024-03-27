
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server) : AResponse(server), chunked(false), done(false), processing(false)
{

}

void Post::doThingsAndLetsSeeWhatHappenMaybeItWillWorkMaybeNotWeWillSeeLetsPrayTogetherAndMakeLoveNotWar___amen(Server & server, Request &request, bool & readyToSend) {
        try {
        if (!this->processing) {
//            this->fileExist = tryAccess_Post(&server);
            this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
            this->processing = true;
        }
        if (server.isCgi(request.getExtension())) {
//            if (!this->fileExist)
//                throw ???
            // Gerer CGI
            if (!this->done)
                return;
        } 
        // Ecire dans fichier
        std::string buffer;
        buffer = request.getBody();
        std::cout << RED << buffer << RESET << std::endl;
        std::cout << buffer.size() << " " << buffer.length() << ": LETS SEE : " << request.getContentLenght() << std::endl;
        std::string pathfile = this->_uri + '/' + request.getPostFilename();
        std::ofstream file(pathfile.c_str(), std::ios_base::out | std::ios_base::trunc);
        // std::cout << GREEN << request.getBoundary() << RESET << std::endl;
        if(buffer.find("--" + request.getBoundary() + "--") != std::string::npos)
        {
            //boucle sur boundary pour gerer multi files + creer file si multi
            file << buffer;
            file.close();
            readyToSend = true;
            throw Request::recvFailure();
        }
        if (!this->done) { // Si le dernier boundary est toujours pas arrive
            long tmp;
            tmp = recv(request.getSockfd(), &(buffer[0]), HTTP_BUFFER_SIZE, 0);
            if (tmp <= 0) {
                std::cout << YELLOW << "recv failed or enmpty, exiting POST" << RESET << std::endl;
                readyToSend = true;
                throw Request::recvFailure();
            }
            request.len += tmp;
            file << request.parseBodyz(this->_uri);
        }
        if (this->chunked) {
            // Gerer multipart
        } else {
            // Gerer en 1 seule fois    
        }
        if (this->done) {
            this->_isGenerated = true;
            this->headerGenBuilder("");
            readyToSend = true;
        }
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