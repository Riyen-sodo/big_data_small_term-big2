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

    void save_tuple_datas(string filePath);

    /**
     * 序列化
     * @param block_offset block的索引偏移量
     */
    void save();

    void save(string filePath);

    /**
     * 反序列化
     * @param filePath
     * @param readAll 是否读取所有的块
     * @param blockNum 文件中块的数量
     * @param blockOffset 从第几个块开始读(从 0 开始)
     */
    void load(string filePath, bool readAll, int &blockNum, int blockOffset);

    void insert_data(const vector<string> &data_vec, const vector<string> &attr_desc);

    void extend_block(vector<string> data_vec, vector<string> attr_desc);

    bool empty();

};

#endif //INC_3_TABLEMANAGER_H
