#include "database.h"
#include "Dbms.h"
#include "TableManager.h"
#include "FilterExpression.h"
#include "Comparator.h"
#include "Sorter.h"
#include "FilePtrManager.h"
#include "TupleDataComparable.h"
#include "BlockReader.h"

#define SHOW_BLOCK_NUM 1<<1

const int run_mode = 1 << 0;

void get_info(vector<TableInfo> &table_info_vec, vector<AttributeInfo> &attr_info_vec);

void task2();

void task3();

int main() {
    system("chcp 65001");
    system("echo  program start  *\\(.'_'.)/*   ");
//    task2();
//    task3();



    // TODO task 4
//    TableManager pg_student(1003);
//    int pg_student_block_num;
//    pg_student.load("1003", true, pg_student_block_num, 0);
//
//    DatabaseManagerSystem dbms;
//    dbms.mount({pg_student});
//
//    dbms.sort(pg_student, to_string(pg_student.tableId), 1005, 2);
    return 0;
}

/**
 * 实验3
 */
void task3() {
    TableManager pg_class(1001);
    TableManager pg_attribute(1002);
    TableManager pg_student(1003);
    int pg_class_block_num;
    int pg_attribute_block_num;
    int pg_student_block_num;
    pg_class.load("1001", true, pg_class_block_num, 0);
    pg_attribute.load("1002", true, pg_attribute_block_num, 0);
    pg_student.load("1003", true, pg_student_block_num, 0);

    DatabaseManagerSystem dbms;
    dbms.mount({pg_class, pg_attribute, pg_student});
    dbms.select("pg_student", {"sno", "sage"}, FilterExpression("sno > 990"), pg_class, pg_attribute);
}

/**
 * 实验2
 * 生成所有表
 */
void task2() {
    int pg_class_id = 1001;
    int pg_attribute_id = 1002;
    int pg_student_id = 1003;
    TableManager pg_class(pg_class_id);
    TableManager pg_attribute(pg_attribute_id);
    TableManager pg_student(pg_student_id);
    int insert_times = 1000;
    // 创建表
    {
        vector<TableInfo> table_info_vec;
        vector<AttributeInfo> attr_info_vec;
        get_info(table_info_vec, attr_info_vec);


        // 创建 pg_class 与 pg_attribute 表
        for (TableInfo &tableInfo : table_info_vec) {
            vector<string> pg_class_data_vec = tableInfo.to_string_vec();
            pg_class.insert_data(pg_class_data_vec, vector<string>(pg_class_data_vec.size(), ""));
        }


        for (AttributeInfo &attributeInfo : attr_info_vec) {
            vector<string> pg_attr_data_vec = attributeInfo.to_string_vec();
            pg_attribute.insert_data(pg_attr_data_vec, vector<string>(pg_attr_data_vec.size(), ""));
        }

        // 创建学生表
        for (int i = 0; i < insert_times; i++) {
            StudentInfo studentInfo(i + 1, get_random_name(4), int(rand() % 10 + 18));
            vector<string> pg_stu_data_vec = studentInfo.to_string_vec();
            pg_student.insert_data(pg_stu_data_vec, vector<string>(pg_stu_data_vec.size(), ""));
        }
    }

    // 打印数据
    {
        if (run_mode & (SHOW_BLOCK_NUM)) {
            cout << "pg_class block num: " << pg_class.blocks.size() << endl;
            cout << "pg_attribute block num: " << pg_attribute.blocks.size() << endl;
            cout << "pg_student block num: " << pg_student.blocks.size() << endl;
        }
    }

    pg_class.save();
    pg_attribute.save();
    pg_student.save();
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



/*
void test() {
    TableManager tableManager1(1001);
    Block block;
    for (int i = 0; i < 300; i++) {
        tableManager1.insert_data({"hello", "cpp", "language"}, vector<string>(3, ""));
    }

    for (Block &block:tableManager1.blocks) {
        cout << block.tuple_datas.size() << endl;
    }
    TableManager tableManager2(1002);
    tableManager1.blocks.emplace_back(block);


    tableManager1.save();

    tableManager2.load(to_string(tableManager1.tableId));


    for (Block &block:tableManager2.blocks) {
        cout << block.tuple_datas.size() << endl;
    }
}
*/

/**
* quick sort test
*/
void quick_sort_test() {
    vector<TupleDataComparable> tuples;
    TupleDataComparable tupleDataComparable1(1);
    TupleDataComparable tupleDataComparable2(1);
    TupleDataComparable tupleDataComparable3(1);
    TupleDataComparable tupleDataComparable4(1);
    vector<string> d1{"Lucy", "85.3"};
    vector<string> d2{"Nano", "88.3"};
    vector<string> d3{"Nano", "78.3"};
    vector<string> d4{"Nano", "68.3"};
    tupleDataComparable1.insert(d1);
    tupleDataComparable2.insert(d2);
    tupleDataComparable3.insert(d3);
    tupleDataComparable4.insert(d4);
    tuples.emplace_back(tupleDataComparable1);
    tuples.emplace_back(tupleDataComparable2);
    tuples.emplace_back(tupleDataComparable3);
    tuples.emplace_back(tupleDataComparable4);
    Sorter sorter;
    sorter.quickSort(tuples, 0, tuples.size() - 1);
    for (auto x:tuples) {
        x.print();
    }
}