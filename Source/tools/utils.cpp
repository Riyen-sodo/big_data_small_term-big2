
#include "utils.h"
#include <unordered_map>
#include <utility>
#include <unordered_set>

vector<string> get_folder_filenames(const string &folder) {
    vector<string> filenames;
    for (const auto &file : directory_iterator(folder)) {
        string s = file.path().string();
        for (size_t i = s.size() - 1; i >= 0; i--) {
            if (s[i] == '\\' or s[i]=='/') {
                s = s.substr(i + 1, s.size() - 1);
                break;
            }
        }
        filenames.push_back(s);
    }
    return filenames;
}


string get_random_name(int len) {
    string name;
    char first_name = char(int(65 + rand() % 26));  // A~Z  65~90
    name.push_back(first_name);
    for (int i = 0; i < len; i++) {
        // a~z  97~122
        name.push_back(int(97 + rand() % 26));
    }
    return name;
}

vector<string> get_random_string(int m, int n) {
    vector<string> ret;
    for (int i = 0; i < n; i++) {
        ret.push_back(get_random_name(m));
    }
    return ret;
}

vector<string> split_string(string str, char spliter) {
    vector<string> ret;
    while (!str.empty()) {
        int idx = str.find_first_of(spliter);
        if (idx == -1) {
            ret.push_back(str);
            str.clear();
        } else {
            ret.push_back(str.substr(0, idx));
            str = str.substr(idx + 1, str.size() - idx - 1);
        }
    }
    return ret;
}

bool is_string_vector_repeat(vector<string> stringVec) {
    std::unordered_map<string, int> strMap;
    for (const string &str:stringVec) {
        strMap[str]++;
    }
    return strMap.size() == stringVec.size();
}


vector<string> string_vector_deduplication(vector<string> stringVec) {
    vector<string> ret;
    std::unordered_set<string> strSet;
    for (string str:stringVec) {
        if (strSet.empty() || strSet.count(str) == 0) {
            ret.push_back(str);
            strSet.insert(str);
        }
    }
    return ret;
}
