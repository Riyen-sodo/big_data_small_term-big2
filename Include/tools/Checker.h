//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_CHECKER_H
#define INC_3_CHECKER_H

#include "PublicIncludes.h"
#include "SizeCalculator.h"
#include "utils.h"
#include "errors.h"
class Checker {
public:
    int comsumed;

    Checker();

    void feed(int size);
};

#endif //INC_3_CHECKER_H
