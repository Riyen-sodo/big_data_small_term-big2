#include "heads.h"
#include "utils.h"

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