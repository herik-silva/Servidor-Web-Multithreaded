#ifndef SOCKET_H
#define SOCKET_H

#define CODE_STATUS_ERROR -1
#define INTERNET_PROTOCOL 0

#include <iostream>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <Databuff.h>

using namespace std;

class Socket {
    public:
        Socket(string host, int port);
        virtual ~Socket();
        void socket_config(string host, int port);
        bool bind_socket(int &socket_descriptor, struct sockaddr_in &client_address);
        bool listen_socket(int queue_length);
        int accept_socket();
        int send_data(int socket, string message);
        sockaddr_in get_client_address();
        void close_socket(int &socket);
        void close_socket();
        void set_time_out(int &socket, int limit_time);
        Databuff receiver_socket(int &socket);
    protected:

    private:
    int socket_descriptor;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t address_length;
};

#endif // SOCKET_H
