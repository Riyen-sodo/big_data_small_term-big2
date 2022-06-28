//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_FILTEREXPRESSION_H
#define INC_3_FILTEREXPRESSION_H

#include "PublicIncludes.h"
#include "utils.h"
#include "errors.h"
struct FilterExpression {
    string column_name;
    char comparator;
    string value;
    const char spliter = ' ';
    /**
     * 根据字符串初始化表达式
     * @param str
     */
    FilterExpression(string str);

    FilterExpression();

    void print();
};

#endif //INC_3_FILTEREXPRESSION_H
