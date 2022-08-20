#include "Socket.h"

Socket::Socket(string host, int port) {
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
        close_socket(socket_descriptor);
    }
}

Socket::~Socket() {
    close_socket();
}

/**
* Realiza a configuração do socket
* @param string host: endereço da conexão
* @param int port: porta de conexão
*/
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

    return is_ok == 0;
}

/**
* Retorna a primeira conexão na fila de conexões
*/
int Socket::accept_socket() {
    const int socket_accepted = accept(this->socket_descriptor, (struct sockaddr)&client_address, &address_length);

    return socket;
}

/**
* Envia dados pelo socket descritor.
* @return Retorna a quantidade de bytes enviados se tudo ocorrer bem. Se ocorrer algum erro, retorna -1.
*/
int send_data(int socket, string message) {
    const int bytes_sent = send(socket, message.c_str(), message.size(), 0);

    if(bytes_sent == CODE_STATUS_ERROR){
        cout << "Nao foi possivel enviar a mensagem pelo socket." << endl;
    }

    return bytes_sent;
}

/**
* Fecha a conexão do socket.
* @param int socket_descriptor: por padrão utiliza o socket embutido na classe que é instanciado no construtor.
*/
void Socket::close_socket(int *socket_descriptor = this->socket_descriptor) {
    close(socket_descriptor);
}





