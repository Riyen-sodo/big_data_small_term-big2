//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_ATTRIBUTEINFO_H
#define DEMO2_ATTRIBUTEINFO_H

#include "PublicIncludes.h"
#include "InfoObject.h"
/**
 * 对应一个属性（字段）的信息
 * table_name : 所属表名
 * table_id : 所属表的 id
 * attribute_name : 字段名
 * column_type : 字段类型
 * column_bytes_len : 字段长度
 * column_index : 字段在第几列
 * is_null : 字段是否为空
 */
struct AttributeInfo : public InfoObject {
    string table_name;
    int table_id;
    string column_name;
    string column_type;
    int column_bytes_len;
    int column_index;
    bool is_null;

    AttributeInfo() {}

    AttributeInfo(
            string table_name,
            int table_id,
            string column_name,
            string column_type,
            int column_bytes_len,
            int column_index,
            bool is_null
    ) :
            table_name(table_name),
            table_id(table_id),
            column_name(column_name),
            column_type(column_type),
            column_bytes_len(column_bytes_len),
            column_index(column_index),
            is_null(is_null) {}

    vector <string> to_string_vec();
};

#endif //DEMO2_ATTRIBUTEINFO_H
