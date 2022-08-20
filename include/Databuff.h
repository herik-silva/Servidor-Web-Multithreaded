#ifndef DATABUFF_H
#define DATABUFF_H

#define MAX_BUFFER_LENGTH 4090

#include <string.h>

class Databuff {
    public:
        Databuff();
        virtual ~Databuff();
        char* get_content();
        int get_buffer_length();
        int get_max_buffer_length();
        void set_buffer_length(int buffer_length);
    protected:

    private:
        int buffer_length;
        char content[MAX_BUFFER_LENGTH];
};

#endif // DATABUFF_H
