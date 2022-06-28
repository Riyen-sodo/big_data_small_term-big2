//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_BLOCKMANAGER_H
#define DEMO2_BLOCKMANAGER_H

#include "PublicIncludes.h"
#include "SizeCalculator.h"
#include "Block.h"

/**
 * 块管理器
 * 每个块管理器对应一张表
 */
struct BlockManager {
    vector<Block> block_vec;


    BlockManager(int table_id);

    /**
     * 获取块管理器中的块数量
     * @return
     */
    int get_block_num();

    /**
     * 获取表的 Id
     * @return
     */
    int get_table_id();

    /**
     * 设置表的 Id
     * @param table_id
     */
    void set_table_id(int table_id);

    /**
     * 插入数据到表中
     * @param data_vec
     * @param attr_desc
     */
    void insert(vector<string> data_vec, vector<string> attr_desc);

    /**
     * @brief 当一个块的空间不足时，需要扩展
     *
     * @param data_vec 数据元组
     * @param attr_desc 数据元组的属性描述
     */
    void extend(vector<string> data_vec, vector<string> attr_desc);

    /**
     * 打印表的内容
     */
    void print();

    /**
     * @brief 将结构体信息写入文件
     *
     */
    static void to_file(BlockManager &bm);

    /**
     * @brief 从文件中读取信息到结构体
     *
     * @param bm 被写入的结构体
     */
    static void from_file(BlockManager &bm, int table_id);


    static void serialization(FILE* fp, BlockManager& bm, int origin);

    static int get_block_num(string file_path);
};

#endif //DEMO2_BLOCKMANAGER_H
