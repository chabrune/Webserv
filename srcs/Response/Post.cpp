
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server, Request &request, bool & readyToSend) : AResponse(server), chunked(false), done(false), processing(false)
{
    try {
        this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
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
        request.parseBodyz(this->_uri);
        if (this->chunked) {
            // Gerer multipart
        } else {
            // Gerer en 1 seule fois
        }
        if (this->done) {
            // Preparer le header et envoyer
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