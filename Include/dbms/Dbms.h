#ifndef DBMS
#define DBMS
#include "PublicIncludes.h"
#include "database.h"
#include "errors.h"
#include "utils.h"
#include "FilterExpression.h"


struct DatabaseManagerSystem {
    /**
     * 从表中查询数据
     * @param table_name 表名
     * @param column_name_vec 列名数组
     * @param fe 过滤表达式
     */
    void select(const string& table_name, const vector <string>& column_name_vec, const FilterExpression& fe);
};
#endif