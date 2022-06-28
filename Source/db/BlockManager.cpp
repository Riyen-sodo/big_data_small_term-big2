//
// Created by Flyoung on 2022/6/27.
//

#include "BlockManager.h"

#define DB_FOLADER "./db_folder/"

#include "errors.h"
#include "utils.h"
#include "cmath"

void BlockManager::insert(vector<string> data_vec, vector<string> attr_desc) {
    int last_block_idx = this->block_vec.size() - 1;
    try {
        block_vec[last_block_idx].insert(data_vec, attr_desc);
    }
    catch (BlockSpaceError &bse) {
        cout << bse.what() << endl;
        this->extend(data_vec, attr_desc);
    }
}

void BlockManager::extend(vector<string> data_vec, vector<string> attr_desc) {
    Block new_b;
    new_b.insert(data_vec, attr_desc);
    this->block_vec.push_back(new_b);
}

void BlockManager::set_table_id(int table_id) {
    for (Block &block : this->block_vec) {
        block.phd.table_id = table_id;
    }
}

int BlockManager::get_table_id() {
    if (this->block_vec.empty()) {

    } else {
        return this->block_vec[0].phd.table_id;
    }
    return -1;
}

int BlockManager::get_block_num() {
    return this->block_vec.size();
}

BlockManager::BlockManager(int table_id) {
    this->block_vec.emplace_back(
            Block(
                    PageHeaderData(
                            BLOCK_SIZE - Special::get_size() - 1,
                            PageHeaderData::get_size(),
                            BLOCK_SIZE - Special::get_size() - 1,
                            table_id
                    )
            )
    );
}

void BlockManager::print() {
    for (Block &block:this->block_vec) {
        block.print();
    }
}

void BlockManager::to_file(BlockManager &bm) {
    try {
        int table_id = bm.get_table_id();
        vector<string> folder_filenames = get_folder_filenames("./db_folder");
        string file_path = DB_FOLADER + to_string(table_id);
        for (const string &filenames : folder_filenames) {
            if (to_string(table_id) == filenames) {
                throw TableIdRepeatError(table_id);
            }
        }

        //
        FILE *fp = fopen(file_path.c_str(), "ab");

        // 序列化
        BlockManager::serialization(fp, bm, 0);

        fclose(fp);
    } catch (TableIdRepeatError &e) {
        cout << e.error() << endl;
    } catch (TableIdMissingError &e) {
        cout << e.what() << endl;
    } catch (BlockSpaceError &e) {
        cout << e.what() << endl;
        // 块空间不足，扩展
        int table_id = bm.get_table_id();
        string file_path = DB_FOLADER + to_string(table_id);
        int block_num = BlockManager::get_block_num(file_path);
        FILE *fp = fopen(file_path.c_str(), "ar");
        BlockManager::serialization(fp, bm, block_num * BLOCK_SIZE);
    }
}

void BlockManager::from_file(BlockManager &bm, int table_id) {
    try {
        string file_path = DB_FOLADER + to_string(table_id);
        int n_block = BlockManager::get_block_num(file_path);
        FILE *fp = fopen(file_path.c_str(), "rb");

        if (fp == nullptr || feof(fp)) {
            cout << "file is null" << endl;
            return;
        }

        for (int i = 0; i < n_block; i++) {
            fseek(fp, BLOCK_SIZE * i, 0);
            Block block;
            int offset = 0;
            // 反序列化 phd
            block.phd.deserialize(fp, offset);

            // 反序列化 ItemVec
            int n_item = (block.phd.free_space_start - offset) / Item::get_size();
            block.items.resize(n_item);
            for (Item &item : block.items) {
                item.deserialize(fp, offset);
            }

            // 反序列化 TupleData
            block.tuple_datas.resize(n_item);
            // 将 fp 移动到数据项开始位置
            // 最后一个 Item 指向的就是最后的 TupleVec
            fseek(fp, block.items[block.items.size() - 1].tuple_data_start, 0);
            for (TupleData &td:block.tuple_datas) {
                td.deserialize(fp, offset);
            }



            // 反序列化 Special
            fseek(fp, block.phd.start_special, 0);
            block.special.deserialize(fp, offset);


            bm.block_vec.emplace_back(block);
        }
        fclose(fp);
    } catch (TableIdMissingError &e) {
        cout << e.what() << endl;
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}


void BlockManager::serialization(FILE *fp, BlockManager &bm, int origin) {
    // 序列化
    for (Block &block : bm.block_vec) {
        int offset = 0;

        // 序列化 phd
        block.phd.serialization(fp, offset);


        // 序列化 ItemVec
        for (Item &item : block.items) {
            item.serialization(fp, offset);
        }

        // 序列化 TupleData
        // 将 fp 移动到数据项开始位置
        fseek(fp, block.items[block.items.size() - 1].tuple_data_start, 0);
        for (TupleData &td:block.tuple_datas) {
            td.serialization(fp, offset);
        }

        // 序列化 Special
        // 将 fp 移动到 Special 开始位置
        if (fseek(fp, block.phd.start_special, origin) != 0) {
            throw std::logic_error("f");
        }
        block.special.serialization(fp, offset);
    }
}

int BlockManager::get_block_num(string file_path) {
    FILE *fp = fopen(file_path.c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    int file_size = ftell(fp);
    rewind(fp);
    int n_block = ceil((1. * file_size) / (1. * BLOCK_SIZE));
    fclose(fp);
    return n_block;
}
