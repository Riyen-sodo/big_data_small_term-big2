//
// Created by Flyoung on 2022/6/29.
//

#include <TupleDataComparable.h>

using std::stof;

#define CONVERT_TUPLE_TO_VAL(TUPLE) \
    float value1 = stof(this->datas[columnIndexToSort]); \
    float value2 = stof(TUPLE.datas[columnIndexToSort])

TupleDataComparable::TupleDataComparable(int columnIndexToSort) {
    this->columnIndexToSort = columnIndexToSort;
}

bool TupleDataComparable::operator==(const TupleDataComparable &tupleDataComparable) {
    CONVERT_TUPLE_TO_VAL(tupleDataComparable);
    return value1 == value2;
}


bool TupleDataComparable::operator>(const TupleDataComparable &tupleDataComparable) {
    CONVERT_TUPLE_TO_VAL(tupleDataComparable);
    return value1 > value2;
}

bool TupleDataComparable::operator<(const TupleDataComparable &tupleDataComparable) {
    CONVERT_TUPLE_TO_VAL(tupleDataComparable);
    return value1 < value2;
}

bool TupleDataComparable::operator<=(const TupleDataComparable &tupleDataComparable) {
    CONVERT_TUPLE_TO_VAL(tupleDataComparable);
    return value1 <= value2;
}

bool TupleDataComparable::operator>=(const TupleDataComparable &tupleDataComparable) {
    CONVERT_TUPLE_TO_VAL(tupleDataComparable);
    return value1 >= value2;
}

void TupleDataComparable::insert(vector<string> data) {
    this->datas = data;
}

void TupleDataComparable::insert(TupleData tupleData) {
    this->datas = tupleData.datas;
    this->n_attr = tupleData.n_attr;
}

