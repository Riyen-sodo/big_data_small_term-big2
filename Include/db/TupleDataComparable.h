//
// Created by Flyoung on 2022/6/29.
//

#ifndef INC_3_TUPLEDATACOMPLEX_H
#define INC_3_TUPLEDATACOMPLEX_H

#include "TupleData.h"
#include "PublicIncludes.h"

struct TupleDataComparable : public TupleData {
public:
    int columnIndexToSort;

    TupleDataComparable(int columnIndexToSort);

    bool operator==(const TupleDataComparable &tupleDataComparable);

    bool operator>(const TupleDataComparable &tupleDataComparable);

    bool operator<(const TupleDataComparable &tupleDataComparable);

    bool operator<=(const TupleDataComparable &tupleDataComparable);

    bool operator>=(const TupleDataComparable &tupleDataComparable);

    void insert(vector<string> data);

    void insert(TupleData tupleData);
};

#endif //INC_3_TUPLEDATACOMPLEX_H
