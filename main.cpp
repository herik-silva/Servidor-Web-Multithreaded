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

string prepare_arg(char* arg[]) {
    int index = 1;
    string directory = "";

    while(arg[index]){
        directory += arg[index];
        index++;
    }

    return directory;
}

int main(int argc, char* argv[]) {
    string directory = prepare_arg(argv);
    cout << "Path: " << directory << endl;

    HttpServer server = HttpServer("0.0.0.0", 5050, directory);
    server.init_server();

    return 0;
}
