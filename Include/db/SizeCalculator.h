//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_SIZECALCULATOR_H
#define DEMO2_SIZECALCULATOR_H

#include "PublicIncludes.h"
#include "Item.h"
#include "TupleData.h"
/**
 * @brief 计算空间大小
 *
 */
struct SizeCalculator {
    static int get_string_vec_size(const vector <string>& string_vec);

    static int get_item_vec_size(const vector <Item>& item_vec);

    static int get_tuple_vec_size(const vector <TupleData>& tuple_vec);
};

#endif //DEMO2_SIZECALCULATOR_H
