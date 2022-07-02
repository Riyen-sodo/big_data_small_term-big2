//
// Created by Flyoung on 2022/6/28.
//

#include "FilterExpression.h"

FilterExpression::FilterExpression(string str) {
    // a > 5
    // a#>#5
    vector<string> formatStr = split_string(str, this->spliter);

    // 异常处理
    {
        // 表达式是否满足  [ attr filter value ]
        if (formatStr.size() != 3 || formatStr[1].size() != 1) {
            throw ExpressionError();
        }

        // 过滤是否规范
        bool isFilterSignValid = false;
        char signs[] = {'<', '=', '>'};
        for (int i = 0; i < sizeof(signs) / sizeof(char); i++) {
            if (formatStr[1][0] == signs[i]) {
                isFilterSignValid = true;
                break;
            }
        }
        if (!isFilterSignValid) {
            throw ExpressionError();
        }
    }

    // 初始化
    this->attribute_name = formatStr[0];
    this->filter = formatStr[1][0];
    this->value = formatStr[2];
}

FilterExpression::FilterExpression() {

}

void FilterExpression::print() {
    cout << "column name : " << attribute_name << endl;
    cout << "filter : " << filter << endl;
    cout << "value : " << value << endl;
}

bool FilterExpression::check(string columnVal) {
    // 类型转换
    float filterVal = std::stof(value);
    float val = std::stof(columnVal);

    if (filter == '>') {
        return val > filterVal;
    } else if (filter == '=') {
        return val == filterVal;
    }
    return val < filterVal;
}





