#include "Dbms.h"


void DatabaseManagerSystem::insert(
        BlockManager &bm,
        const vector<string> &data_vec,
        const vector<string> &attr_desc
) {
    bm.insert(data_vec, attr_desc);
}

void DatabaseManagerSystem::select(
        const string &table_name,
        const vector<string> &column_name_vec,
        const FilterExpression &fe
) {
    int pg_class_id = 1001;
    BlockManager pg_class_buffer(pg_class_id);
    BlockManager::from_file(pg_class_buffer, pg_class_id);
//    pg_class_buffer.print();
}