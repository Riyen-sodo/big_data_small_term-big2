//
// Created by Flyoung on 2022/6/27.
//
#include "Block.h"
#include "SizeCalculator.h"
#include "errors.h"

#define SIZE_T int

int Block::get_size() {
    int size = PageHeaderData::get_size() + Special::get_size();
    size += SizeCalculator::get_tuple_vec_size(this->tuple_datas);
    size += SizeCalculator::get_item_vec_size(this->items);
    return size;
}

void Block::set_table_id(int table_id) {
    this->phd.table_id = table_id;
}

void Block::insert(const vector<string> &data_vec, const vector<string> &attr_desc) {

    // TupleData 的大小
    int data_size = SizeCalculator::get_string_vec_size(data_vec) + sizeof(SIZE_T);

    // 如果块内空间不足，则抛出异常
    int free_space = this->phd.free_space_end - this->phd.free_space_start;
    if (free_space < data_size) {
        throw BlockSpaceError();
    }

    // 将 TupleData 装入临时变量
    TupleData td;
    td.insert(data_vec, attr_desc);
    this->tuple_datas.push_back(td);

    // 已经在块内的 TupleData 的大小（data_vec已经装进去了）
    int tuple_vec_size = SizeCalculator::get_tuple_vec_size(this->tuple_datas);

    // 计算 Item
    Item it;
    this->phd.free_space_end = BLOCK_SIZE - 1 - tuple_vec_size - Special::get_size();

    // 插入 Item
    it.insert(this->phd.free_space_end, data_size);
    this->phd.free_space_end -= data_size;
    this->items.push_back(it);

    // 修改 phd
    this->phd.free_space_start += Item::get_size();
}

void Block::print() {
    int block_id = this->phd.table_id;
    cout << "========================  Start Block (" << block_id
         << ")  ========================\n"
         << endl;
    printf(" Block Size[ %d ] \n\n", this->get_size());
    cout << "===========  PageHeaderData  ===========" << endl;
    this->phd.print();
    cout << "\n\n===========  Item  ===========" << endl;
    for (Item it : this->items) {
        it.print();
    }
    cout << "\n\n===========  TupleData  ===========" << endl;
    for (TupleData td : this->tuple_datas) {
        td.print();
    }
    cout << "\n\n===========  Special  ===========" << endl;
    this->special.print();
    cout << "\n\n========================  End Block (" << block_id
         << ") ========================\n\n"
         << endl;
}

Block::Block(PageHeaderData phd) {
    this->phd = phd;
}

Block::Block() {
    this->phd = PageHeaderData(
            BLOCK_SIZE - Special::get_size(),
            PageHeaderData::get_size(),
            BLOCK_SIZE - Special::get_size(),
            0
    );
}

