#include "Socket.h"

Socket::Socket(string host, int port) {
    int option_value = 1;
    int is_connected;

    // Ajuda a evitar erros como endereço já em uso.
    int successful = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));

    socket_descriptor = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL); // Instancia de um socket

    if(successful == CODE_STATUS_ERROR){
        cout << "Instancia invalida!" << endl;
    }

    socket_config(host, port);

    is_connected = bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address));

    if(is_connected){
        cout << "Ouvindo na porta " << port << endl;
        cout << "http://localhost:" << port << endl;
    }
    else{
        cout << "Verifique se a porta " << port << " esta ocupada" << endl;
        close_socket();
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
    memset(server_address.sin_zero, '\0', sizeof(server_address));
}

/**
* Realiza a conexão do socket.
*/
bool Socket::bind_socket(int &socket_descriptor, struct sockaddr_in &server) {
    const int is_binded = bind(socket_descriptor, (struct sockaddr*)&server, sizeof(server));

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
    const int socket_accepted = accept(socket_descriptor, (struct sockaddr*)&client_address, &address_length);

    return socket_accepted;
}

/**
* Envia dados pelo socket descritor.
* @return Retorna a quantidade de bytes enviados se tudo ocorrer bem. Se ocorrer algum erro, retorna -1.
*/
int Socket::send_data(int socket, string message) {
    const int bytes_sent = send(socket, message.c_str(), message.size(), 0); // Por que a flag é 0?

    if(bytes_sent == CODE_STATUS_ERROR){
        cout << "Nao foi possivel enviar a mensagem pelo socket." << endl;
    }

    return bytes_sent;
}

sockaddr_in Socket::get_client_address() {
    return client_address;
}

/**
* Define o timer de conexão
*/
void Socket::set_time_out(int &socket, int limit_time) {
    timeval elapsed_time;
    elapsed_time.tv_sec = limit_time;
    elapsed_time.tv_usec = 0;

    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&elapsed_time, sizeof(timeval));
}

/**
* Fecha a conexão do socket.
* @param int socket_descriptor
*/
void Socket::close_socket(int &socket) { // Talvez não funcione
    close(socket);
}

/**
* Fecha a conexão do socket embutido na classe.
*/
void Socket::close_socket() {
    close(socket_descriptor);
}

Databuff Socket::receiver_socket(int &socket) {
    Databuff data_buffer = Databuff();
    const ssize_t buffer_length = recv(socket, &data_buffer, data_buffer.get_max_buffer_length(), 0);
    data_buffer.set_buffer_length(buffer_length);

    return data_buffer;
}







