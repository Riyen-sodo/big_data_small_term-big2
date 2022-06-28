#ifndef PAGE_HEADER_DATA
#define PAGE_HEADER_DATA

/**
 * @brief 块头，记录块的部分信息
 *
 */
#include "PublicIncludes.h"

struct PageHeaderData {
    int start_special;
    int free_space_start;
    int free_space_end;
    int table_id;

    PageHeaderData() {}

    PageHeaderData(int startSpecial, int freeSpaceStart, int freeSpaceEnd, int tableId);

    static int get_size();

    void print();

    void serialization(FILE *fp, int &offset);

    void deserialize(FILE *fp, int &offset);

    void write_to_IOBuffer(char *buffer, int &offset);

    void read_from_IOBuffer(char *buffer, int &offset);
};

#endif