//
// Created by Flyoung on 2022/6/30.
//

#ifndef INC_3_BLOCKREADER_H
#define INC_3_BLOCKREADER_H

#include "PublicIncludes.h"

#define READ_BASIC_TYPE(TYPE, VAL) \
    memcpy(&VAL, buffer+offset, sizeof(TYPE)); \
    offset += sizeof(TYPE)

class BlockReader {
public:
    typedef char byte;
    int bufferSize;
    byte *buffer;
    int offset;
    vector<string> cache;
    bool isVisit;

    void readInt(int &val);

    bool is_space_not_enough(int sizeWillBeWrite);

    BlockReader(int bufferSize);

    void set_cache(vector<string> data);

    ~BlockReader();
};

#endif //INC_3_BLOCKREADER_H
