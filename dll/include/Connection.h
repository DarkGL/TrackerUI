#pragma once

class Connection
{
    private:
        static int port;

        static const char * ip;
    public:
        Connection();
        ~Connection();

        void checkFile( const char * fileName );
        void downloadFile( const char * fileName );
};
