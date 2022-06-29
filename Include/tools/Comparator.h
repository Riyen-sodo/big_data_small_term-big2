//
// Created by Flyoung on 2022/6/29.
//

#ifndef INC_3_COMPARATOR_H
#define INC_3_COMPARATOR_H

#include "PublicIncludes.h"

#define IS_STRING_TYPE(VAL) \
    typeid(VAL) == typeid(string) ? true : false


#define CONVERT(A, B) \
        try { \
    convert(A, val);\
    convert(B, filterVal);\
    } catch (exception &e) {\
    throw TypeConvertError();\
    }

using std::stof;

class Comparator {
public:
    float val;
    float filterVal;

    template<typename T>
    bool equal(T a, T b);


    template<typename T>
    bool smaller(T a, T b);

    template<typename T>
    bool greater(T a, T b);
};


template<typename T>
bool Comparator::equal(T a, T b) {
    CONVERT(a, b)
    return val == filterVal;
}


template<typename T>
bool Comparator::smaller(T a, T b) {
    CONVERT(a, b)
    return val < filterVal;
}

template<typename T>
bool Comparator::greater(T a, T b) {
    CONVERT(a, b)
    return val > filterVal;
}

#endif //INC_3_COMPARATOR_H
