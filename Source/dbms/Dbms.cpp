#include "Dbms.h"

#define INTERNAL_SORT 1<<1
#define MERGE_BLOCK 1 << 2

#define READ_EXTERN_BASIC_TYPE(READER, TYPE, VAL, OFFSET) \
    memcpy(&(VAL), READER.buffer + OFFSET, sizeof(TYPE));\
    OFFSET += sizeof(TYPE)

#define READ_PAGE_HEADER_DATA(BLOCK_READER, PHD)\
    BLOCK_READER.readInt(PHD.start_special);\
    BLOCK_READER.readInt(PHD.free_space_start);\
    BLOCK_READER.readInt(PHD.free_space_end);\
    BLOCK_READER.readInt(PHD.table_id)

#define CALC_ITEM_COUNT(PHD) \
    (PHD.free_space_start - PageHeaderData::get_size()) / (Item::get_size())


#define MOVE_OFFSET_TO_TUPLE_DATA(N_ITEM, BLOCK_READER, OFFSET)\
    for (int i = 0; i < (N_ITEM); i++) {\
        Item item;\
        (BLOCK_READER).readInt(item.tuple_data_start);\
        (BLOCK_READER).readInt(item.tuple_data_size);\
        (OFFSET) -= item.tuple_data_size; \
    }

#define READ_EXTERN_COMPLEX_STR(READER, SIZE, STR, OFFSET) \
    memcpy(STR, READER.buffer + OFFSET, SIZE);\
    OFFSET += SIZE

const int SORT_MODE = ((1 << 0) | MERGE_BLOCK);

void DatabaseManagerSystem::select(
        const string &tableName,
        const vector<string> &columnNames,
        const FilterExpression &filterExpression,
        TableManager &pg_class,
        TableManager &pg_attribute
) {
    {
        if (pg_class.empty()) {
            cout << "pg_class is empty," <<
                 " can't select from empty table" << endl;
            return;
        }
        if (pg_attribute.empty()) {
            cout << "pg_attribute is empty," <<
                 " can't select from empty table" << endl;
            return;
        }
    }
    // TODO 根据表名获取表的 Id
    int n_attr = pg_class.blocks[0].tuple_datas[0].n_attr;
    int table_id = find_table_id_by_name(pg_class, tableName);
    if (table_id == -1) {
        printf("can't find table [ %s ], may be table name error?", tableName.c_str());
        return;
    }

    // TODO 获取列名数组在表中的索引
    vector<int> columnIndexes = get_column_indexes(pg_attribute, columnNames);
    {
//        for (int i = 0; i < columnIndexes.size(); i++) {
//            cout << "column : " << columnNames[i] << "   , idx : " << columnIndexes[i] << endl;
//        }
    }
    if (columnIndexes.size() == 0) {
        cout << "The specified column was not found" << endl;
        return;
    }

    // TODO 获取到具体的表
    TableManager target(-1);
    for (const TableManager &tm:this->tables) {
        if (tm.tableId == table_id) {
            target = TableManager(tm);
        }
    }
    if (target.tableId == -1) {
        cout << "Table exists but is not mounted on the database management system" << endl;
        return;
    }
    // TODO 获取 columnNames 中的列
    vector<vector<string>> resultSet;
    try {
        resultSet = get_target_columns(columnIndexes, target);
    } catch (VectorIndexError &vectorIndexError) {
        cout << vectorIndexError.what() << endl;
        return;
    }
    if (resultSet.size() == 0) {
        cout << "There are specified columns, but no records" << endl;
        return;
    }
    // TODO 过滤
    resultSet = filtter_result_set(resultSet, filterExpression, columnNames);

    // TODO 打印信息
    if (RUN_MODE & (SHOW_SELECT)) {
        for (const string &columnName:columnNames) {
            cout << setw(20) << columnName;
        }
        cout << endl;
        for (const vector<string> &strVec:resultSet) {
            for (const string &val:strVec) {
                cout << setw(20) << val;
            }
            cout << endl;
        }
        cout << endl;
    }

}

int DatabaseManagerSystem::find_table_id_by_name(TableManager pg_class, string tableName) {
    int table_id = -1;
    for (Block &block:pg_class.blocks) {
        for (int i = 0; i < block.tuple_datas.size(); i++) {
            TupleData tupleData = block.tuple_datas[i];
            // TODO 硬编码
            if (tupleData.datas[0] == tableName) {
                table_id = std::stoi(tupleData.datas[1]);
                break;
            }
        }
    }
    return table_id;
}

void DatabaseManagerSystem::mount(TableManager &tableManager) {
    this->tables.emplace_back(tableManager);
}

void DatabaseManagerSystem::mount(vector<TableManager> tables) {
    if (tables.size() == 0) {
        cout << "No tables are mounted on the DBMS this time" << endl;
        return;;
    }
    for (TableManager table:tables) {
        this->tables.emplace_back(table);
    }
}

vector<int> DatabaseManagerSystem::get_column_indexes(TableManager &tableManager, vector<string> columnNames) {
    if (RUN_MODE & (DEBUG)) {
        bool isRepeat = is_string_vector_repeat(columnNames);
        if (isRepeat) {
            cout << "column name repeat" << endl;
            throw StringVectorRepeatError();
        }
    } else {
        columnNames = string_vector_deduplication(columnNames);
    }
    vector<int> columnIndexes;

    // pg_attribute
    for (string columnName:columnNames) {
        for (Block &block:tableManager.blocks) {
            for (int i = 0; i < block.tuple_datas.size(); i++) {
                vector<string> row = block.tuple_datas[i].datas;
                // TODO 硬编码
                // pg_attribute[2] = 字段名
                if (row[2] == columnName) {
                    // TODO 硬编码
                    // pg_attribute[5] = 字段所在位置
                    columnIndexes.emplace_back(std::stoi(row[5]));
                }
            }
        }
    }
    return columnIndexes;
}


vector<vector<string>> DatabaseManagerSystem::get_target_columns(vector<int> columnIndexes, TableManager &target) {
    vector<vector<string>> resultSet;
    for (Block &block : target.blocks) {
        for (TupleData tupleData:block.tuple_datas) {
            vector<string> columnValues = tupleData.datas;
            vector<string> targetColumns;
            for (int columnIndex:columnIndexes) {
                try {
                    // TODO 文件被修改，硬编码未修改
                    // 列索引异常
                    targetColumns.emplace_back(columnValues[columnIndex - 1]);
                } catch (exception &e) {
                    throw VectorIndexError();
                }
            }
            resultSet.emplace_back(targetColumns);
        }
    }
    return resultSet;
}

vector<vector<string>>
DatabaseManagerSystem::filtter_result_set(vector<vector<string>> resultSet, FilterExpression filterExpression,
                                          vector<string> columnNames) {
    vector<vector<string>> filterResultSet;
    string columnName = filterExpression.column_name;
    int columnIndex = -1;
    {
        /*
         * 要过滤的列 是否在提供的 列名 中
         * */
        bool isColumnNameValid = false;
        for (int i = 0; i < columnNames.size(); i++) {
            if (columnNames[i] == columnName) {
                isColumnNameValid = true;
                columnIndex = i;
            }
        }
        if (!isColumnNameValid) {
            throw ColumnMissingError(columnName);
        }
    }
    for (int i = 0; i < resultSet.size(); i++) {
        string columnVal = resultSet[i][columnIndex];
        if (filterExpression.check(columnVal)) {
            filterResultSet.emplace_back(resultSet[i]);
        }
    }
    return filterResultSet;
}

void DatabaseManagerSystem::sort(TableManager &tableManager, string filePath, int newTableId, int columnIndexToSort) {
    try {
        string mergeFolder = "tmp/";
        // TODO 内部排序
        if (SORT_MODE & (INTERNAL_SORT)) {
            TableManager newTable(newTableId);
            int blockNum;
            Sorter sorter;
            newTable.load(filePath, false, blockNum, 0);
            for (int i = 0; i < blockNum; i++) {
                if (i != 0) {
                    newTable.load(filePath, false, blockNum, i);
                }
                Block block = newTable.blocks[0];
                vector<TupleData> tuples = block.tuple_datas;
                vector<TupleDataComparable> tupleCmps;
                // block 的 TupleData 转换成 TupleDataComparable
                for (const TupleData &tupleData: tuples) {
                    TupleDataComparable tupleCmp(columnIndexToSort);
                    tupleCmp.n_attr = tupleData.n_attr;
                    tupleCmp.insert(tupleData.datas);
                    tupleCmps.emplace_back(tupleCmp);
                }
                sorter.quickSort(tupleCmps, 0, tupleCmps.size() - 1);
                // TODO 把排序好的结果放到新的 Block
                newTable.blocks.clear();
                for (TupleDataComparable tupleDataComparable:tupleCmps) {
                    newTable.insert_data(tupleDataComparable.datas,
                                         vector<string>(tupleDataComparable.datas.size(), ""));
                }
                // 将新的 Block 写入到 临时文件
                // "./db_folder/" + filePath;
                newTable.save(mergeFolder + to_string(i));
            }
        }

        // TODO merger
        if (SORT_MODE & (MERGE_BLOCK)) {
            vector<string> unmerge = get_folder_filenames("./db_folder/" + mergeFolder);
            while (unmerge.size() > 1) {
                string right = unmerge.back();
                unmerge.pop_back();
            }
        }

    } catch (exception &e) {
        cout << e.what() << endl;
    }
}


void DatabaseManagerSystem::merge_block_in_file(string left, string right) {
    int bufferSize = 1024;
    BlockReader leftReader(bufferSize);
    BlockReader rightReader(bufferSize);
    FILE *leftFilePtr = fopen(left.c_str(), "rb");
    FILE *rightFilePtr = fopen(left.c_str(), "rb");


    int leftOffset = BLOCK_SIZE - Special::get_size();
    int rightOffset = BLOCK_SIZE - Special::get_size();
    // TODO 读取 PageHeaderData
    PageHeaderData leftPHD;
    PageHeaderData rightPHD;
    READ_PAGE_HEADER_DATA(leftReader, leftPHD);
    READ_PAGE_HEADER_DATA(rightReader, rightPHD);

    // TODO 读取 Item
    // n_item = n_tuple
    int n_left_item = CALC_ITEM_COUNT(leftPHD);
    int n_right_item = CALC_ITEM_COUNT(rightPHD);

    MOVE_OFFSET_TO_TUPLE_DATA(n_left_item, leftReader, leftOffset)
    MOVE_OFFSET_TO_TUPLE_DATA(n_right_item, rightReader, rightOffset)

    // TODO 读取 TupleDatas
    for (int i = 0; i < n_left_item; i++) {
        TupleData tupleData;
        // TODO 读取属性数量
        int n_attr = 0;
        READ_EXTERN_BASIC_TYPE(leftReader, int, n_attr, leftOffset);

        // TODO 读取 TupleData
        for (int j = 0; j < n_attr; j++) {
            // 读取大小
            int leftStrSize = 0;
            READ_EXTERN_BASIC_TYPE(leftReader, int, leftStrSize, leftOffset);

            // 读取内容
            char *leftStr = (char *) malloc(leftStrSize);
            READ_EXTERN_COMPLEX_STR(leftReader, leftStrSize, leftStr, leftOffset);
        }
    }

}


