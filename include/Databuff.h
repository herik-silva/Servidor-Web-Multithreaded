#ifndef DATABUFF_H
#define DATABUFF_H

#define MAX_BUFFER_LENGTH 4090

class Databuff {
    public:
        Databuff();
        virtual ~Databuff();

    protected:

    private:
        int buffer_length;
        char content[MAX_BUFFER_LENGTH];
};

#endif // DATABUFF_H
