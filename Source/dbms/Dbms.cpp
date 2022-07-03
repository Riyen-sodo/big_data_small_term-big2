#include "Dbms.h"

#define INTERNAL_SORT 1<<1
#define MERGE_BLOCK 1 << 2

#define READ_EXTERN_BASIC_TYPE(READER, TYPE, VAL, OFFSET) \
    memcpy(&(VAL), READER.buffer + OFFSET, sizeof(TYPE));\
    OFFSET += sizeof(TYPE)


#define READ_EXTERN_COMPLEX_STR(READER, SIZE, STR, OFFSET) \
    memcpy(STR, READER.buffer + OFFSET, SIZE);\
    OFFSET += SIZE


#define WRITE_LEFT_TO_TMP_FILE(N_ATTR, LEFT, RIGHT, TMP_FP)\
    fwrite(&(N_ATTR), sizeof(int), 1, TMP_FP);\
    for (int l = 0; l < (N_ATTR); l++) {\
        int size = int((LEFT).tuple[l].size()) + 1;\
        fwrite(&size, sizeof(int), 1, TMP_FP);\
        fwrite((LEFT).tuple[l].c_str(), (LEFT).tuple.size() + 1, 1, TMP_FP);\
    }\
    (LEFT).needRead = true;\
    (RIGHT).needRead = false;\
    (LEFT).tuple.clear();\
    i++


#define WRITE_RIGHT_TO_TMP_FILE(N_ATTR, LEFT, RIGHT, TMP_FP)\
    fwrite(&(N_ATTR), sizeof(int), 1, TMP_FP);\
    for (int l = 0; l < (N_ATTR); l++) {\
    int size = int((RIGHT).tuple[l].size()) + 1;\
    fwrite(&size, sizeof(int), 1, TMP_FP);\
    fwrite((RIGHT).tuple[l].c_str(), size, 1, TMP_FP);\
    }\
    (RIGHT).needRead = true;\
    (LEFT).needRead = false;\
    (RIGHT).tuple.clear();\
    j++


const int SORT_MODE = ((1 << 0) | MERGE_BLOCK);

TupleData get_one_tuple(int &n_tuple, BlockReader &blockReader, int &offset, int &alreadyReady) {
    TupleData tupleData;
    // TODO 读取属性数量
    int n_attr = 0;
    READ_EXTERN_BASIC_TYPE(blockReader, int, n_attr, offset);

    // TODO 读取 TupleData
    for (int j = 0; j < n_attr; j++) {
        // 读取大小
        int strSize = 0;
        READ_EXTERN_BASIC_TYPE(blockReader, int, strSize, offset);

        // 读取内容
        char *str = (char *) malloc(strSize);
        READ_EXTERN_COMPLEX_STR(blockReader, strSize, str, offset);

        tupleData.datas.emplace_back(str);
    }
    alreadyReady++;
    return tupleData;
}

void DatabaseManagerSystem::select(
        const string &tableName,
        const vector<string> &columnNames,
        const FilterExpression &filterExpression,
        TableManager &pg_class,
        TableManager &pg_attribute
) {
    // 异常处理
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

    // 根据表名获取表的 Id
    int table_id;
    {
        table_id = find_table_id_by_name(pg_class, tableName);
        if (table_id == -1) {
            printf("can't find table [ %s ], may be table name error?", tableName.c_str());
            return;
        }
    }

    // 获取列名数组在表中的索引
    vector<int> columnIndexes;
    {
        columnIndexes = get_column_indexes(pg_attribute, columnNames);
        if (columnIndexes.empty()) {
            cout << "The specified column was not found" << endl;
            return;
        }
    }

    // 获取所管理的表 (根据 table_id)
    TableManager target(-1);
    {
        for (const TableManager &tm:this->tables) {
            if (tm.tableId == table_id) {
                target = TableManager(tm);
            }
        }

        if (target.tableId == -1) {
            cout << "Table exists but is not mounted on the database management system" << endl;
            return;
        }
    }

    // 获取 columnNames 中指定的列
    vector<vector<string>> resultSet;
    {

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
    }

    // 过滤
    resultSet = filtter_result_set(resultSet, filterExpression, columnNames);

    // 打印信息
    {
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

}

int DatabaseManagerSystem::find_table_id_by_name(TableManager pg_class, string tableName) {
    int table_id = -1;
    for (Block &block : pg_class.blocks) {
        for (int i = 0; i < block.tuple_datas.size(); i++) {
            TupleData tupleData = block.tuple_datas[i];
            // TODO 硬编码
            // #define TABLE_NAME 0
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
DatabaseManagerSystem::filtter_result_set(
        vector<vector<string>> resultSet,
        FilterExpression filterExpression,
        vector<string> columnNames
) {
    // return : 排好序的结果集
    vector<vector<string>> filterResultSet;
    string columnName = filterExpression.attribute_name;
    int columnIndex = -1;

    // 异常处理
    {
        // 要过滤的列 是否在提供的 列名 中
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

    // 过滤
    {
        for (int i = 0; i < resultSet.size(); i++) {
            string columnVal = resultSet[i][columnIndex];
            if (filterExpression.check(columnVal)) {
                filterResultSet.emplace_back(resultSet[i]);
            }
        }
    }


    return filterResultSet;
}

void DatabaseManagerSystem::sort(TableManager &tableManager, string filePath, int newTableId, int columnIndexToSort) {
    try {
        string mergeFolder = "tmp_tupledata/";
        // 内部排序  (分)
//        if (SORT_MODE & (INTERNAL_SORT))
        {
            TableManager newTable(newTableId);
            int blockNum;
            Sorter sorter;
            // 将文件中的第一个块加载到 newTable, 同时获取到 块的数量
            newTable.load(filePath, false, blockNum, 0);

            // 对每个块进行内部排序，排好序后写入到文件
            {
                for (int i = 0; i < blockNum; i++) {
                    if (i != 0) {
                        newTable.load(filePath, false, blockNum, i);
                    }

                    Block block = newTable.blocks[0];

                    // block 的 TupleData 转换成 TupleDataComparable
                    vector<TupleData> tuples = block.tuple_datas;
                    vector<TupleDataComparable> tupleCmps;
                    {
                        for (const TupleData &tupleData: tuples) {
                            TupleDataComparable tupleCmp(columnIndexToSort);
                            tupleCmp.n_attr = tupleData.n_attr;
                            tupleCmp.insert(tupleData.datas);
                            tupleCmps.emplace_back(tupleCmp);
                        }
                    }

                    // 排序
                    sorter.quickSort(tupleCmps, 0, tupleCmps.size() - 1);

                    // 把排序好的结果放到新的 Block
                    {
                        newTable.blocks.clear();
                        for (const TupleDataComparable &tupleSorted:tupleCmps) {
                            newTable.insert_data(tupleSorted.datas, vector<string>(tupleSorted.datas.size(), ""));
                        }
                    }

                    // 将新的 TupleData 写入到 临时文件
                    newTable.save_tuple_datas(mergeFolder + to_string(i));
                }
            }
        }

        // merger (并)
        if (SORT_MODE & (MERGE_BLOCK)) {
            // 获取所有待排序的文件
            vector<string> unmerge = get_folder_filenames("./db_folder/" + mergeFolder);

            // 直到待排序的文件数量为 1
            while (unmerge.size() > 1) {
                string right = unmerge.front();
                unmerge.erase(unmerge.begin());
                string filePrefix = "./db_folder/" + mergeFolder;
                merge_block_in_file(filePrefix + unmerge.front(), filePrefix + right, columnIndexToSort);

                unmerge = get_folder_filenames("./db_folder/" + mergeFolder);
            }
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}

void DatabaseManagerSystem::merge_block_in_file(string leftFile, string rightFile, int columnIndexToSort) {
    struct Cache {
        vector<string> tuple;
        float val;
        bool needRead;

        Cache() {
            val = 0;
            needRead = true;
        }

        bool operator<(Cache &cache) const {
            return val < cache.val;
        }
    };
    Cache left, right;
    string mergeFileName = "db_folder/tmp_tupledata/tmp";
    FILE *leftFp = fopen(leftFile.c_str(), "rb");
    FILE *rightFp = fopen(rightFile.c_str(), "rb");
    FILE *tmpFp = fopen(mergeFileName.c_str(), "ab");

    // 左/右 文件的 TupleData 数量
    int leftTupleNum, rightTupleNum;
    fread(&leftTupleNum, sizeof(int), 1, leftFp);
    fread(&rightTupleNum, sizeof(int), 1, rightFp);

    // 整个临时文件的 TupleData 数量
    int totalTupleNum = leftTupleNum + rightTupleNum;
    fwrite(&totalTupleNum, sizeof(int), 1, tmpFp);

    int i = 0, j = 0;
    int left_n_attr, right_n_attr;
    int leftStrSize, rightStrSize;

    // 第一次读
    fread(&left_n_attr, sizeof(int), 1, leftFp);
    fread(&right_n_attr, sizeof(int), 1, rightFp);
    if (left_n_attr != right_n_attr) {
        cout << "两个元组的属性数量不一致，无法比较" << endl;
        exit(-1);
    }
    for (int k = 0; k < left_n_attr; k++) {
        if (left.needRead && i < leftTupleNum) {
            fread(&leftStrSize, sizeof(int), 1, leftFp);
            char *leftStr = (char *) malloc(leftStrSize);
            fread(leftStr, leftStrSize, 1, leftFp);
            if (k == columnIndexToSort) {
                left.val = stof(leftStr);
            }
            left.tuple.emplace_back(leftStr);
            free(leftStr);
        }
        if (right.needRead && j < rightTupleNum) {
            fread(&rightStrSize, sizeof(int), 1, rightFp);
            char *rightStr = (char *) malloc(rightStrSize);
            fread(rightStr, rightStrSize, 1, rightFp);
            if (k == columnIndexToSort) {
                right.val = stof(rightStr);
            }
            right.tuple.emplace_back(rightStr);
            free(rightStr);
        }
    }

    while (i < leftTupleNum && j < rightTupleNum) {
        if (left < right) {
//            cout << "左边写入 : i = " << i << endl;
            for (auto x:left.tuple) {
                cout << x << "  ";
            }
            cout << "\n=======" << endl;
            WRITE_LEFT_TO_TMP_FILE(left_n_attr, left, right, tmpFp);
        } else {
//            cout << "右边写入 : j = " << j << endl;
            for (auto x:left.tuple) {
                cout << x << "  ";
            }
            cout << "\n=======" << endl;

            WRITE_RIGHT_TO_TMP_FILE(right_n_attr, left, right, tmpFp);
        }
        if (left.needRead && i < leftTupleNum) {
            fread(&left_n_attr, sizeof(int), 1, leftFp);
            for (int k = 0; k < left_n_attr; k++) {

                fread(&leftStrSize, sizeof(int), 1, leftFp);
                char *leftStr = (char *) malloc(leftStrSize);
                fread(leftStr, leftStrSize, 1, leftFp);
                if (k == columnIndexToSort) {
                    left.val = stof(leftStr);
                }
                left.tuple.emplace_back(leftStr);
                free(leftStr);
            }
        }
        if (right.needRead && j < rightTupleNum) {
            fread(&right_n_attr, sizeof(int), 1, rightFp);
            for (int k = 0; k < right_n_attr; k++) {

                fread(&rightStrSize, sizeof(int), 1, rightFp);

                char *rightStr = (char *) malloc(rightStrSize);
                fread(rightStr, rightStrSize, 1, rightFp);
                if (k == columnIndexToSort) {
                    right.val = stof(rightStr);
                }
                right.tuple.emplace_back(rightStr);
                free(rightStr);
            }
        }

    }


    while (i < leftTupleNum) {
//        cout << "左边写入 : i = " << i << endl;
//        for (auto x:left.tuple) {
//            cout << x << "  ";
//        }
//        cout << "\n=======" << endl;
        WRITE_LEFT_TO_TMP_FILE(left_n_attr, left, right, tmpFp);
        if (left.needRead && i < leftTupleNum) {
            fread(&left_n_attr, sizeof(int), 1, leftFp);
            for (int k = 0; k < left_n_attr; k++) {
                fread(&leftStrSize, sizeof(int), 1, leftFp);
                char *leftStr = (char *) malloc(leftStrSize);
                fread(leftStr, leftStrSize, 1, leftFp);
                if (k == columnIndexToSort) {
                    left.val = stof(leftStr);
                }
                left.tuple.emplace_back(leftStr);
                free(leftStr);
            }
        }
    }

    while (j < rightTupleNum) {
//        cout << "右边写入 : j = " << j << endl;
//        for (auto x:left.tuple) {
//            cout << x << "  ";
//        }
//        cout << "\n=======" << endl;
        WRITE_RIGHT_TO_TMP_FILE(right_n_attr, left, right, tmpFp);
        if (right.needRead && j < rightTupleNum) {
            fread(&right_n_attr, sizeof(int), 1, rightFp);
            for (int k = 0; k < left_n_attr; k++) {

                fread(&rightStrSize, sizeof(int), 1, rightFp);

                char *rightStr = (char *) malloc(rightStrSize);
                fread(rightStr, rightStrSize, 1, rightFp);
                if (k == columnIndexToSort) {
                    right.val = stof(rightStr);
                }
                right.tuple.emplace_back(rightStr);
                free(rightStr);
            }
        }
    }

    fclose(tmpFp);
    fclose(leftFp);
    fclose(rightFp);

    remove(leftFile.c_str());
    remove(rightFile.c_str());
    rename(mergeFileName.c_str(), rightFile.c_str());
}


