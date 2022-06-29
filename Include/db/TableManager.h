//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_TABLEMANAGER_H
#define INC_3_TABLEMANAGER_H

#include "PublicIncludes.h"
#include "Block.h"
#include "Checker.h"

class TableManager {
public:
    vector<Block> blocks;
    int tableId;

    TableManager(int tableId);
    /**
     * 序列化
     * @param block_offset block的索引偏移量
     */
    void save();

    void load(string filePath);

    void insert_data(const vector<string> &data_vec, const vector<string> &attr_desc);

    void extend_block(vector<string> data_vec, vector<string> attr_desc);

    bool empty();
};

#endif //INC_3_TABLEMANAGER_H
