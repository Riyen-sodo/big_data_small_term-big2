#ifndef SPECIAL
#define SPECIAL

#include "PublicIncludes.h"

struct Special {
    char reserved_bit;

    Special();

    static int get_size();

    void print();

    void serialization(FILE *fp, int &offset);

    void deserialize(FILE *fp, int &offset);
};

#endif