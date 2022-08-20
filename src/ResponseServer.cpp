#include "ResponseServer.h"

#define Debug false

void ResponseServer::handleRequest(int thread_id, int clientSockfd, sockaddr_in clientAddr, string dir){

    if(Debug){
       cout << "ID THREAD: " << thread_id << endl; //Exibição de quantidade de threads abertas
    }
    string strRequest = "";
    vector<string> extension;
    vector<string> linesHeader;

    if (clientSockfd == -1) { //Caso não haja um socket de conexão do cliente não é estabelecida conexão com o servidor
        return;
    }

    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr)); //Conversão de endereço em forma de string par endereço web
    if(Debug){
        cout << "Conexão iniciada com o cliente " << ipstr << ":" << ntohs(clientAddr.sin_port) << endl << endl;
    }
    databuff datarec = socli.sockrecv(clientSockfd);
    if(datarec.bufflen > 0){ //Caso haja requisição esta será tratada
        strRequest = string(datarec.dataChar, 0, datarec.bufflen);
        if(Debug){
            cout<< strRequest << endl;
        }
        linesHeader = split(strRequest,'\n');
        linesHeader = split(linesHeader[0],' ');
        extension = split(linesHeader[1],'.');

        try { //Captura de possíveis erros para definição de Bad request
            if(linesHeader[0] == "GET"){ //Tratamento de requisições GET
                if(Debug){
                    cout<<"é uma requisição get"<<endl;
                    cout<<"Pasta Desejada: "<<linesHeader[1]<<endl;
                    cout<<"extensão: "<<extension[extension.size()-1]<<endl;
                }
                //Chamada da função que irá tratar a requisição
                handleGet(thread_id,clientSockfd,clientAddr, dir+linesHeader[1],extension[extension.size()-1], dir);
            }
            else {
                throw (linesHeader[0]);
            }
        } catch(string n) { //Caso não seja uma requisição adequada será retornada a resposta de bad request ao cliente
            int len = 0;
            string aux = "", data="";
            aux = "400 Bad Request\n\r";
            len = aux.size();
            data = getStatus(3, len, "html"); //Obtenção do status da requisição
            if(Debug){
                cout << data << endl;
            }
            data += aux;
            socli.socksendk(clientSockfd, data);
            socli.closesock(clientSockfd);
        }
    }
    else {
        if(Debug){
            cout<<"Tempo limite de requisição atingido!"<<endl;
        }
        this->socli.closesock(clientSockfd); //Fechamento do socket caso o tempo limite de conexão seja atingido
    }
}

//Carregamento dos dados para posterior disponibilização ao cliente
vector<string> ResponseServer::split(string str, char del){

    string temp = "";
    vector<string> stringsHeader;
    for(int i =0; i < (int)str.size(); i++){
    if(str[i] != del){
            temp += str[i];
        }else{
            stringsHeader.push_back(temp);
            temp = "";
        }
    }
    stringsHeader.push_back(temp);
    return stringsHeader;
}

//abertura do arquivo solicitado
int ResponseServer::handleGet(int thread_id, int clientSockfd, sockaddr_in clientAddr, string root,string extension, string dir){
    string n, data;
    int len;
    if(Debug){
        cout<< "Root " << root << endl;
    }
    n = data = "";
    if(arqStream(root, len, n)) {
        data = getStatus(1, len, extension); //Caso o fornecimento do arquivo seja efetivo será exibido o status 200
        data += n;
        socli.socksendk(clientSockfd, data); //Envio da resposta para o cliente
        handleRequest(thread_id, clientSockfd,clientAddr, dir);
    } else {
        data = getStatus(2, len, "html"); //Caso não haja o arquivo solicitado será retornado o status 404
        data+= n;
        socli.socksendk(clientSockfd, data);
        socli.closesock(clientSockfd);
    }
    return 0;
}

bool ResponseServer::arqStream(string root, int &len, string &n) { //Abertura do arquivo
    fstream file;
    file.open(root, fstream::in |fstream::out | fstream::binary);

    if(file.is_open()){
        file.seekg(0, ios::end); //Posicionamento da cabeça de leitura ao final do arquivo
        len = file.tellg(); // Determinação do tamanho do arquivo
        readFile(file, n, len); //Leitura dos dados
        return true;
    }
    else {
        //Caso o arquivo não seja aberto um arquivo padrão com erro 404 será aberto
        file.open("pasta/404.html", fstream::in |fstream::out | fstream::binary);
        if(file.is_open()) {
            file.seekg(0, ios::end);
            len = file.tellg();
            readFile(file, n, len);
        } else {
            n = "404 Not Found\n\r";
            len = n.size();
        }
        return false;
    }
}

//Leitura dos dados do arquivo solicitado
void ResponseServer::readFile(fstream &file, string &n, int len) {
    char *aux =  new char[len];
    file.clear();
    file.seekg(0, ios::beg);
    file.read(aux, len);
    for(int i = 0; i < len; i++){
        n += aux[i];
    }
    file.close();
    delete[] aux;
    aux = nullptr;
}

//Definição do status da requisição
string ResponseServer::getStatus(int resp, int len, string extension) {
    string text;
    MimeType mim;
    char buff[256];
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = gmtime (&rawtime);
    strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S GMT\r\n", timeinfo);

    if(resp == 1) { // Caso o parâmetro de resp seja 1, a requisição resultou em sucesso e o status é definido
        text = "HTTP/1.1 200 OK\r\n"
               "Date: " + string(buff) +
               "Content-Length: " + to_string(len) + "\r\n"
               "Keep-Alive: timeout=5, max=100\r\n"
               "Connection: Keep-Alive\r\n"
               "Content-type: " + mim.getType(extension) + "; charset=UTF-8\r\n\r\n";
    }
    else if(resp == 2){ // Caso o parâmetro de resp seja 2, o arquivo solicitado não foi encontrado
        text = "HTTP/1.1 404 Not Found\r\n"
                "Date: " + string(buff) +
                "Content-Length: " + to_string(len) + "\r\n"
                "Connection: Close\r\n"
                "Content-type: " + mim.getType(extension) + "; charset=UTF-8\r\n\r\n";
    } else {
        text = "HTTP/1.1 400 Bad Request\r\n" //Caso o status seja 3, ou qualquer outro valor houve uma Bad request
                "Date: " + string(buff) +
                "Content-Length: " + to_string(len) + "\r\n"
                "Connection: Close\r\n"
                "Content-type: " + mim.getType(extension) + "; charset=UTF-8\r\n\r\n";
    }
    return text;
}
