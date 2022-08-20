#include "Databuff.h"

Databuff::Databuff() {
    memset(content, '\0', sizeof(MAX_BUFFER_LENGTH));
    buffer_length = 0;
}

Databuff::~Databuff() {
    //dtor
}

char* Databuff::get_content() {
    return content;
}

int Databuff::get_buffer_length() {
    return buffer_length;
}

int Databuff::get_max_buffer_length() {
    return MAX_BUFFER_LENGTH;
}

void Databuff::set_buffer_length(int buffer_length) {
    this->buffer_length = buffer_length;
}
