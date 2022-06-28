#include "Special.h"

int Special::get_size() {
    return sizeof(Special);
}

void Special::print() {
    cout << "Special" << endl;
}

void Special::serialization(FILE *fp, int &offset) {
    fwrite(&reserved_bit, sizeof(reserved_bit), 1, fp);
    offset += sizeof(reserved_bit);
}

void Special::deserialize(FILE *fp, int &offset) {
    fread(&reserved_bit, sizeof(reserved_bit), 1, fp);
    offset += sizeof(reserved_bit);
}

Special::Special() {
    reserved_bit = '\0';
}
