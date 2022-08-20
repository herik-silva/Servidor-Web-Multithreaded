#ifndef RESPONSESERVER_H
#define RESPONSESERVER_H

#include "Socket.h"
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "MimeType.h"

using namespace std;

class ResponseServer
{
    public:
        ResponseServer() {};
        virtual ~ResponseServer() {};
        //Função para estabelecimento da conexão com o cliente
        void handleRequest(int thread_id, int clientSockfd, sockaddr_in clientAddr, string dir);
        //Função de fornecimento de arquivos solicitados
        int handleGet(int thread_id, int clientSockfd, sockaddr_in clientAddr, string root, string extension, string dir);
        vector<string> split(string str, char del);//Carregamento dos dados do arquivo
        bool arqStream(string root, int &len, string &n); //Abertura do arquivo solicitado
        void readFile(fstream &file, string &n, int len); //Leitura dos dados
        string getStatus(int resp, int len, string extension); //Definição do status de resposta
        int	operator()(int thread_id, int clientSockfd, sockaddr_in clientAddr, string dir) //Operador de threads
        {
            handleRequest(thread_id, clientSockfd, clientAddr, dir);
            return (0);
        }
    private:
        Socket socli;
        MimeType mimetype_list[14] = {
            MimeType("au", "audio/basic"),
            MimeType("mp3", "audio/mpeg"),
            MimeType("wav", "audio/wav"),
            MimeType(".ico", "image/x-icon"),
            MimeType("gif", "image/gif"),
            MimeType("jpeg", "image/jpeg"),
            MimeType("jpg", "image/jpg"),
            MimeType("png", "image/png"),
            MimeType("css", "text/css"),
            MimeType("html", "text/html"),
            MimeType("js", "text/js"),
            MimeType("txt", "text/txt"),
            MimeType("avi", "video/x-msvideo"),
            MimeType("pdf", "application/pdf")
        };
};

#endif // RESPONSESERVER_H
