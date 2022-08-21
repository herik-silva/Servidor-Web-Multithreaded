#ifndef MIMETYPE_H
#define MIMETYPE_H

#include <string>

using namespace std;

class MimeType {
    public:
        MimeType(string type, string value);
        virtual ~MimeType();
        string get_mime_type();
        string get_type();

    private:
        string type;
        string value;
};

#endif // MIMETYPE_H
