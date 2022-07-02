//
// Created by Flyoung on 2022/6/27.
//

#include "TupleData.h"

#define SIZE_T int


void TupleData::print() {
    for (const string &s : this->datas) {
        cout << setw(20) << s;
    }
    cout << endl;
}

int TupleData::get_size() {
    int size = sizeof(n_attr) + sizeof(int) * n_attr;
//    cout << "初始大小" << size << endl;
    for (int i = 0; i < datas.size(); i++) {
        size += (datas[i].size() + 1);
    }
    return size;
}

void TupleData::insert(vector<string> data_vec, const vector<string> &attr_desc) {
    this->datas = data_vec;
    this->n_attr = attr_desc.size();
}

void TupleData::serialization(FILE *fp, int &offset) {
    fwrite(&n_attr, sizeof(n_attr), 1, fp);
    for (string &str : this->datas) {
        int size = str.size() * sizeof(char);

        // 写入数据项大小
        fwrite(&size, sizeof(SIZE_T), 1, fp);
        offset += sizeof(SIZE_T);


        // 写入数据项内容
        fwrite(str.c_str(), 1, size, fp);
        offset += size;
    }
}

void TupleData::deserialize(FILE *fp, int &offset) {
    fread(&this->n_attr, sizeof(n_attr), 1, fp);
    char *str = nullptr;
    for (int i = 0; i < n_attr; i++) {
        int size = 0;

        // 读取数据项大小
        fread(&size, sizeof(SIZE_T), 1, fp);
        size *= sizeof(char);
        offset += sizeof(SIZE_T);

        if (size == 0) {
            continue;
        }

        // 读取数据项内容
        str = (char *) calloc(1, size);
        fread(str, size, 1, fp);
        offset += size;

        this->datas.emplace_back(str);
    }
    delete[] str;
}