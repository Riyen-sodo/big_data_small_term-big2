#include "InfoObject.h"

void InfoObject::print() {
    for (const string& s : this->to_string_vec()) {
        cout << s << endl;
    }
}

vector <string> InfoObject::to_string_vec() {
    return vector<string>();
}