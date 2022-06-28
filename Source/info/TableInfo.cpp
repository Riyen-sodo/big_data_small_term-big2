//
// Created by Flyoung on 2022/6/27.
//
#include "TableInfo.h"

void TableInfo::print() {
    printf("table_name : %s\ntable_id : %d\ntable_attr_cnt : %d\n", this->table_name.c_str(), this->table_id,
           this->table_attr_cnt);
}

vector <string> TableInfo::to_string_vec() {
    vector <string> ret;
    ret.push_back(this->table_name);
    ret.push_back(std::to_string(this->table_id));
    ret.push_back(std::to_string(this->table_attr_cnt));
    return ret;
}