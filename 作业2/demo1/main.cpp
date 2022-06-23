#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cstring>
#include <filesystem>
#include <typeinfo>
#include <random>

#define BLOCK_SIZE 8192
using std::filesystem::directory_iterator;
using namespace std;


vector <string> get_folder_filenames(string folder) {
    vector <string> filenames;
    for (const auto &file : directory_iterator(folder)) {
        string s = file.path().string();
        for (int i = s.size() - 1; i >= 0; i--) {
            if (s[i] == '\\') {
                s = s.substr(i + 1, s.size() - 1);
                break;
            }
        }
        filenames.push_back(s);
    }
    return filenames;
}

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

/**
 * @brief 块空间异常
 * 如果【要插入的数据 + 块内数据 > 块大小】，则抛出此异常
 * 
 */
struct BlockSpaceError : public exception {
    const char *what() const throw() { return "Not enough block space"; }
};

/**
 * @brief 表Id缺失异常
 * 如果将表的内容存放到文件中之前，没用设置表的Id，则抛出此异常
 */
struct TableIdMissingError : public exception {
    const char *what() const throw() { return "Table id has not been set"; }
};

/**
 * @brief 表Id重复异常
 * 如果将表保存到文件时，该表的 Id 已经存在，则抛出此异常
 */
struct TableIdRepeatError : public exception {
    int repeat_table_id;

    TableIdRepeatError(int repeat_table_id) : repeat_table_id(repeat_table_id) {}

    const char *what() const throw() {
        return ("Table id " + to_string(this->repeat_table_id) + " already exists").c_str();
    }

    string error() {
        return ("Table id " + to_string(this->repeat_table_id) + " already exists").c_str();
    }
};

/**
 * @brief 块内保留空间，暂时没有作用
 * 
 */
struct Special {
    int get_size() { return 0; }

    void print() { cout << "Special" << endl; }
};

/**
 * @brief 块头，记录块的部分信息
 * 
 */
struct PageHeaderData {
    int start_special;
    int free_space_start;
    int free_space_end;
    int table_id;

    PageHeaderData() {
        start_special = BLOCK_SIZE;
        free_space_start = get_size();
        free_space_end = 0;
        table_id = 0;
    }

    int get_size() { return sizeof(PageHeaderData); }

    void print() {
        printf(
                "start special : %d\nfree space start : %d\nfree space end : "
                "%d\ntable id : %d\n",
                start_special, free_space_start, free_space_end, table_id);
    }
};

/**
 * @brief 块内数据项
 * 
 */
struct TupleData {
    int n_attr;
    vector <string> data_vec;

    void insert(vector <string> data_vec, vector <string> attr_desc) {
        this->data_vec = data_vec;
        this->n_attr = attr_desc.size();
    }

    void print() {
        for (string s : this->data_vec) {
            cout << s << "\t";
        }
        cout << endl;
    }

    int get_size() {
        int size = sizeof(TupleData);
        for (string s : this->data_vec) {
            size += s.size();
        }
        return size;
    }
};

/**
 * @brief 块内数据项头部
 * 
 */
struct Item {
    int tuple_data_start;
    int len_tuple_data;

    int get_size() { return sizeof(Item); }

    void insert(int tuple_data_start, int len_tuple_data) {
        this->tuple_data_start = tuple_data_start;
        this->len_tuple_data = len_tuple_data;
    }

    void print() {
        cout << "tuple start : " << tuple_data_start
             << "\ttuple len : " << len_tuple_data << endl;
    }
};

/**
 * @brief 计算空间大小
 * 
 */
struct SizeCalculator {
    static int get_string_vec_size(vector <string> string_vec) {
        int size = sizeof(string_vec);
        for (auto x : string_vec) {
            size += x.size();
        }
        return size;
    }

    static int get_item_vec_size(vector <Item> item_vec) {
        int size = sizeof(item_vec);
        for (auto x : item_vec) {
            size += x.get_size();
        }
        return size;
    }

    static int get_tuple_vec_size(vector <TupleData> tuple_vec) {
        int size = sizeof(tuple_vec);
        for (auto x : tuple_vec) {
            size += x.get_size();
        }
        return size;
    }
};

/**
 * @brief 块
 * 
 */
struct Block {
    PageHeaderData phd;
    vector <Item> item_vec;
    vector <TupleData> tuple_data_vec;
    Special special;

    void set_table_id(int table_id) { this->phd.table_id = table_id; }

    int get_size() {
        int size = this->phd.get_size() + this->special.get_size();
        for (int i = 0; i < this->tuple_data_vec.size(); i++) {
            size += this->item_vec[i].get_size();
            size += this->tuple_data_vec[i].get_size();
        }
        return size;
    }

    void insert(vector <string> data_vec, vector <string> attr_desc) {
        int data_size = SizeCalculator::get_string_vec_size(data_vec);
        if (this->get_size() + data_size > BLOCK_SIZE) {
            throw BlockSpaceError();
        }

        TupleData td;
        td.insert(data_vec, attr_desc);
        this->tuple_data_vec.push_back(td);
        int tuple_size =
                SizeCalculator::get_tuple_vec_size(this->tuple_data_vec);

        Item it;
        it.insert(BLOCK_SIZE - tuple_size, data_size);
        this->item_vec.push_back(it);

        int item_size = SizeCalculator::get_item_vec_size(this->item_vec);
        this->phd.free_space_end = BLOCK_SIZE - special.get_size() - data_size;
        this->phd.free_space_start = this->phd.get_size() + item_size;
    }

    void print(int block_id) {
        cout << "========================  Start Block (" << block_id
             << ")  ========================\n"
             << endl;
        printf(" Block Size[ %d ] \n\n", this->get_size());
        cout << "===========  PageHeaderData  ===========" << endl;
        this->phd.print();
        cout << "\n\n===========  Item  ===========" << endl;
        for (Item it : this->item_vec) {
            it.print();
        }
        cout << "\n\n===========  TupleData  ===========" << endl;
        for (TupleData td : this->tuple_data_vec) {
            td.print();
        }
        cout << "\n\n===========  Special  ===========" << endl;
        this->special.print();
        cout << "\n\n========================  End Block (" << block_id
             << ") ========================\n\n"
             << endl;
    }
};

/**
 * 获取一个长度为 n 的随机名字
 * @param len
 * @return 随机名字
 */
string get_random_name(int len) {
    string name;
    char first_name = char(int(65 + rand() % 26));  // A~Z  65~90
    name.push_back(first_name);
    for (int i = 0; i < len; i++) {
        // a~z  97~122
        name.push_back(int(97 + rand() % 26));
    }
    return name;
}

/**
 * 获取 n 个长度为 m 的随机字符串
 * @param m
 * @param n
 * @return 随机字符串
 */
vector <string> get_random_string(int m, int n) {
    vector <string> ret;
    for (int i = 0; i < n; i++) {
        ret.push_back(get_random_name(m));
    }
    return ret;
}

/**
 * 块管理器
 * 每个块管理器对应一张表
 */
struct BlockManager {
    vector <Block> block_vec;

    BlockManager() {}

    BlockManager(int table_id) {
        if (this->block_vec.empty()) {
            this->block_vec.push_back(Block());
        }
        this->set_table_id(table_id);
    }

    /**
     * 获取块管理器中的块数量
     * @return
     */
    int get_block_num() { return this->block_vec.size(); }

    /**
     * 获取表的 Id
     * @return
     */
    int get_table_id() {
        if (this->block_vec.empty()) {

        } else {
            return this->block_vec[0].phd.table_id;
        }
        return -1;
    }

    /**
     * 设置表的 Id
     * @param table_id
     */
    void set_table_id(int table_id) {
        for (Block &block : this->block_vec) {
            block.phd.table_id = table_id;
        }
    }

    /**
     * 插入数据到表中
     * @param data_vec
     * @param attr_desc
     */
    void insert(vector <string> data_vec, vector <string> attr_desc) {
        if (this->block_vec.empty()) {
            this->block_vec.push_back(Block());
        }
        int last_block_id = block_vec.size() - 1;
        try {
            block_vec[last_block_id].insert(data_vec, attr_desc);
        }
        catch (BlockSpaceError &bse) {
            cout << bse.what() << endl;
            this->extend(data_vec, attr_desc);
        }
    }

    /**
     * @brief 当一个块的空间不足时，需要扩展
     * 
     * @param data_vec 数据元组
     * @param attr_desc 数据元组的属性描述
     */
    void extend(vector <string> data_vec, vector <string> attr_desc) {
        Block new_b;
        new_b.insert(data_vec, attr_desc);
        this->block_vec.push_back(new_b);
    }

    /**
     * 打印表的内容
     */
    void print() {
        for (int i = 0; i < this->block_vec.size(); i++) {
            block_vec[i].print(i);
        }
    }

    /**
     * @brief 将结构体信息写入文件
     * 
     */
    void to_file() {
        try {
            vector <string> folder_filenames = get_folder_filenames("./db_folder");
            int table_id = this->get_table_id();
            string file_path = "./db_folder/" + to_string(table_id);
            for (string filenames : folder_filenames) {
                if (to_string(table_id) == filenames) {
                    throw TableIdRepeatError(this->get_table_id());
                }
            }
            FILE *fp = fopen(file_path.c_str(), "wb");
            for (int i = 0; i < this->block_vec.size(); i++) {
                Block *block = (Block *) calloc(1, BLOCK_SIZE);  // malloc 报错
                if (block == nullptr) {
                    cout << "申请空间失败" << endl;
                    return;
                }
                /*Block cur = this->block_vec[i];
                block->phd = cur.phd;
                block->item_vec = cur.item_vec;
                block->tuple_data_vec = cur.tuple_data_vec;
                block->special = cur.special;*/
                *block = this->block_vec[i];
                fwrite(block, BLOCK_SIZE, 1, fp);
            }
            fclose(fp);
        } catch (TableIdRepeatError &e) {
            cout << e.error() << endl;
        } catch (TableIdMissingError &e) {
            cout << e.what() << endl;
        } catch (exception &e) {
            cout << e.what() << endl;
        }

    }

    /**
     * @brief 从文件中读取信息到结构体
     * 
     * @param bm 被写入的结构体
     */
    void from_file(BlockManager &bm) {
        try {
            string file_path = "./db_folder/" + to_string(this->get_table_id());
            FILE *fp = fopen(file_path.c_str(), "rb");
            if (fp == nullptr || feof(fp)) {
                cout << "file is null" << endl;
                return;
            }
            Block *block = (Block *) calloc(1, BLOCK_SIZE);

            while (!feof(fp) && fread(block, BLOCK_SIZE, 1, fp) == 1) {
                Block b(*block);
                bm.block_vec.push_back(b);
                memset(block, 0, BLOCK_SIZE);
            }
            fclose(fp);
        } catch (TableIdMissingError &e) {
            cout << e.what() << endl;
        } catch (exception &e) {
            cout << e.what() << endl;
        }
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