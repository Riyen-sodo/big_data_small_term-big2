//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_FILTEREXPRESSION_H
#define INC_3_FILTEREXPRESSION_H

#include "PublicIncludes.h"
#include "utils.h"
#include "errors.h"
#include "Comparator.h"

struct FilterExpression {
    string attribute_name;
    char filter;
    string value;
    const char spliter = ' ';

    /**
     * 根据字符串初始化表达式
     * @param str
     */
    FilterExpression(string str);

    FilterExpression();

    void print();

    /**
     * 检查列值是否复合条件
     * @param type 列的类型
     * @param columnValue 列值
     * @param filterValue 过滤值
     * @return
     */
    bool check(string columnVal);
};


#endif //INC_3_FILTEREXPRESSION_H
