
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server, Request &request) : AResponse(server)
{
    try {
        this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
        std::string ptf = request.getPathToFile();
        request.tryAccess_Post(&server);
        std::string pathFile = this->_uri + '/' + request.getPostFilename();
        std::fstream file(pathFile.c_str(), std::ios_base::out | std::ios_base::trunc);
        file << request.getBody();
        file.close();
    } catch(std::exception &e) {
        throw;
    }
}

/*
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