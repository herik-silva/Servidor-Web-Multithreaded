/**
* TRABALHO FINAL DE REDES
* --------------------------------------
* Servidor Web Multithreads
* Disciplina: Redes de Computadores
* Professor: Samuel Pereira Dias
* Alunos:
*  - Carlos Eduardo de Sousa: 0048507
*  - Herik Aparecida Ramos da Silva: 0048843
*  - Jean Lucio Campos: 0048042
*/

#include <iostream>
#include <HttpServer.h>
#include <Socket.h>

using namespace std;

int main(int argc, char* argv[]) {
    if(argv[1]){
        string directory = argv[1];
        int port = 5050;

        if(argv[2]){
            port = stoi(argv[2]);
        }

        cout << "Path: " << directory << endl;
        cout << "Port: " << port << endl;

        HttpServer server = HttpServer("0.0.0.0", port, directory);
        server.init_server();

        return 0;
    }

    cout << "Informe o caminho da pasta publica" << endl;
    return -1;
}
