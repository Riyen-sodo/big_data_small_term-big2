//
// Created by Flyoung on 2022/6/27.
//
#include "SizeCalculator.h"

int SizeCalculator::get_string_vec_size(const vector <string>& string_vec) {
    int size = 0;
    for (const auto& x : string_vec) {
        size += x.size();
    }
    return size;
}

int SizeCalculator::get_item_vec_size(const vector <Item>& item_vec) {
    int size = 0;
    for (auto x : item_vec) {
        size += x.get_size();
    }
    return size;
}

int SizeCalculator::get_tuple_vec_size(const vector <TupleData>& tuple_vec) {
    int size = 0;
    for (auto x : tuple_vec) {
        size += x.get_size();
    }
    return size;
}