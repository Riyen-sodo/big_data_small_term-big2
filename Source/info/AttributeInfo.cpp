//
// Created by Flyoung on 2022/6/27.
//

#include "AttributeInfo.h"

vector <string> AttributeInfo::to_string_vec() {
    vector <string> ret;
    ret.push_back(this->table_name);
    ret.push_back(std::to_string(this->table_id));
    ret.push_back(this->column_name);
    ret.push_back(this->column_type);
    ret.push_back(std::to_string(this->column_bytes_len));
    ret.push_back(std::to_string(this->column_index));
    ret.push_back(std::to_string(this->is_null));
    return ret;
}
