#ifndef INFO_OBJECT
#define INFO_OBJECT

#include "PublicIncludes.h"

struct InfoObject {
    virtual vector<string> to_string_vec();

    void print();
};

#endif