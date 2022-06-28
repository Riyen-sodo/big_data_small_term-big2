//
// Created by Flyoung on 2022/6/28.
//

#include "FilterExpression.h"

FilterExpression::FilterExpression(string str) {
    vector<string> formatStr = split_string(str, this->spliter);
    if (formatStr.size() != 3 || formatStr[1].size() != 1) {
        throw ExpressionError();
    }
    this->column_name = formatStr[0];
    this->comparator = formatStr[1][0];
    this->value = formatStr[2];
}

FilterExpression::FilterExpression() {

}

void FilterExpression::print() {
    cout << "column name : " << column_name << endl;
    cout << "comparator : " << comparator << endl;
    cout << "value : " << value << endl;
}

