```cpp
try {
        int bufferSize = 1024;
        BlockReader leftReader(bufferSize);
        BlockReader rightReader(bufferSize);
        FILE *leftFilePtr = fopen(leftFile.c_str(), "rb");
        FILE *rightFilePtr = fopen(rightFile.c_str(), "rb");

        int leftOffset = BLOCK_SIZE - Special::get_size();
        int rightOffset = BLOCK_SIZE - Special::get_size();

        // TODO 读取 PageHeaderData
        PageHeaderData leftPHD;
        PageHeaderData rightPHD;
        fread(&leftPHD, sizeof(leftPHD), 1, leftFilePtr);

        int n_left_item = (leftPHD.free_space_start - PageHeaderData::get_size()) / (Item::get_size());

        for (int j = 0; j < n_left_item; j++) {
            // TODO 读取 Item
            Item leftItem;
            fseek(leftFilePtr, PageHeaderData::get_size() + j * Item::get_size(), 0);
            fread(&leftItem, Item::get_size(), 1, leftFilePtr);

            // TODO 根据 Item 定位到 TupleData
            int leftTupleStart = leftItem.tuple_data_start;
            int leftTupleSize = leftItem.tuple_data_start;
            fseek(leftFilePtr, leftTupleStart, 0);

            // TODO 读取 TupleData
            TupleDataComparable leftTupleCmp(columnIndexToSort);

            // TODO 读取 n_attr
            vector<string> data;
            fread(&leftTupleCmp.n_attr, sizeof(int), 1, leftFilePtr);

            for (int i = 0; i < leftTupleCmp.n_attr; i++) {
                // TODO 读取大小
                int strSize = 0;
                fread(&strSize, sizeof(int), 1, leftFilePtr);
                // TODO 读取内容
                char *str = static_cast<char *>(malloc(strSize));
                fread(str, strSize, 1, leftFilePtr);
                data.emplace_back(str);
                free(str);
            }
            leftTupleCmp.insert(data);
            leftTupleCmp.print();
        }

    } catch (exception &e) {
        cout << e.what() << endl;
    }
```





### 代码

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <algorithm>

using namespace std;


// 角色
struct Role {
    // 角色名
    string roleName;

    // 操作是否有 *
    bool optionHasStar;

    // 种类是否有 *
    bool categoryHasStar;

    Role() {
        optionHasStar = false;
        categoryHasStar = false;
    }

    // 关联用户
    vector<string> unionUsers;

    // 关联用户组
    vector<string> unionGroups;

    // 操作清单
    vector<string> operations;

    // 资源种类清单
    vector<string> categories;

    // 资源名清单
    vector<string> resNames;


    bool is_category_valid(const string &category) {
        return find(this->categories.begin(), this->categories.end(), category) == this->categories.end();
    }

    bool is_resource_name_valid(const string &resName) {
        return find(this->resNames.begin(), this->resNames.end(), resName) == this->resNames.end();
    }

    bool is_option_on_resource_Valid(const string &operation, const string &category, const string &resName) {
        // 如果操作清单为空
        if (this->operations.empty()) {
            return false;
        }

        // 允许访问任意类的资源，但资源名不为空
        if (this->categoryHasStar && !this->resNames.empty()) {
            return false;
        }

        // 如果操作清单不是任意的，而且没有指定的操作
        if (!this->optionHasStar &&
            find(this->operations.begin(), this->operations.end(), operation) == this->operations.end()) {
            return false;
        }

        // 有该类资源，但没有该名称的资源
        if (is_category_valid(category) && !is_resource_name_valid(resName)) {
            return false;
        }

        return true;
    }
};

bool check_operation(const string &userName,
                     unordered_map<string, Role> &userInfo,
                     const string &operation,
                     const string &category,
                     const string &resName
) {
    Role needCheck = userInfo[userName];
    for (const string &groupName : needCheck.unionGroups) {
        for (const pair<string, Role> &rolePair:userInfo) {

            Role role = rolePair.second;

            if (role.is_option_on_resource_Valid(operation, category, resName)) {
                return true;
            }

            if (find(role.unionGroups.begin(), role.unionGroups.end(), groupName) != role.unionGroups.end()) {
                if (role.is_option_on_resource_Valid(operation, category, resName)) {
                    return true;
                }
            }

            if (find(role.unionUsers.begin(), role.unionUsers.end(), userName) != role.unionUsers.end()) {
                if (role.is_option_on_resource_Valid(operation, category, resName)) {
                    return true;
                }
            }
        }
    }
    return false;
}

template<typename T>
void printVector(vector<T> &arr) {
    for (auto x:arr) {
        cout << x << "  ";
    }
    cout << endl;
}

int main() {
    int n, m, q;
    cin >> n >> m >> q;


    unordered_map<string, Role> userInfo;

    // 角色
    for (int i = 0; i < n; i++) {
        Role role;

        string userName;
        cin >> userName;
        role.roleName = userName;

        int n_option;
        cin >> n_option;


        for (int j = 0; j < n_option; j++) {
            string option;
            cin >> option;
            if (option == "*") {
                role.optionHasStar = true;
                continue;
            }
            role.operations.emplace_back(option);
        }


        int n_category;
        cin >> n_category;


        for (int j = 0; j < n_category; j++) {
            string category;
            cin >> category;
            if (category == "*") {
                role.categoryHasStar = true;
                continue;
            }
            role.categories.emplace_back(category);
        }

        int n_resource_name;
        cin >> n_resource_name;

        for (int j = 0; j < n_resource_name; j++) {
            string resName;
            cin >> resName;
            role.resNames.emplace_back(resName);
        }

        userInfo[userName] = role;
    }

    // 关联
    for (int i = 0; i < m; ++i) {
        string userName;
        cin >> userName;


        int n_union;
        cin >> n_union;
        for (int j = 0; j < n_union; j++) {
            string type;
            string userOrGroup;
            cin >> type >> userOrGroup;
            if (type == "u") {
                userInfo[userName].unionUsers.emplace_back(userOrGroup);
            } else {
                // type = g
                userInfo[userName].unionGroups.emplace_back(userOrGroup);
            }
        }
    }

    // 检查
    for (int i = 0; i < q; ++i) {
        unordered_map<string, Role> userInfo_(userInfo);

        string userName;
        cin >> userName;

        int n_group;
        cin >> n_group;

        vector<string> groupNames;
        for (int j = 0; j < n_group; j++) {
            string groupName;
            cin >> groupName;
            groupNames.emplace_back(groupName);
        }

        string option, category, resName;
        cin >> option >> category >> resName;


        userInfo_[userName].unionGroups.insert(userInfo_[userName].unionGroups.end(), groupNames.begin(),
                                               groupNames.end());


        if (check_operation(userName, userInfo_, option, category, resName)) {
            cout << 1 << endl;
        } else {
            cout << 0 << endl;
        }
    }

//    printVector(userInfo["doorman"].unionUsers);
    return 0;
}
```



### 样例1

输入

```
1 2 4
doorman 2 open close 1 door 2 room1 room2
doorman 3 u foo1 u foo2 g bar
foo4 1 g barz
foo1 0 open door room3
foo2 0 open door room1
foo3 1 bar open door room1
foo4 1 barz open door room1
```

输出

```
1
1
1
0
```

```
// 操作清单包含 O
        if (find(this->operations.begin(), this->operations.end(), operation) == this->operations.end()) {
            // 资源种类清单包含 C
            if (find(this->categories.begin(), this->categories.end(), category) == this->categories.end()) {
                // 资源名单包含 R
                if (find(this->resNames.begin(), this->resNames.end(), resName) == this->resNames.end()) {
                    return true;
                }
                    // 资源名单不包含 R
                else {
                    return this->resNames.empty();
                }
            }
                // 资源种类清单不包含 C
            else {
                // 资源种类清单包含 *
                if (this->categoryHasStar) {
                    // 资源名单包含 R
                    if (find(this->resNames.begin(), this->resNames.end(), resName) == this->resNames.end()) {
                        return true;
                    }
                        // 资源名单不包含 R
                    else {
                        return this->resNames.empty();
                    }
                } else {
                    return false;
                }
            }
        }
            // 操作清单不包含 O
        else {
            // 操作清单包含 *
            if (this->optionHasStar) {
                // 资源种类清单包含 C
                if (find(this->categories.begin(), this->categories.end(), category) == this->categories.end()) {
                    // 资源名单包含 R
                    if (find(this->resNames.begin(), this->resNames.end(), resName) == this->resNames.end()) {
                        return true;
                    }
                        // 资源名单不包含 R
                    else {
                        return this->resNames.empty();
                    }
                }
                    // 资源种类清单不包含 C
                else {
                    // 资源种类清单包含 *
                    if (this->categoryHasStar) {
                        // 资源名单包含 R
                        if (find(this->resNames.begin(), this->resNames.end(), resName) == this->resNames.end()) {
                            return true;
                        }
                            // 资源名单不包含 R
                        else {
                            return this->resNames.empty();
                        }
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
```

