#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Socket {
    public:
        Socket(string host, int port);
        virtual ~Socket();
        void socket_config(string host, int port);
        int bind_socket(int *socket_descriptor, struct socketadd_in *client_address);
        int listen_socket(int queue_length);
        void close_socket();
    protected:

    private:
    int socket_descriptor;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
};

#endif // SOCKET_H
