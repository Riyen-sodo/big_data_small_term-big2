//
// Created by Flyoung on 2022/6/27.
//

#include "Item.h"

int Item::get_size() {
    return sizeof(Item);
}

void Item::insert(int tuple_data_start, int len_tuple_data) {
    this->tuple_data_start = tuple_data_start;
    this->tuple_data_size = len_tuple_data;
}

void Item::print() {
    cout << "tuple start : " << tuple_data_start
         << "\ttuple len : " << tuple_data_size << endl;
}

void Item::serialization(FILE *fp, int &offset) {
    fwrite(&tuple_data_start, sizeof(tuple_data_start), 1, fp);
    offset += sizeof(tuple_data_start);

    fwrite(&tuple_data_size, sizeof(tuple_data_size), 1, fp);
    offset += sizeof(tuple_data_size);
}

void Item::deserialize(FILE *fp, int & offset) {
    fread(&tuple_data_start, sizeof(tuple_data_start), 1, fp);
    offset += sizeof(tuple_data_start);

    fread(&tuple_data_size, sizeof(tuple_data_size), 1, fp);
    offset += sizeof(tuple_data_size);
}

Item::Item() {
    tuple_data_start = BLOCK_SIZE - Special::get_size();
    tuple_data_size = 0;
}
