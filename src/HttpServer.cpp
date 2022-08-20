#include "HttpServer.h"

HttpServer::HttpServer(string host, int port, string directory) {
    this->directory = directory;
    socket_server = new Socket(host, port);
}

HttpServer::~HttpServer() {
    delete socket_server;
    socket_server = nullptr;
}

void HttpServer::init_server() {
    int thread_id = 1;
    ResponseServer response_server;
    socket_server->listen_socket(MAX_QUEUE_LENGTH);

    while(true){
        int socket_descriptor = socket_server->accept_socket();
        socket_server->set_time_out(socket_descriptor, 3);
        thread(response_server, thread_id++, socket_descriptor, socket_server->get_client_address(), directory).detach();
    }
}
