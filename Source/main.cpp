#include "database.h"
#include "Dbms.h"
#include "Writer.h"
#include "TableManager.h"
#include "Reader.h"

void get_info(vector<TableInfo> &table_info_vec, vector<AttributeInfo> &attr_info_vec);

void task2();

int main() {
    system("chcp 65001");

    int table_id = 1001;
    Block block;
    block.insert({"hello", "cpp"}, vector<string>(2, ""));
    block.insert({"good", "bug"}, vector<string>(2, ""));
    TableManager tableManager1;
    TableManager tableManager2;
    tableManager1.tableId = 1001;
    tableManager1.blocks.emplace_back(block);

    try {
        tableManager1.save();
    } catch (FileOpenError &e) {
        cout << e.error() << endl;
    }
    tableManager2.load(to_string(tableManager1.tableId));

    tableManager2.blocks[0].print();
    return 0;
}

/*
void task2() {
    int pg_class_id = 1001;
    int pg_attribute_id = 1002;
    int pg_student_id = 1003;
    BlockManager pg_class(pg_class_id);
    BlockManager pg_attribute(pg_attribute_id);
    BlockManager pg_student(pg_student_id);
    int insert_times = 200;
    // 创建表
    {
        vector<TableInfo> table_info_vec;
        vector<AttributeInfo> attr_info_vec;
        get_info(table_info_vec, attr_info_vec);


        // 创建 pg_class 与 pg_attribute 表
        for (TableInfo &tableInfo : table_info_vec) {
            vector<string> pg_class_data_vec = tableInfo.to_string_vec();
            pg_class.insert(pg_class_data_vec, vector<string>(pg_class_data_vec.size(), ""));
        }


        for (AttributeInfo &attributeInfo : attr_info_vec) {
            vector<string> pg_attr_data_vec = attributeInfo.to_string_vec();
            pg_attribute.insert(pg_attr_data_vec, vector<string>(pg_attr_data_vec.size(), ""));
        }

        // 创建学生表
        for (int i = 0; i < insert_times; i++) {
            StudentInfo studentInfo(i + 1, get_random_name(4), int(rand() % 10 + 18));
            vector<string> pg_stu_data_vec = studentInfo.to_string_vec();
            pg_student.insert(pg_stu_data_vec, vector<string>(pg_stu_data_vec.size(), ""));
        }
    }
//
//     将表保存到文件中
    {
        BlockManager::to_file(pg_class);
        BlockManager::to_file(pg_attribute);
        BlockManager::to_file(pg_student);
    }

    // 从文件中读取表，并打印
    {
        BlockManager pg_class_buffer;
        BlockManager::from_file(pg_class_buffer, pg_class_id);
        pg_class_buffer.print();

        BlockManager pg_attribute_buffer;
        BlockManager::from_file(pg_attribute_buffer, pg_attribute_id);
        pg_attribute_buffer.print();

        BlockManager pg_student_buffer;
        BlockManager::from_file(pg_student_buffer, pg_student_id);
        pg_student_buffer.print();
    }
}

void get_info(
        vector<TableInfo> &table_info_vec,
        vector<AttributeInfo> &attr_info_vec
) {
    int pg_class_id = 1001;
    int pg_attribute_id = 1002;
    int pg_student_id = 1003;

    vector<TableInfo> table_info_vec_template = {
            TableInfo("pg_class", pg_class_id, 3),
            TableInfo("pg_attribute", pg_attribute_id, 6),
            TableInfo("pg_student", pg_student_id, 3)
    };


    vector<AttributeInfo> attr_info_vec_template = {
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
*/