
#include "../../includes/Response/Get.hpp"
#include "../../includes/Server.hpp"

Get::Get(Server &server, Request &request) : AResponse(server) {
    try {
        if (this->handleReturn(&server, request))
            return;
        request.tryAccess_Get(&server);
        this->_uri = server.getRootFrom(request.getPathToFile()) + request.subLocation(server.getLocationFrom(request.getPathToFile()));
        if (request.getIsDir())
            generateAutoindex(request);
        else if (server.isCgi(request.getExtension())) {
            Cgi(*this, request, server);
            headerFileBuilder(request.getFileType());
        }
        else {
            defaultFileBuilder(request);
            headerFileBuilder(request.getFileType());
        }
    } catch (std::exception &e) {
        throw;
    }
    if (DEBUG)
        std::cout << "Response created. Header:" << std::endl << MAGENTA << this->_header << RESET;
}

void Get::headerFileBuilder(std::string file_type) {
    std::stringstream header_tmp;

    if (this->_isGenerated) {
        file_type = "text/html";
    }
    header_tmp << "HTTP/1.1 200 OK\nContent-Type: " << file_type << "\nContent-Length: " << this->_contentSize << "\r\n\r\n";
    this->_header = header_tmp.str();
}

bool Get::findServerStatusCode(std::map<unsigned int, std::string>::iterator itf)
{
    std::map<unsigned int, std::string>::iterator it = __defaultErrorCodes.errorCodes.find(itf->first);
    return it != __defaultErrorCodes.errorCodes.end();
}

//Jpense que ca n'ira pas voir pour modif
bool Get::findReturnLocations(Server* server)
{
    std::vector<Location*>::iterator itl = server->locations.begin();
    for(; itl != server->locations.end(); itl++)
    {
        if(!(*itl)->to_return.empty())
            return true;
    }
    return false;
}

bool Get::findLocationStatusCode(Server *server, std::string ptf)
{
    bool uri, found;
    std::vector<Location*>::iterator itl = server->locations.begin();
    for(; itl != server->locations.end(); itl++)
    {
        std::map<unsigned int, std::string>::iterator it = (*itl)->to_return.begin();
        for (; it != (*itl)->to_return.end(); it++)
        {
            std::map<unsigned int, std::string>::iterator it2 = __defaultErrorCodes.errorCodes.find(it->first);
            if (it2 != __defaultErrorCodes.errorCodes.end())
                found = true;
            if(!(*itl)->path.empty())
                if(ptf.find((*itl)->path) == 0 || erasesSidesChar((*itl)->path, '/') == erasesSidesChar(ptf, '/'))
                    uri = true;
        }
    }
    return (uri && found);
}

//VOIR NGINX POUR CODE INEXISTANT EX : 999 // POUR L'INSTANT INGORER
bool Get::handleReturn(Server *server, Request& request)
{
    if(!server->to_return.empty() && !findReturnLocations(server))
        return (this->_isGenerated = false);
    std::string ptf = request.getPathToFile();
    // std::cout << GREEN << "TA SOEUR EN SLIP" << RESET << std::endl;
    if (findServerStatusCode(server->to_return.begin()))
    {
        // std::cout << GREEN << "TA SOEUR EN CALBAR" << RESET << std::endl;
        std::map<unsigned int, std::string>::iterator it = server->to_return.begin();
        if(it->first != 301 && it->first != 302 && it->first != 303 && it->first != 307)
        {
            std::stringstream ss;
            ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
            ss << "\r\n";
            this->_header = ss.str();
            this->_content = server->to_return[it->first];
            this->_contentSize = server->to_return[it->first].length();
        }
        else if(it->first == 301 || it->first == 302 || it->first == 303 || it->first == 307)
        {
            if(it->second.find("http://") == std::string::npos && it->second.find("https://") == std::string::npos)
                return(this->_isGenerated = false);
            std::stringstream ss;
            ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
            ss << "Location: " << it->second << "\r\n";
            ss << "\r\n";
            this->_header = ss.str();
        }
        return this->_isGenerated = true;
    }
    if(findLocationStatusCode(server, ptf))
    {
        Location* location = server->getLocationFrom(ptf);
        // std::cout << GREEN << "TA SOEUR EN TMAX" << ptf << RESET << std::endl;
        if(!location)
            return(this->_isGenerated = false);
        // std::cout << GREEN << "TA SOEUR EN SUEUR" << ptf << RESET << std::endl;
        std::map<unsigned int, std::string>::iterator it = location->to_return.begin();
        if (erasesSidesChar(location->path, '/') == erasesSidesChar(ptf, '/'))
        {
            // std::cout << GREEN << "TA SOEUR EN SUEUR" << RESET << std::endl;
            if(it->first != 301 && it->first != 302 && it->first != 303 && it->first != 307)
            {
                std::stringstream ss;
                ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
                ss << "\r\n";
                this->_header = ss.str();
                this->_content = it->second;
                this->_contentSize = it->second.length();
            }
            else if(it->first == 301 || it->first == 302 || it->first == 303 || it->first == 307)
            {
                if(it->second.find("http://") == std::string::npos && it->second.find("https://") == std::string::npos)
                    return(this->_isGenerated = false);
                std::stringstream ss;
                ss << "HTTP/1.1 " << it->first << " " << __defaultErrorCodes.errorCodes[it->first] << "\r\n";
                ss << "Location: " << it->second << "\r\n";
                ss << "\r\n";
                this->_header = ss.str();
            }
            return this->_isGenerated = true;
        }
    }
    else
        this->_isGenerated = false;
    return(this->_isGenerated);
}

void Get::generateAutoindex(Request & req) {
    DIR* dir = opendir(this->_uri.c_str());
    if (!dir && DEBUG) {
        std::cerr << RED << "can't open autoindex dir" << RESET << std::endl;
        throw std::exception();
    }
    std::stringstream buff;

    // WARNING!!! MOST BEAUTIFUL LINE IN THE WORLD
    buff << "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " << this->_uri << "</title>\n<style>* {box-sizing: border-box;}  html {height: 150%;width: 100%;background: linear-gradient(0deg, rgb(39, 16, 16) 0%, rgb(93, 35, 55) 100%);}  .cybr-btn{margin-top: 50px;width: 500px;--primary: hsl(var(--primary-hue), 85%, calc(var(--primary-lightness, 50) * 1%));--shadow-primary: hsl(var(--shadow-primary-hue), 90%, 50%);--primary-hue: -15;--primary-lightness: 50;--color: hsl(0, 0%, 100%);--font-size: 26px;--shadow-primary-hue: 180;--label-size: 9px;--shadow-secondary-hue: 60;--shadow-secondary: hsl(var(--shadow-secondary-hue), 90%, 60%);--clip: polygon(0 0, 100% 0, 100% 100%, 95% 100%, 95% 90%, 85% 90%, 85% 100%, 8% 100%, 0 70%);--border: 4px;--shimmy-distance: 5;--clip-one: polygon(0 2%, 100% 2%, 100% 95%, 95% 95%, 95% 90%, 85% 90%, 85% 95%, 8% 95%, 0 70%);--clip-two: polygon(0 78%, 100% 78%, 100% 100%, 95% 100%, 95% 90%, 85% 90%, 85% 100%, 8% 100%, 0 78%);--clip-three: polygon(0 44%, 100% 44%, 100% 54%, 95% 54%, 95% 54%, 85% 54%, 85% 54%, 8% 54%, 0 54%);--clip-four: polygon(0 0, 100% 0, 100% 0, 95% 0, 95% 0, 85% 0, 85% 0, 8% 0, 0 0);--clip-five: polygon(0 0, 100% 0, 100% 0, 95% 0, 95% 0, 85% 0, 85% 0, 8% 0, 0 0);--clip-six: polygon(0 40%, 100% 40%, 100% 85%, 95% 85%, 95% 85%, 85% 85%, 85% 85%, 8% 85%, 0 70%);--clip-seven: polygon(0 63%, 100% 63%, 100% 80%, 95% 80%, 95% 80%, 85% 80%, 85% 80%, 8% 80%, 0 70%);font-family: 'Cyber', sans-serif;color: var(--color);background: transparent;text-transform: uppercase;font-size: var(--font-size);outline: transparent;letter-spacing: 2px;position: relative;font-weight: 700;border: 0;min-width: 300px;height: 75px;line-height: 75px;}  .cybr-btn:after, .cybr-btn:before {content: '';position: absolute;top: 0;left: 0;right: 0;bottom: 0;clip-path: var(--clip);z-index: -1;}  .cybr-btn:before {background: var(--shadow-primary);transform: translate(var(--border), 0);}.cybr-btn:after {background: var(--primary);}  .cybr-btn__glitch {position: absolute;top: calc(var(--border) * -1);left: calc(var(--border) * -1);right: calc(var(--border) * -1);bottom: calc(var(--border) * -1);background: var(--shadow-primary);text-shadow: 2px 2px var(--shadow-primary), -2px -2px var(--shadow-secondary);clip-path: var(--clip);animation: glitch 2s infinite;}  .cybr-btn:hover .cybr-btn__glitch {display: block;}.cybr-btn__glitch:before {content: '';position: absolute;top: calc(var(--border) * 1);right: calc(var(--border) * 1);bottom: calc(var(--border) * 1);left: calc(var(--border) * 1);clip-path: var(--clip);background: var(--primary);z-index: -1;}@keyframes glitch { 0% {clip-path: var(--clip-one);} 2%, 8% {clip-path: var(--clip-two);transform: translate(calc(var(--shimmy-distance) * -1%), 0);} 6% {clip-path: var(--clip-two);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 9% {clip-path: var(--clip-two);transform: translate(0, 0);} 10% {clip-path: var(--clip-three);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 13% {clip-path: var(--clip-three);transform: translate(0, 0);} 14%, 21% {clip-path: var(--clip-four);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 25% {clip-path: var(--clip-five);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 30% {clip-path: var(--clip-five);transform: translate(calc(var(--shimmy-distance) * -1%), 0);} 35%, 45% {clip-path: var(--clip-six);transform: translate(calc(var(--shimmy-distance) * -1%));} 40% {clip-path: var(--clip-six);transform: translate(calc(var(--shimmy-distance) * 1%));} 50% {clip-path: var(--clip-six);transform: translate(0, 0);} 55% {clip-path: var(--clip-seven);transform: translate(calc(var(--shimmy-distance) * 1%), 0);} 60% {clip-path: var(--clip-seven);transform: translate(0, 0);} 31%, 61%, 100% {clip-path: var(--clip-four);} }  .autoindex-title {font-family: 'Cyber', sans-serif;font-size: 25px;color: #fefefe;margin-top: 100px;}.container {display: flex;justify-content: center;}.index {font-size: 20px;display: flex;flex-direction: column;text-decoration: none;font-family: 'Cyber', sans-serif;color: whitesmoke;list-style-type: none;}.index a {color: inherit;text-decoration: none;}  .index li {position: relative;color: #ffffff;background-color: #3C0D0F;border-radius: 10px 2px;padding: 10px;border: 2px solid #AA2824;margin: 10px;right: 3%;min-width: 450px;filter: drop-shadow(0 0 0.25rem #a01336);transition: background-color 1s ease;}.index li::before, .index li::after {content: '';position: absolute;top: 0;bottom: 0;width: 0%;background: linear-gradient(to right, transparent, #AA2824);transition: width 1s ease;z-index: -1;}.index li::before{left: 0;}.index li::after{right: 0;}.index li:hover::before,.index li:hover::after {width: 50%;}.index li:hover {filter: drop-shadow(0 0 0.75rem #a01336);background-color: #AA2824;}</style></head>\n<body><center><h1 class=\"cybr-btn\">Webserv, a 42 project<span aria-hidden class=\"cybr-btn__glitch\">Webserv, a 42 project</span></h1><h2 class=\"autoindex-title\">Autoindex of " << this->_uri << "</h2></center><div class=\"container\"><ul class=\"index\">";

    struct dirent *entry;
    entry = readdir(dir);
    while (entry != NULL) {
        buff << "<a class=\"text\" href=\"http://" << this->server->getServerName() << ":";
        std::ostringstream tstring;
        tstring << this->server->getPort();
        buff << tstring.str() << req.getPathToFile();
        buff << (req.getPathToFile()[req.getPathToFile().size() - 1] == '/' ? "" : "/");
        buff << entry->d_name;
        buff << (entry->d_type == DT_DIR ? "/" : "");
        buff << "\"><li>";
        buff << (entry->d_type == DT_DIR ? "" : "./");
        buff << entry->d_name << "</li></a>\n";
        entry = readdir(dir);
    }
    buff << "</ul></div></body></html>\r\n\r\n";
    closedir(dir);

    this->_content = buff.str();
    headerGenBuilder("text/html; charset=UTF-8");
    this->_isGenerated = true;
    if (DEBUG)
        std::cout << "Autoindex Generated" << std::endl;
}
