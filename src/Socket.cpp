#include "Socket.h"

Socket::Socket(string host, int port) {
    int option_value = 1;
    int is_connected, successful;
    socket_descriptor = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL); // Instancia de um socket

    // Ajuda a evitar erros como endereço já em uso.
    successful = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));

    if(successful == CODE_STATUS_ERROR){
        cout << "Instancia invalida!" << endl;
    }

    socket_config(host, port);

    is_connected = bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address));

    if(is_connected != CODE_STATUS_ERROR){
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
* @param socket_descriptor: socket que será conectado ao servidor
* @param server: tipo de endereço endereço de socket IPv4
* @return retorna true se a conexão foi estabelecida. Caso contrário, retorna false
*/
bool Socket::bind_socket(int &socket_descriptor, struct sockaddr_in &server) {
    const int is_binded = bind(socket_descriptor, (struct sockaddr*)&server, sizeof(server));

    return is_binded == 0;
}

/**
* Aceita solicitações do cliente e cria uma fila de solicitação de conexões.
* @param queue_length: tamanho máximo da fila de solicitações.
* @return retorna true se a solicitação foi adicionada na fila. Caso contrário, retorna false.
*/
bool Socket::listen_socket(int queue_length) {
    const int request = listen(this->socket_descriptor, queue_length);

    return request == REQUEST_ACCEPTED;
}

/**
* Retorna a primeira conexão na fila de conexões
* @return Caso a conexão seja aceita, retorna o descritor de arquivo não negativo. Caso contrário, retorna -1.
*/
int Socket::accept_socket() {
    const int socket_accepted = accept(socket_descriptor, (struct sockaddr*)&client_address, &address_length);

    return socket_accepted;
}

/**
* Envia dados pelo socket descritor.
* @param socket: socket em que será enviado os dados.
* @param message: conteúdo que será enviado.
* @return Retorna a quantidade de bytes enviados se tudo ocorrer bem. Se ocorrer algum erro, retorna -1.
*/
int Socket::send_data(int socket, string message) {
    const int bytes_sent = send(socket, message.c_str(), message.size(), 0); // Por que a flag é 0?

    if(bytes_sent == CODE_STATUS_ERROR){
        cout << "Nao foi possivel enviar a mensagem pelo socket." << endl;
    }

    return bytes_sent;
}

/**
* @return retorna o endereço do cliente.
*/
sockaddr_in Socket::get_client_address() {
    return client_address;
}

/**
* Define o timer de conexão
* @param socket: recebe um socket por referência.
* @param limite_time: tempo limite de conexão do socket em segundos.
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

/**
* Recebe o socket e insere os dados no Databuff.
* @param socket: recebe um socket por referência.
* @return retorna um Databuff com os dados transmitidos pelo socket.
*/
Databuff Socket::receiver_socket(int &socket) {
    Databuff data_buffer = Databuff();
    const ssize_t buffer_length = recv(socket, data_buffer.get_content(), data_buffer.get_max_buffer_length(), 0);
    data_buffer.set_buffer_length(buffer_length);

    return data_buffer;
}
