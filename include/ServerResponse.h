#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

#define STATUS_CODE_ERROR -1

#include <Socket.h>
#include <vector>
#include <string>
#include <MimeType.h>
#include <fstream>
#include <ctime>

using namespace std;

class ServerResponse {
    public:
        ServerResponse();
        virtual ~ServerResponse();
        void request_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory);
        void get_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory, string root, string extension);

    private:
        Socket socket_client;
};

#endif // SERVERRESPONSE_H
