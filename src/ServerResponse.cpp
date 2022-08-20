#include "ServerResponse.h"

#define Debug false

ServerResponse::ServerResponse() {}

ServerResponse::~ServerResponse() {
    //dtor
}

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

    Databuff receiver_data = socket_client->receiver_socket(socket_descriptor);
    if(receiver_data.get_buffer_length() > 0){
        request = string(receiver_data.get_content(), 0, receiver_data.get_buffer_length());
        if(Debug){
            cout<< request << endl;
        }

        headers = split(request, '\n');
        headers = split(headers[0], ' ');
        extension_list = split(headers[1], '.');

        try{
            if(headers[0] == "GET"){
                if(Debug){
                    cout << "Uma requisiao GET" << endl;
                    cout << "Pasta desejada: " << header[1] << endl;
                    cout << "Extensao: " << extension_list[extension_list.size()-1] << endl;
                }

                // receiver_get(...)
            }
            else{
                throw(headers[0]);
            }
        }
        catch(string error){
            string data = "400 Bad Request";
            int length = aux.size();
            data += //getStatus(3, length, "html");

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

void ServerResponse::get_receiver(int thread_id, int socket_descriptor, sockaddr_in client_address, string directory, string root, string extension) {

}
