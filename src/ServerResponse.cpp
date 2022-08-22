#include "ServerResponse.h"

#define Debug true

/**
* Recebe as requisições feitas para o servidor.
* @param thread_id: identificador da thread.
* @param socket_descriptor: socket de conexão com o servidor.
* @param client_address: tipo de endereço de socket IPv4.
* @param directory: diretório da pasta onde se encontram os arquivos.
*/
void ServerResponse::request_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory) {
    string request = "", root = "";
    vector<string> extension_list;
    vector<string> headers;

    if(Debug){
        cout << "THREAD_ID: " << thread_id << endl;
    }

    if(socket_descriptor == STATUS_CODE_ERROR){
        return ;
    }

    char ip_string[INET_ADDRSTRLEN] = {'\0'};

    // Converte o endereço de string para um endereço web
    inet_ntop(client_address.sin_family, &client_address.sin_addr, ip_string, sizeof(ip_string));

    if(Debug){
        cout << "Conexão iniciada com o cliente " << ip_string << ":" << ntohs(client_address.sin_port) << endl << endl;
    }

    Databuff receiver_data = socket_client.receiver_socket(socket_descriptor);
    if(receiver_data.get_buffer_length() > 0){
        request = string(receiver_data.get_content(), 0, receiver_data.get_buffer_length());
        if(Debug){
            cout<< request << endl;
        }

        headers = split_string(request, '\n');
        headers = split_string(headers[0], ' ');
        extension_list = split_string(headers[1], '.');

        try{
            if(headers[0] == "GET"){
                if(Debug){
                    cout << "Uma requisiao GET" << endl;
                    cout << "Pasta desejada: " << headers[1] << endl;
                    cout << "Extensao: " << extension_list[extension_list.size()-1] << endl;
                }

                root = directory + headers[1];
                
                get_receiver(thread_id, socket_descriptor, client_address, directory, root, extension_list[extension_list.size()-1]);
            }
            else{
                throw(headers[0]);
            }
        }
        catch(string error){
            string data = "400 Bad Request";
            int length = data.size();
            data += get_status(3, length, "html");

            if(Debug){
                cout << data << endl;
            }

            // Enviando dados para o socket conectado ao cliente e fecha a conexão.
            socket_client.send_data(socket_descriptor, data);
            socket_client.close_socket(socket_descriptor);
        }
    }
    else{
        if(Debug){
            cout<<"Tempo limite de requisição atingido!"<<endl;
        }

        // Fechando conexão.
        socket_client.close_socket(socket_descriptor);
    }
}

/**
* Divide a string onde tiver o caractere de quebra.
* @param value: string que será dividida.
* @param break_point: caractere de quebra onde será dividido a string.
* @return retorna um vector<string> da string separada.
*/
vector<string> ServerResponse::split_string(string value, char break_point) {
    string aux_string = "";
    vector<string> header_content;
    
    for(int index=0; index<(int)value.size(); index++){
        if(value[index] != break_point){
            aux_string += value[index];
        }
        else{
            // Realizando a divisão da string.
            header_content.push_back(aux_string);
            aux_string = "";
        }
    }

    header_content.push_back(aux_string);

    return header_content;
}

/**
* Recebe o pedido e envia o arquivo solicitado. Se o arquivo não for encontrado,
* é enviado o arquivo 404.html, que se encontra na pasta public.
* @param thread_id: identificador único da thread.
* @param socket_descriptor: socket de conexão do cliente ao servidor.
* @param client_address: tipo de endereço de socket IPv4.
* @param directory: caminho da pasta em que se encontra os arquivos.
* @param root: caminho do arquivo que será aberto e coletado o seu conteúdo.
* @param extension: tipo do arquivo que será lido.
*/
int ServerResponse::get_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory, string root, string extension) {
    string string_aux = "", data = "";
    int length = 0;

    if(Debug){
        cout << "Root: " << root << endl;
    }

    if(arq_stream(root, length, string_aux)){ // Caso arquivo tenha sido encontrado, envie-o.
        data = get_status(1, length, extension);
        data += string_aux;
        if(Debug){
            cout << "\n\nArquivo encontrado: " << data << endl;
        }
        socket_client.send_data(socket_descriptor, data);
        request_receiver(thread_id, socket_descriptor, client_address, directory);
    }
    else{ // Caso contrário, envia o arquivo 404.html
        data = get_status(2, length, "html");
        data += string_aux;
        if(Debug){
            cout << data << endl;
        }
        socket_client.send_data(socket_descriptor, data);
        socket_client.close_socket(socket_descriptor);
    }

    return 0;
}

/**
* Abre e pega o conteúdo do arquivo que será enviado pelo servidor.
* @param root: Caminho do arquivo
* @param length: passado por referencia para ser setada o tamanho do arquivo lido
* @param content_file: passado por referencia para ser setado o conteúdo do arquivo lido
* @return retorna true se o arquivo do caminho passado for aberto. Caso contrário, é retornado false.
*/
bool ServerResponse::arq_stream(string root, int &length, string &content_file) {
    fstream file;
    file.open(root, fstream::in | fstream::out | fstream::binary);

    if(file.is_open()){
        // Posicionando a cabeça de leitura no final do arquivo
        file.seekg(0, ios::end);

        // Tamanho do arquivo lido.
        length = file.tellg();

        read_file(file, content_file, length);

        return true;
    }
    else{
        file.open(NOT_FOUND, fstream::in | fstream::out | fstream::binary);

        if(file.is_open()){
            file.seekg(0, ios::end);

            length = file.tellg();

            read_file(file, content_file, length);
            return false;
        }
        else{
            content_file = "404 NOT FOUND\n";
            length = content_file.size();
        }

        return false;
    }
}

/**
* Realiza a leitura do arquivo e em seguida fecha o mesmo.
* @param file: passado por referencia o arquivo que será lido
* @param content_file: passado por referencia para coletar o conteúdo do arquivo lido
* @param length: tamanho do arquivo que será lido
*/
void ServerResponse::read_file(fstream &file, string &content_file, int length) {
    char *content = new char[length];
    file.clear();
    file.seekg(0, ios::beg);
    file.read(content, length);

    for(int index=0; index<length; index++){
        content_file += content[index];
    }

    if(Debug){
        cout << content_file;
    }

    file.close();
    delete[] content;
    content = nullptr;
}

/**
* Pega a mensagem de cabeçalho e retorna de acordo com o código de status da resposta.
* @param response: opção de resposta 1: OK(200), 2: NOT_FOUND(404), default: BAD_REQUEST(400).
* @param extension: extensão do arquivo.
* @return retorna a mensagem de cabeçalho de acordo com o código de status da resposta.
*/
string ServerResponse::get_status(int response, int length, string extension) {
    char data_buffer[256];
    time_t rawtime;
    struct tm * timeinfo;

    // Setando a data e hora em que a mensagem foi gerada.
    time (&rawtime);
    timeinfo = gmtime (&rawtime);
    strftime(data_buffer, sizeof(data_buffer), "%a, %d %b %Y %H:%M:%S GMT\r\n", timeinfo);

    switch(response){
        case 1:
            return get_message(200, string(data_buffer), length, true, extension);
        case 2:
            return get_message(404, string(data_buffer), length, false, extension);
        default:
            return get_message(400, string(data_buffer), length, false, extension);
    }
}

/**
* Verifica o status da resposta e retorna uma string com a mensagem do status.
* @param status_code: código de status http
* @return retorna a mensagem do status ex: status_code = "200", retorna "OK".
*/
string ServerResponse::check_status(int status_code) {
    const string status_list[3][2] = { {"200", "OK"}, {"404", "Not Found"}, {"400", "Bad Request"} };
    string selected_status = "";
    for(int index=0; index<3; index++){
        if(status_list[index][0] == to_string(status_code)){
            selected_status = status_list[index][1];
        }
    }

    cout << "Status: " << selected_status << endl;
    if(selected_status.size() > 0){
        return selected_status;
    }
}

/**
* Prepara a mensagem de cabeçalho e retorna.
* @param status_code: código de status http.
* @param data_buffer: data e hora em que a mensagem foi gerada.
* @param content_length: tamanho do conteúdo.
* @param use_keep_alive: true se deseja manter a conexão. Caso não deseje manter a conexão utilize false.
* @param extension: Tipo do conteúdo.
* @return retorna uma strinc com a mensagem de cabeçalho.
*/
string ServerResponse::get_message(int status_code, string date_buffer, int content_length, bool use_keep_alive, string extension) {
    const string status = check_status(status_code);
    string content_type = "", message = "";

    for(int index=0; index<14; index++){
        if(mimetype_list[index].get_type() == extension){
            content_type = mimetype_list[index].get_mime_type();
        }
    }

    if(content_type.size() == 0){
        content_type = "application/" + extension;
    }

    if(status_code == STATUS_CODE_OK){
        message = "HTTP/1.1 " + to_string(status_code) + status + "\r\n"
                            "Connection: Keep-Alive\r\n"
                            "Date: " + date_buffer +
                            "Keep-Alive: timeout=4, max=100\r\n"
                            "Content-Length: " + to_string(content_length) + "\r\n"
                            "Content-Type: " + content_type + "\n\r; charset=UTF-8\r\n\r\n";
    }
    else{
        message = "HTTP/1.1 " + to_string(status_code) + status + "\r\n"
                            "Connection: Close\r\n"
                            "Date: " + date_buffer +
                            "Content-Length: " + to_string(content_length) + "\r\n"
                            "Content-Type: " + content_type + "\n\r; charset=UTF-8\r\n\r\n";
    }

    return message;
}
