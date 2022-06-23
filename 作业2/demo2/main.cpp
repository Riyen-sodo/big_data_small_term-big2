#include "database.cpp"
#include "tables.cpp"


int main() {
	system("chcp 65001");
    int insert_times = 200;


    int pg_class_id = 1001;
    int pg_attribute_id = 1002;
    int pg_student_id = 1003;
    BlockManager pg_class(pg_class_id);
    BlockManager pg_attribute(pg_attribute_id);
    BlockManager pg_student(pg_student_id);


    vector <TableInfo> table_info_vec;
    vector <AttributeInfo> attr_info_vec;
    get_info(table_info_vec, attr_info_vec);


    // 创建 pg_class 与 pg_attribute 表
    for (TableInfo &tableInfo : table_info_vec) {
        vector <string> pg_class_data_vec = tableInfo.to_string_vec();
        pg_class.insert(pg_class_data_vec, vector<string>(pg_class_data_vec.size(), ""));
    }
    for (AttributeInfo &attributeInfo : attr_info_vec) {
        vector <string> pg_attr_data_vec = attributeInfo.to_string_vec();
        pg_attribute.insert(pg_attr_data_vec, vector<string>(pg_attr_data_vec.size(), ""));
    }

    // 创建学生表
    for (int i = 0; i < insert_times; i++) {
        StudentInfo studentInfo(i + 1, get_random_name(4), int(rand() % 10 + 18));
        vector <string> pg_stu_data_vec = studentInfo.to_string_vec();
        pg_student.insert(pg_stu_data_vec, vector<string>(pg_stu_data_vec.size(), ""));
    }

    // 将表保存到文件中
    {
        pg_class.to_file();
        pg_attribute.to_file();
        pg_student.to_file();
    }

    // 从文件中读取表，并打印
    {
        BlockManager pg_class_buffer;
        pg_class.from_file(pg_class_buffer);
        pg_class_buffer.print();

        BlockManager pg_attribute_buffer;
        pg_attribute.from_file(pg_attribute_buffer);
        pg_attribute_buffer.print();

        BlockManager pg_student_buffer;
        pg_student.from_file(pg_student_buffer);
        pg_student_buffer.print();
    }
    return 0;
}