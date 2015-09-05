#include "Server.h"

std::string Server::getName(){
    return this -> name;
}

std::string Server::getIp(){
    return this -> ip;
}

void Server::setName( std::string newName ){
    this -> name = newName;
}

void Server::setIp( std::string newIp ){
    this -> ip = newIp;
}
