#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <thread>
#include <Socket.h>
#include <MimeType.h>
#include <string>

using namespace std;

#define MAX_QUEUE_LENGTH 3


class HttpServer {
    public:
        HttpServer(string host, int port, string directory);
        virtual ~HttpServer();
        void init_server();

    private:
        Socket* socket_server;
        string directory;
        MimeType mimetype_list[14] = {
            MimeType("au", "audio/basic"),
            MimeType("mp3", "audio/mpeg"),
            MimeType("wav", "audio/wav"),
            MimeType(".ico", "image/x-icon"),
            MimeType("gif", "image/gif"),
            MimeType("jpeg", "image/jpeg"),
            MimeType("jpg", "image/jpg"),
            MimeType("png", "image/png"),
            MimeType("css", "text/css"),
            MimeType("html", "text/html"),
            MimeType("js", "text/js"),
            MimeType("txt", "text/txt"),
            MimeType("avi", "video/x-msvideo"),
            MimeType("pdf", "application/pdf")
        };
};

#endif // HTTPSERVER_H
