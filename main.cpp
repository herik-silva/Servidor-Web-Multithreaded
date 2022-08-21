#include <string>
#include <iostream>
#include <HttpServer.h>

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

    return 0;
}
