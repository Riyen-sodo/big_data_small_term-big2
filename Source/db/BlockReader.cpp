//
// Created by Flyoung on 2022/6/30.
//

#include <BlockReader.h>

void BlockReader::readInt(int &val) {
    READ_BASIC_TYPE(int, val);
}

BlockReader::BlockReader(int bufferSize) {
    this->bufferSize = bufferSize;
    this->buffer = (byte *) calloc(1, bufferSize);
    this->isVisit = false;
    this->offset = 0;
}

bool BlockReader::is_space_not_enough(int sizeWillBeWrite) {
    return (this->bufferSize - this->offset) > sizeWillBeWrite;
}

