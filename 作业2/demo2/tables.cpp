#include "heads.h"

struct InfoObject {
    virtual vector <string> to_string_vec() { return vector<string>(); }

    void print() {
        for (string s : this->to_string_vec()) {
            cout << s << endl;
        }
    }
};

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

    vector <string> to_string_vec() {
        vector <string> ret;
        ret.push_back(this->table_name);
        ret.push_back(std::to_string(this->table_id));
        ret.push_back(std::to_string(this->table_attr_cnt));
        return ret;
    }

    void print() {
        printf("table_name : %s\ntable_id : %d\ntable_attr_cnt : %d\n", this->table_name.c_str(), this->table_id,
               this->table_attr_cnt);
    }
};

/**
 * 对应一个属性（字段）的信息
 * table_name : 所属表名
 * table_id : 所属表的 id
 * column_name : 字段名
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

    vector <string> to_string_vec() {
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
};



void get_info(
        vector <TableInfo> &table_info_vec,
        vector <AttributeInfo> &attr_info_vec
) {
    int pg_class_id = 1001;
    int pg_attribute_id = 1002;
    int pg_student_id = 1003;

    vector <TableInfo> table_info_vec_template = {
            TableInfo("pg_class", pg_class_id, 3),
            TableInfo("pg_attribute", pg_attribute_id, 6),
            TableInfo("pg_student", pg_student_id, 3)
    };


    vector <AttributeInfo> attr_info_vec_template = {
            // pg_class 表的属性
            AttributeInfo("pg_class", pg_class_id, "table_name", "string", 20, 1, false),
            AttributeInfo("pg_class", pg_class_id, "table_id", "int", 4, 2, false),
            AttributeInfo("pg_class", pg_class_id, "table_attr_cnt", "string", 20, 3, false),

            // pg_attribute 表的属性
            AttributeInfo("pg_attribute", pg_attribute_id, "table_name", "string", 20, 1, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "table_id", "int", 4, 2, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "column_name", "string", 20, 3, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "column_type", "string", 20, 4, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "column_bytes_len", "int", 4, 5, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "column_index", "int", 4, 6, false),
            AttributeInfo("pg_attribute", pg_attribute_id, "is_null", "bool", 1, 7, false),

            // pg_student 表的属性
            AttributeInfo("pg_student", pg_student_id, "sno", "int", 4, 1, false),
            AttributeInfo("pg_student", pg_student_id, "sname", "string", 20, 2, false),
            AttributeInfo("pg_student", pg_student_id, "sage", "int", 4, 3, false)
    };

    table_info_vec = table_info_vec_template;
    attr_info_vec = attr_info_vec_template;
}


struct StudentInfo : public InfoObject {
    int sno;
    string sname;
    int sage;

    StudentInfo() {}

    StudentInfo(int sno, string sname, int sage) : sno(sno), sname(sname), sage(sage) {}

    vector <string> to_string_vec() {
        return {to_string(sno), sname, to_string(sage)};
    }
};