//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_WRITER_H
#define INC_3_WRITER_H

#include "PublicIncludes.h"

#define BLOCK_SIZE 8192
#define WRITE_BASIC_TYPE(TYPE, VAL) \
    memcpy(buffer+offset, &VAL, sizeof(TYPE)); \
    offset += sizeof(TYPE)



class Writer {

public:
    typedef char byte;

    Writer();

    void writeInt(int val);

    void writeString(string str);

    void save(string filePath);

    ~Writer();

//private:
    byte *buffer;
    int offset;

};

#endif //INC_3_WRITER_H
