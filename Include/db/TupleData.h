#ifndef DEMO2_TUPLEDATA_H
#define DEMO2_TUPLEDATA_H

/**
 * @brief 块内数据项
 *
 */
#include "PublicIncludes.h"
#include <iomanip>

using std::setw;

struct TupleData {
    int n_attr;
    vector<string> datas;

    void insert(vector<string> data_vec, const vector<string> &attr_desc);

    void print();

    int get_size();

    void serialization(FILE *fp, int &offset);

    void deserialize(FILE *fp, int &offset);

};

#endif //DEMO2_TUPLEDATA_H
