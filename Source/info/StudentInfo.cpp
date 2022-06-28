//
// Created by Flyoung on 2022/6/27.
//

#include "StudentInfo.h"

vector <string> StudentInfo::to_string_vec() {
    return {to_string(sno), sname, to_string(sage)};
}