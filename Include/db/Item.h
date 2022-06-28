//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_ITEM_H
#define DEMO2_ITEM_H

#include "PublicIncludes.h"
#include "Special.h"
/**
 * @brief 块内数据项头部
 *
 */
struct Item {
    int tuple_data_start;
    int tuple_data_size;

    Item();

    static int get_size();

    void insert(int tuple_data_start, int len_tuple_data);

    void print();

    void serialization(FILE *fp, int &offset);

    void deserialize(FILE *fp, int &offset);
};

#endif //DEMO2_ITEM_H
