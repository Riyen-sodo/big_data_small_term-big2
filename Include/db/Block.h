//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_BLOCK_H
#define DEMO2_BLOCK_H

#include "PublicIncludes.h"
#include "Item.h"
#include "TupleData.h"
#include "PageHeaderData.h"
#include "Special.h"

/**
 * @brief 块
 *
 */
struct Block {
    PageHeaderData phd;
    vector<Item> items;
    vector<TupleData> tuple_datas;
    Special special;

    Block();

    Block(PageHeaderData phd);

    void set_table_id(int table_id);

    int get_size();

    void insert(const vector<string> &data_vec, const vector<string> &attr_desc);

    void print();

};

#endif //DEMO2_BLOCK_H
