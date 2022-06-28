//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_READER_H
#define INC_3_READER_H

#include "PublicIncludes.h"

#define READ_BASIC_TYPE(TYPE, VAL) \
    memcpy(&VAL, buffer+offset, sizeof(TYPE)); \
    offset += sizeof(TYPE)

class Reader {
public:
    typedef char byte;
    byte *buffer;
    int offset;

    void readInt(int &val);


    Reader();
};

#endif //INC_3_READER_H
