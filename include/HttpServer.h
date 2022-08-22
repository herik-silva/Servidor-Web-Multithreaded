#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <thread>
#include <Socket.h>
#include <string>
#include <ServerResponse.h>
#include <sys/stat.h>

using namespace std;

#define MAX_QUEUE_LENGTH 5

class HttpServer {
    public:
        HttpServer(string host, int port, string directory);
        virtual ~HttpServer();
        void init_server();

    private:
        Socket* socket_server;
        string directory;

        bool directory_exists();
};

#endif // HTTPSERVER_H
