#ifndef DBMS
#define DBMS
#include "PublicIncludes.h"
#include "database.h"
#include "errors.h"
#include "utils.h"

struct FilterExpression {
    string column_name;
    char comparator;
    string value;
};


struct DatabaseManagerSystem {
    /**
     * 将数据插入到表中
     * @param bm
     * @param data_vec 数据数组
     * @param attr_desc 属性描述数组
     */
    static void insert(BlockManager &bm, const vector <string>& data_vec, const vector <string>& attr_desc);

    /**
     * 从表中查询数据
     * @param table_name 表名
     * @param column_name_vec 列名数组
     * @param fe 过滤表达式
     */
    void select(const string& table_name, const vector <string>& column_name_vec, const FilterExpression& fe);
};
#endif