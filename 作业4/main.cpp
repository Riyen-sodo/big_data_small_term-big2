#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>

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

    // 操作清单
    unordered_set<string> operations;

    // 资源种类清单
    unordered_set<string> categories;

    // 资源名清单
    unordered_set<string> resNames;

    bool is_option_on_resource_Valid(const string &operation, const string &category, const string &resName) const {
        // 会快吧,1ms
        if (!this->optionHasStar && !operations.count(operation)) {
            return false;
        }
        if (!this->categoryHasStar && !categories.count(category)) {
            return false;
        }
        if (!this->resNames.empty() && !resNames.count(resName)) {
            return false;
        }
        return true;
    }
};


// 根据用户名获取关联的用户
unordered_map<string, unordered_set<string>> roleMap;

// 根据组名获取组内用户
unordered_map<string, unordered_set<string>> groupMap;

bool check_operation(const string &userName,
                     unordered_map<string, Role> &userInfo,
                     const vector<string> &groupNames,
                     const string &operation,
                     const string &category,
                     const string &resName
) {
    Role &needCheck = userInfo[userName];

    // 自己能不能执行
    if (needCheck.is_option_on_resource_Valid(operation, category, resName)) {
        return true;
    }

    // 有没有直接跟该用户关联的，而且可以执行该操作的
    // roleMap[userName] : 关联 userName 的用户
    for (const auto &roleName:roleMap[userName]) {
        if (userInfo[roleName].is_option_on_resource_Valid(operation, category, resName)) {
            return true;
        }
    }



    // 关联的用户组里面，有没有谁是可以执行该操作的
    // groupMap[groupName] : 在 groupName 中的用户
    for (const string &groupName:groupNames) {
        for (const auto &role:groupMap[groupName]) {
            if (userInfo[role].is_option_on_resource_Valid(operation, category, resName)) {
                return true;
            }
        }
    }
    return false;
}


int main() {
    int n, m, q;
    cin >> n >> m >> q;


    unordered_map<string, Role> userInfo;

    // 角色
    for (int i = 0; i < n; i++) {
        string userName;
        cin >> userName;

        userInfo[userName] = Role();
        Role &role = userInfo[userName];
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
            role.operations.insert(option);
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
            role.categories.insert(category);
        }


        int n_resource_name;
        cin >> n_resource_name;
        for (int j = 0; j < n_resource_name; j++) {
            string resName;
            cin >> resName;
            role.resNames.insert(resName);
        }


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
                roleMap[userOrGroup].insert(userName);
            } else {
                groupMap[userOrGroup].insert(userName);
            }
        }
    }


    // 检查
    for (int i = 0; i < q; ++i) {
        string userName;
        cin >> userName;

        userInfo[userName] = Role();
        Role &role = userInfo[userName];
        role.roleName = userName;

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


        if (check_operation(userName, userInfo, groupNames, option, category, resName)) {
            cout << 1 << "\n";
        } else {
            cout << 0 << "\n";
        }
        userInfo.erase(userName);
    }
    return 0;
}