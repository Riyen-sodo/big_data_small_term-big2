//
// Created by Flyoung on 2022/6/28.
//

#include "Reader.h"

void Reader::readInt(int &val) {
    READ_BASIC_TYPE(int, val);
}

Reader::Reader() {
    buffer = (byte *) calloc(1, BLOCK_SIZE);
    offset = 0;
}

Reader::~Reader() {
    if (this->buffer) {
        free(this->buffer);
    }
}


