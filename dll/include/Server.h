#pragma once

#include <string>

class Server
{
    public:
        std::string getName();
        std::string getIp();

        void setName( std::string newName );
        void setIp( std::string newIp );
    private:
        std::string name;
        std::string ip;
};
