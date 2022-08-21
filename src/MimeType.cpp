#include "MimeType.h"

MimeType::MimeType(string type, string value) {
    this->type = type;
    this->value = value;
}

MimeType::~MimeType() {}

string MimeType::get_mime_type() {
    return value;
}

string MimeType::get_type() {
    return type;
}
