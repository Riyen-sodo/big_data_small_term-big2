//
// Created by Flyoung on 2022/6/28.
//

#include "FilterExpression.h"

FilterExpression::FilterExpression(string str) {
    vector<string> formatStr = split_string(str, this->spliter);
    if (formatStr.size() != 3 || formatStr[1].size() != 1) {
        throw ExpressionError();
    }
    {
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
    this->column_name = formatStr[0];
    this->filter = formatStr[1][0];
    this->value = formatStr[2];
}

FilterExpression::FilterExpression() {

}

void FilterExpression::print() {
    cout << "column name : " << column_name << endl;
    cout << "filter : " << filter << endl;
    cout << "value : " << value << endl;
}

bool FilterExpression::check(string columnVal) {
    float filterVal = std::stof(value);
    float val = std::stof(columnVal);
    if (filter == '>') {
        return val > filterVal;
    } else if (filter == '=') {
        return val == filterVal;
    }
    return val < filterVal;
}





