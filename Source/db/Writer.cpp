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
    FILE *fp = fopen(filePath.c_str(), "wb");
    if (fp == nullptr) {
        throw FileOpenError(filePath);
    }
    cout << "fwrite : " << fwrite(buffer, BLOCK_SIZE, 1, fp) << endl;
//    if ( != BLOCK_SIZE) {
//        throw FileWriteError(filePath);
//    }
    if (fclose(fp) != 0) {
        throw FileCloseError(filePath);
    }
}

Writer::~Writer() {
    free(buffer);
}









