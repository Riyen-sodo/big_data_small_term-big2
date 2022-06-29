//
// Created by Flyoung on 2022/6/28.
//

#include <Writer.h>
#include <cstdio>
#include "errors.h"

Writer::Writer() {
    buffer = (byte *) calloc(1, BLOCK_SIZE);
    offset = 0;
}

void Writer::writeInt(int val) {
    WRITE_BASIC_TYPE(int, val);
}

void Writer::writeString(string str) {
    int strSize = str.size() + 1;
    memcpy(buffer + offset, str.c_str(), strSize);
    offset += strSize;
}

void Writer::save(string filePath) {
    filePath = "./db_folder/" + filePath;
    FILE *fp = fopen(filePath.c_str(), "ab");
    if (fp == nullptr) {
        exit(FILE_OPEN_ERROR);
    }

    // fwrite : return _Count
    if (fwrite(buffer, BLOCK_SIZE, 1, fp) != 1) {
//        throw FileWriteError(columnName);
        exit(FILE_WRITE_ERROR);
    }
    if (fclose(fp) != 0) {
//        throw FileCloseError(columnName);
        exit(FILE_CLOSE_ERROR);
    }
}

Writer::~Writer() {
    if (this->buffer) {
        free(this->buffer);
    }
}









