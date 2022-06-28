//
// Created by Flyoung on 2022/6/28.
//

#include <Checker.h>


vector<string> Checker::split_string(string str, char spliter) {
    vector<string> ret;
    int pre = 0;
    for (int i = 0; i < str.size(); i++) {
        if (i != str.size() - 1 && str[i] == spliter) {
            ret.emplace_back(str.substr(pre, i));
            pre = i + 1;
        }
    }
    return ret;
}
