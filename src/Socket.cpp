#include "Socket.h"

Socket::Socket(string host, int port) {
    const int CODE_STATUS_ERROR = -1;
    const int INTERNET_PROTOCOL = 0; // Constante do protocolo IP
    int option_value = 1;
    int is_connected;

    // Ajuda a evitar erros como endereço já em uso.
    int successful = setsockopt(socket_instance, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));

    socket_descriptor = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL); // Instancia de um socket

    if(successful == CODE_STATUS_ERROR){
        cout << "Instancia invalida!" << endl;
    }

    socket_config(host, port);

    is_connected = bind_socket(socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address));

    if(is_connected){
        cout << "Ouvindo na porta " << port << endl;
        cout << "http://localhost:" << port << endl;
    }
    else{
        cout << "Verifique se a porta " << port << " esta ocupada" << endl;
        close(socket_descriptor);
    }
}

Socket::~Socket() {
    //dtor
}

void Socket::socket_config(string host, int port) {
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;
    inet_aton(host.c_str(), &server_address.sin_addr);
}

/**
* Realiza a conexão do socket.
*/
bool Socket::bind_socket(int *socket_descriptor, struct sockaddr* client_address) {
    const int is_binded = bind(socket_descriptor, (struct sockaddr)&client_address, sizeof(client_address));

    return is_binded == 0;
}

bool Socket::listen_socket(int queue_length) {
    const int is_ok = listen(this->socket_descriptor, queue_length);
    this->close_
    return is_ok == 0;
}

/**
* Fecha a conexão do socket.
*/
void Socket::close_socket(int *socket_descriptor = this->socket_descriptor) {
    close(socket_descriptor);
}







