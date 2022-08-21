#include "Formatter.h"

string Formatter::prepare_arg(char* arg[]) {
    int index = 1;
    string directory = "";

    while(arg[index]){
        directory += arg[index];
        index++;
    }

    return directory;
}
