#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

#define STATUS_CODE_ERROR -1
#define STATUS_CODE_OK 200
#define NOT_FOUND "public/404.html"

#include "Socket.h"
#include <vector>
#include <string>
#include "MimeType.h"
#include <fstream>
#include <ctime>

using namespace std;

class ServerResponse {
    public:
        ServerResponse() {};
        virtual ~ServerResponse() {};
        string get_status(int response, int length, string extension);
        vector<string> split_string(string value, char break_point);
        void request_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory);
        void get_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory, string root, string extension);
        bool arq_stream(string root, int &length, string &content_file);
        void read_file(fstream &file, string &content_file, int length);
        int	operator()(int thread_id, int clientSockfd, sockaddr_in clientAddr, string dir) {
            request_receiver(thread_id, clientSockfd, clientAddr, dir);
            cout << "PASSOU" << endl;
            return (0);
        }

    private:
        Socket socket_client;
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

        string check_status(int status_code);
        string get_message(int status_code, string date_buffer, int content_length, bool use_keep_alive, string extension);
};

#endif // SERVERRESPONSE_H
