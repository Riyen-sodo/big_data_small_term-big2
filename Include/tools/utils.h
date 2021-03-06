#ifndef UTILS
#define UTILS

#include "PublicIncludes.h"


vector<string> get_folder_filenames(const string &folder);


/**
 * 获取一个长度为 n 的随机名字
 * @param len
 * @return 随机名字
 */
string get_random_name(int len);

/**
 * 获取 n 个长度为 m 的随机字符串
 * @param m
 * @param n
 * @return 随机字符串
 */
vector<string> get_random_string(int m, int n);

/**
 * 根据 spliter 分隔字符串 str
 * @param str
 * @param spliter
 * @return
 */
vector<string> split_string(string str, char spliter);

bool is_string_vector_repeat(vector<string> stringVec);

vector<string> string_vector_deduplication(vector<string> stringVec);

#endif