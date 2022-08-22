#include "HttpServer.h"

HttpServer::HttpServer(string host, int port, string directory) {
    this->directory = directory;
    cout << "Instanciando Socket" << endl;
    socket_server = new Socket(host, port);
    cout << "Socket Instanciado" << endl;
}

HttpServer::~HttpServer() {
    delete socket_server;
    socket_server = nullptr;
}

/**
* Inicia o servidor
*/
void HttpServer::init_server() {
    if(directory_exists()){
        int thread_id = 1;
        ServerResponse server_response;
        socket_server->listen_socket(MAX_QUEUE_LENGTH);

        while(true){
            int socket_descriptor = socket_server->accept_socket();
            socket_server->set_time_out(socket_descriptor, 3);
            thread(server_response, thread_id++, socket_descriptor, socket_server->get_client_address(), directory).detach();
        }
    }
    else{
        cout << "Diretório nao encontrado!" << endl;
    }
}

/**
* Verifica se o diretório existe
* @return Retorna true se existir e false, caso contrário.
*/
bool HttpServer::directory_exists() {
    struct stat buffer;
    stat(directory.c_str(), &buffer);

    return buffer.st_mode & S_IFDIR;
}
