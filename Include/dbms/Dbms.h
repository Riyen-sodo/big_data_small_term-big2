#ifndef DBMS
#define DBMS

#include "PublicIncludes.h"
#include "database.h"
#include "errors.h"
#include "utils.h"
#include "FilterExpression.h"
#include "TableManager.h"
#include "utils.h"
#include "Sorter.h"
#include "TupleDataComparable.h"
#include "BlockReader.h"

#define DEBUG 1<<1
#define SHOW_SELECT 1<<2
const int RUN_MODE = ((1 << 0) | SHOW_SELECT);

struct DatabaseManagerSystem {

    vector<TableManager> tables;

    /**
     * 把一张表挂载到DBMS
     * @param tableManager 表
     */
    void mount(TableManager &tableManager);

    /**
     * 将多张表挂载到DBMS
     * @param tables
     */
    void mount(vector<TableManager> tables);

    /**
     * 筛选出表中指定的列和行
     * @param tableName 表名
     * @param columnNames 列名数组
     * @param filterExpression 过滤表达式
     * @param pg_class 表
     */
    void select(
            const string &tableName,
            const vector<string> &columnNames,
            const FilterExpression &filterExpression,
            TableManager &pg_class,
            TableManager &pg_attribute
    );

    /**
     * 根据表名查找表
     * @param pg_class 表表 :-)
     * @param tableName 表名
     * @return
     */
    int find_table_id_by_name(TableManager pg_class, string tableName);

    /**
     * 获取指定列在表中的位置
     * @param tableManager 表
     * @param columnNames 列名
     * @return
     */
    vector<int> get_column_indexes(TableManager &tableManager, vector<string> columnNames);

    /**
     * 获取表中指定列的元组
     * @param columnIndexes 指定的列
     * @param target 表
     * @return
     */
    vector<vector<string>> get_target_columns(vector<int> columnIndexes, TableManager &target);

    /**
     * 过滤查询结果集
     * @param resultSet 查询结果集
     * @param filterExpression 过滤表达式
     * @param columnNames 列名
     * @return 过滤后的查询结果集
     */
    vector<vector<string>>
    filtter_result_set(vector<vector<string>> resultSet,
                       FilterExpression filterExpression,
                       vector<string> columnNames);

    /**
     * 对块进行内部排序
     * @param tableManager
     * @param filePath 要排序的表文件路径
     * @param newTableId
     * @param columnIndexToSort 要排序的字段
     */
    void sort(TableManager &tableManager, string filePath, int newTableId, int columnIndexToSort);

    /**
     * 将两个块的数据 有序的 合并到一个块中
     * @param left
     * @param right
     * @param columnIndexToSort
     */
    void merge_block_in_file(string left, string right, int columnIndexToSort);
};

#endif