//
// Created by Flyoung on 2022/6/27.
//

#include "errors.h"

const char *BlockSpaceError::what() const throw() {
    return "Not enough block space";
}

const char *TableIdMissingError::what() const throw() {
    return "Table id has not been set";
}

const char *TableIdRepeatError::what() const throw() {
    return ("Table id " + to_string(this->repeat_table_id) + " already exists").c_str();
}

TableIdRepeatError::TableIdRepeatError(int repeat_table_id) : repeat_table_id(repeat_table_id) {}

string TableIdRepeatError::error() {
    return ("Table id " + to_string(this->repeat_table_id) + " already exists").c_str();
}

FileCloseError::FileCloseError(string filePath) {
    this->filePath = filePath;
}

string FileCloseError::error() {
    return this->filePath + " close failed!";
}

string FileWriteError::error() {
    return this->filePath + " write failed!";
}

FileWriteError::FileWriteError(string filePath) {
    this->filePath = filePath;
}

FileOpenError::FileOpenError(string filePath) {
    this->filePath = filePath;
}

string FileOpenError::error() {
    return this->filePath + " open failed!";
}

FilePtrMoveError::FilePtrMoveError(string filePath) {
    this->filePath = filePath;
}

string FilePtrMoveError::error() {
    return this->filePath + " fseek failed!";
}

const char *MemoryCopyError::what() const throw() {
    return "memcpy error!";
}

const char *ExpressionError::what() const throw() {
    return "format of expression error!";
}

const char *TypeConvertError::what() const throw() {
    return "type convert error!";
}

const char *StringVectorRepeatError::what() const throw() {
    return "element of string vector repeat!";
}

const char *VectorIndexError::what() const throw() {
    return "The index is out of the addressing range of the vector!";
}

string ColumnMissingError::error() {
    return "column [" + this->columnName + "] not found!";
}

ColumnMissingError::ColumnMissingError(string columnName) {
    this->columnName = columnName;
}
