#ifndef ERRORS
#define ERRORS
#define FILE_OPEN_ERROR -1

#define FILE_CLOSE_ERROR -2

#define FILE_WRITE_ERROR -3

#define MEMORY_COPY_ERROR -4

#include "PublicIncludes.h"

/**
 * @brief 块空间异常
 * 如果【要插入的数据 + 块内数据 > 块大小】，则抛出此异常
 *
 */
struct BlockSpaceError : public exception {
    const char *what() const throw();
};

/**
 * @brief 表Id缺失异常
 * 如果将表的内容存放到文件中之前，没用设置表的Id，则抛出此异常
 */
struct TableIdMissingError : public exception {
    const char *what() const throw();
};

/**
 * @brief 表Id重复异常
 * 如果将表保存到文件时，该表的 Id 已经存在，则抛出此异常
 */
struct TableIdRepeatError : public exception {
    int repeat_table_id;

    TableIdRepeatError(int repeat_table_id);

    const char *what() const throw();

    string error();
};

/**
 * 文件关闭异常
 */
struct FileCloseError : public exception {
    string filePath;

    FileCloseError(string filePath);

    string error();
};

/**
 * 文件写入异常
 */
struct FileWriteError : public exception {
    string filePath;

    FileWriteError(string filePath);

    string error();
};

/**
* 文件打开异常
*/
struct FileOpenError : public exception {
    string filePath;

    FileOpenError(string filePath);

    string error();
};

struct FilePtrMoveError : public exception {
    string filePath;

    FilePtrMoveError(string filePath);

    string error();
};

struct MemoryCopyError : public exception {
    const char *what() const throw();
};

struct ExpressionError : public exception {
    const char *what() const throw();
};

#endif
