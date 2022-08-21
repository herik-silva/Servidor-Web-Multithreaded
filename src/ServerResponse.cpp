#include "ServerResponse.h"

#define Debug false

void ServerResponse::request_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory) {
    string request = "";
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

        cout << "1"<<endl;
        headers = split_string(request, '\n');
        cout << "2"<<endl;
        headers = split_string(headers[0], ' ');
        cout << "3"<<endl;
        extension_list = split_string(headers[1], '.');
        try{
            if(headers[0] == "GET"){
                if(Debug){
                    cout << "Uma requisiao GET" << endl;
                    cout << "Pasta desejada: " << headers[1] << endl;
                    cout << "Extensao: " << extension_list[extension_list.size()-1] << endl;
                }

                get_receiver(thread_id, socket_descriptor, client_address, directory, directory+headers[1], extension_list[extension_list.size()-1]);
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

vector<string> ServerResponse::split_string(string value, char break_point) {
    string aux_string = "";
    vector<string> header_content;
    for(int index=0; index<(int)value.size(); index++){
        if(value[index] != break_point){
            aux_string += value[index];
        }
        else{
            header_content.push_back(aux_string);
            aux_string = "";
        }
    }

    header_content.push_back(aux_string);
    cout << "\n\n\n" << endl;
    cout << header_content[0] << endl;
    cout << "\n\n\n" << endl;

    return header_content;
}

void ServerResponse::get_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory, string root, string extension) {
    string string_aux = "", data = "";
    int length = 0;

    if(Debug){
        cout << "Root: " << root << endl;
    }

    if(arq_stream(root, length, string_aux)){ // Caso o arquivo tenha sido encontrado, envie o arquivo.
        data = get_status(1, length, extension);
        data += string_aux;
        socket_client.send_data(socket_descriptor, data);
        request_receiver(thread_id, socket_descriptor, client_address, directory);
    }
    else{ // Caso contrário, envia o arquivo NOT_FOUND.html
        data = get_status(2, length, extension);
        data += string_aux;
        socket_client.send_data(socket_descriptor, data);
        socket_client.close_socket(socket_descriptor);
    }
}

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

string ServerResponse::get_status(int response, int length, string extension) {
    char data_buffer[256];
    time_t rawtime;
    struct tm * timeinfo;

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

    return "TESTE";
}

string ServerResponse::get_message(int status_code, string date_buffer, int content_length, bool use_keep_alive, string extension) {
    if(Debug){
        cout << "PEGANDO MENSAGEM" << endl;
    }
    const string status = check_status(status_code);
    string content_type = "", text = "";

    for(int index=0; index<14; index++){
        if(mimetype_list[index].get_type() == extension){
            content_type = mimetype_list[index].get_mime_type();
        }
    }

    if(content_type.size() == 0){
        content_type = "application/" + extension;
    }

    if(status_code == STATUS_CODE_OK){
        text = "HTTP/1.1 " + to_string(status_code) + status + "\r\n"
                            "Connection: Keep-Alive\r\n"
                            "Date: " + date_buffer +
                            "Keep-Alive: timeout=4, max=100\r\n"
                            "Content-Length: " + to_string(content_length) + "\r\n"
                            "Content-Type: " + content_type + "\n\r; charset=UTF-8\r\n\r\n";
    }
    else{
        text = "HTTP/1.1 " + to_string(status_code) + status + "\r\n"
                            "Connection: Close\r\n"
                            "Date: " + date_buffer +
                            "Content-Length: " + to_string(content_length) + "\r\n"
                            "Content-Type: " + content_type + "\n\r; charset=UTF-8\r\n\r\n";
    }

    return text;
}









