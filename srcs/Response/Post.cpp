
#include "../../includes/Response/Post.hpp"
#include "../../includes/Response/AResponse.hpp"
#include "../../includes/Server.hpp"

Post::Post(Server & server, Request &request) : AResponse(server)
{
    this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
    std::cout << this->_uri << std::endl;
    (void)request;
}

/*
    check path si allowed
    separer body header dans request
    parse header content type : (pour upload) multipart/form-data avec numero boundary
    utiliser ce numero pour split tout les body
    non bloquant ?
    parser file name
    supprimmer "header" boundary pour parser le body pour le fichier
    verifier max body size
    lire le fichier upload
    std::fstream() ecraser si existe TRONC
    sinon le creer et write le contenu dans le nouveau fichier
    si pas allowed ou si erreur return code erreur HTTP correspondant pour POST
*/