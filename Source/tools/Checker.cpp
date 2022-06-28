//
// Created by Flyoung on 2022/6/28.
//

#include <Checker.h>

Checker::Checker() {
    comsumed = 0;
}

void Checker::feed(int size) {
    this->comsumed += size;
    if (comsumed >= BLOCK_SIZE) {
        throw BlockSpaceError();
    }
}
