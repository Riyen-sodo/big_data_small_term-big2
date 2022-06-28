#include "TableManager.h"
#include "Writer.h"
#include "errors.h"
#include <cmath>
#include "Reader.h"

// memcpy (to, from, size)


#define WRITE_EXTERN_BASIC_TYPE(TYPE, VAL, OFFSET) \
    memcpy(writer.buffer + OFFSET, &(VAL), sizeof(TYPE));\
    OFFSET += sizeof(TYPE)

#define READ_EXTERN_BASIC_TYPE(TYPE, VAL, OFFSET) \
    memcpy(&(VAL), reader.buffer + OFFSET, sizeof(TYPE));\
    offset += sizeof(TYPE)

#define WRITE_EXTERN_COMPLEX_STR(SIZE, STR, OFFSET) \
    memcpy(writer.buffer + OFFSET, STR.c_str(), SIZE);\
    OFFSET += SIZE

#define READ_EXTERN_COMPLEX_STR(SIZE, STR, OFFSET) \
    memcpy(STR, reader.buffer + OFFSET, SIZE);\
    OFFSET += SIZE

void TableManager::save() {
    // 写入
    for (int block_idx = 0; block_idx < this->blocks.size(); block_idx++) {
        Block block = this->blocks[block_idx];
        int offset = BLOCK_SIZE - Special::get_size();
        block.phd.table_id = this->tableId;

        // 每个 writer 对应一个块
        Writer writer;

        // 写入 PageHeaderData
        PageHeaderData phd = block.phd;
        writer.writeInt(phd.start_special);
        writer.writeInt(phd.free_space_start);
        writer.writeInt(phd.free_space_end);
        writer.writeInt(phd.table_id);

        // 写入 Items
        for (int i = 0; i < block.items.size(); i++) {
            Item item = block.items[i];
            item.tuple_data_size = block.tuple_datas[i].get_size();
            // BlockSpaceError
            writer.writeInt(item.tuple_data_start);
            writer.writeInt(item.tuple_data_size);
            offset -= item.tuple_data_size;
        }
        // 写入 TupleDatas
        // 反向迭代
        for (auto tupleData = block.tuple_datas.rbegin(); tupleData != block.tuple_datas.rend(); tupleData++) {
            // 先写入 n_attr  属性数量
            WRITE_EXTERN_BASIC_TYPE(int, tupleData->n_attr, offset);
            for (string &str : tupleData->datas) {
                // 写入大小
                int strSize = str.size() + 1;
                WRITE_EXTERN_BASIC_TYPE(int, strSize, offset);
                // 写入内容
                WRITE_EXTERN_COMPLEX_STR(strSize, str, offset);
            }
        }
        // 写入 Special
        int special_start = BLOCK_SIZE - Special::get_size();
        memcpy(writer.buffer + special_start, &block.special.reserved_bit, Special::get_size());
        // 写入 文件
        writer.save(to_string(tableId));
    }
}

void TableManager::load(string filePath) {
    filePath = "./db_folder/" + filePath;
    FILE *fp = fopen(filePath.c_str(), "rb");
    if (fp == nullptr) {
        throw FileOpenError(filePath);
    }
    if (fseek(fp, 0L, SEEK_END) != 0) {
        throw FilePtrMoveError(filePath);
    }
    int file_size = ftell(fp);
    rewind(fp);
    int n_block = ceil((1. * file_size) / (1. * BLOCK_SIZE));

    // 读取
    for (int i = 0; i < n_block; i++) {
        Block block;
        Reader reader;
        fread(reader.buffer, BLOCK_SIZE, 1, fp);
        int offset = BLOCK_SIZE - Special::get_size();

        // 读取 PageHeaderData
        reader.readInt(block.phd.start_special);
        reader.readInt(block.phd.free_space_start);
        reader.readInt(block.phd.free_space_end);
        reader.readInt(block.phd.table_id);

        // 读取 Items
        int n_item = (block.phd.free_space_start - PageHeaderData::get_size()) / (Item::get_size());
        for (int k = 0; k < n_item; k++) {
            Item item;
            reader.readInt(item.tuple_data_start);
            reader.readInt(item.tuple_data_size);

            offset -= item.tuple_data_size;

            block.items.emplace_back(item);
        }
        
        // 读取 TupleDatas
        // 正向迭代
        for (int k = 0; k < n_item; k++) {
            TupleData tupleData;
            // 先读取属性数量
            int n_attr = 0;
            // TODO 写入的offset相同，值不同
            READ_EXTERN_BASIC_TYPE(int, n_attr, offset);
            tupleData.n_attr = n_attr;

            // 读取TupleDta
            for (int j = 0; j < n_attr; j++) {
                // 读取大小
                int strSize = 0;
                READ_EXTERN_BASIC_TYPE(int, strSize, offset);

                // 读取内容
                char *str = (char *) malloc(strSize);
                READ_EXTERN_COMPLEX_STR(strSize, str, offset);

                tupleData.datas.emplace_back(string(str));
            }
            block.tuple_datas.emplace_back(tupleData);

        }
        if (block.tuple_datas.size() > 0) {
            this->blocks.emplace_back(block);
        }
    }

    if (fclose(fp) != 0) {
        throw FileCloseError(filePath);
    }
}

void TableManager::insert_data(const vector<string> &data_vec, const vector<string> &attr_desc) {
    int last_block_idx = this->blocks.size() - 1;
    try {
        blocks[last_block_idx].insert(data_vec, attr_desc);
    }
    catch (BlockSpaceError &bse) {
        cout << bse.what() << endl;
        this->extend_block(data_vec, attr_desc);
    }
}

TableManager::TableManager(int tableId) {
    this->tableId = tableId;
    if (this->blocks.empty()) {
        Block block;
        block.phd.table_id = tableId;
        this->blocks.emplace_back(block);
    }
}

void TableManager::extend_block(vector<string> data_vec, vector<string> attr_desc) {
    Block new_b;
    new_b.phd.table_id = this->tableId;
    new_b.insert(data_vec, attr_desc);
    this->blocks.emplace_back(new_b);
}



