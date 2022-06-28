//
// Created by Flyoung on 2022/6/28.
//

#ifndef INC_3_TABLEMANAGER_H
#define INC_3_TABLEMANAGER_H

#include "PublicIncludes.h"
#include "Block.h"

class TableManager {
public:
    vector<Block> blocks;
    int tableId = tableId;

    void save();

    void load(string filePath);
};

#endif //INC_3_TABLEMANAGER_H
