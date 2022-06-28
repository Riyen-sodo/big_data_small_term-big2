//
// Created by Flyoung on 2022/6/27.
//
#include "PublicIncludes.h"
#include "InfoObject.h"

#ifndef DEMO2_TABLEINFO_H
#define DEMO2_TABLEINFO_H

/**
 * 对应一行表的信息
 * table_name : 表名
 * table_id : 表的 id
 * table_attr_cnt : 表的属性数量
 */
struct TableInfo : public InfoObject {
    string table_name;
    int table_id;
    int table_attr_cnt;

    TableInfo() {}

    TableInfo(
            string table_name,
            int table_id,
            int table_attr_cnt
    ) :
            table_name(table_name),
            table_id(table_id),
            table_attr_cnt(table_attr_cnt) {}

    vector<string> to_string_vec();

    void print();
};

#endif //DEMO2_TABLEINFO_H
