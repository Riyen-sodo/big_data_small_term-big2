#include "TableManager.h"
#include "Writer.h"
#include "errors.h"
#include <cmath>
#include "Reader.h"

void TableManager::save() {
    for (Block &block:blocks) {
        int offset = BLOCK_SIZE - Special::get_size();
        // 写入 PageHeaderData
        PageHeaderData phd = block.phd;
        Writer writer;
        writer.writeInt(phd.start_special);
        writer.writeInt(phd.free_space_start);
        writer.writeInt(phd.free_space_end);
        writer.writeInt(phd.table_id);

        // 写入 Items
        for (int i = 0; i < block.items.size(); i++) {
            Item item = block.items[i];
            writer.writeInt(item.tuple_data_start);
            writer.writeInt(item.tuple_data_size);

            offset -= block.tuple_datas[i].get_size();
        }

        {
            for (Item item:block.items) {
                item.print();
            }
        }

        // 写入 TupleDatas
        // 反向迭代
        for (auto tupleData = block.tuple_datas.rbegin(); tupleData != block.tuple_datas.rend(); tupleData++) {
            // 先写入 n_attr  属性数量
            memcpy(writer.buffer + offset, &(tupleData->n_attr), sizeof(int));
            for (string &str : tupleData->datas) {
                // 写入大小
                int strSize = str.size() + 1;
                memcpy(writer.buffer + offset, &strSize, sizeof(int));
                offset += sizeof(int);

                // 写入内容
                memcpy(writer.buffer + offset, str.c_str(), strSize);
                offset += strSize;
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
    fseek(fp, 0L, SEEK_END);
    int file_size = ftell(fp);
    rewind(fp);
    int n_block = ceil((1. * file_size) / (1. * BLOCK_SIZE));
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

        {
            cout << "\n\n==================" << endl;
            for (Item item:block.items) {
                item.print();
            }
        }


        // 读取 TupleDatas
        // 正向迭代
        for (int k = 0; k < n_item; k++) {
            TupleData tupleData;
            // 先读取属性数量
            int n_attr = 0;
            memcpy(&n_attr, reader.buffer, sizeof(int));
            tupleData.n_attr = n_attr;

            // 读取TupleDta
            for (int j = 0; j < n_attr; j++) {
                // 读取大小
                int strSize = 0;
                memcpy(&strSize, reader.buffer, sizeof(int));

                // 读取内容
                char *str = (char *) malloc(strSize);
                memcpy(str, reader.buffer, strSize);

                tupleData.datas.emplace_back(string(str));
            }
            block.tuple_datas.emplace_back(tupleData);
        }
    }

    if (fclose(fp) != 0) {
        throw FileCloseError(filePath);
    }
}


