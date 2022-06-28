#include "PageHeaderData.h"

void PageHeaderData::print() {
    printf(
            "start special : %d\nfree space start : %d\nfree space end : "
            "%d\ntable id : %d\n",
            start_special, free_space_start, free_space_end, table_id);
}

int PageHeaderData::get_size() {
    return sizeof(PageHeaderData);
}

void PageHeaderData::serialization(FILE *fp, int &offset) {
    fwrite(&start_special, sizeof(start_special), 1, fp);
    offset += sizeof(start_special);

    fwrite(&free_space_start, sizeof(free_space_start), 1, fp);
    offset += sizeof(free_space_start);

    fwrite(&free_space_end, sizeof(free_space_end), 1, fp);
    offset += sizeof(free_space_end);

    fwrite(&table_id, sizeof(table_id), 1, fp);
    offset += sizeof(table_id);
}


void PageHeaderData::deserialize(FILE *fp, int &offset) {
    fread(&start_special, sizeof(start_special), 1, fp);
    offset += sizeof(start_special);

    fread(&free_space_start, sizeof(free_space_start), 1, fp);
    offset += sizeof(free_space_start);

    fread(&free_space_end, sizeof(free_space_end), 1, fp);
    offset += sizeof(free_space_end);

    fread(&table_id, sizeof(table_id), 1, fp);
    offset += sizeof(table_id);
}

PageHeaderData::PageHeaderData(int startSpecial, int freeSpaceStart, int freeSpaceEnd, int tableId) : start_special(
        startSpecial), free_space_start(freeSpaceStart), free_space_end(freeSpaceEnd), table_id(tableId) {}

void PageHeaderData::write_to_IOBuffer(char *buffer, int &offset) {
    cout << "void PageHeaderData::write_to_IOBuffer(char *buffer, int &offset) { " << offset << endl;
    memcpy(buffer + offset, &start_special, sizeof(start_special));
    offset += sizeof(start_special);

    memcpy(buffer + offset, &free_space_start, sizeof(free_space_start));
    offset += sizeof(free_space_start);

    memcpy(buffer + offset, &free_space_end, sizeof(free_space_end));
    offset += sizeof(free_space_end);

    memcpy(buffer + offset, &table_id, sizeof(table_id));
    offset += sizeof(table_id);
    cout << "void PageHeaderData::write_to_IOBuffer(char *buffer, int &offset) { end" << offset << endl;

}

void PageHeaderData::read_from_IOBuffer(char *buffer, int &offset) {
    offset = 0;
    cout << "void PageHeaderData::read_from_IOBuffer(char *buffer, int &offset) {  " << offset << endl;
    memcpy(&start_special, buffer + offset, sizeof(start_special));
    offset += sizeof(start_special);

    memcpy(&free_space_start, buffer + offset, sizeof(free_space_start));
    offset += sizeof(free_space_start);

    memcpy(&free_space_end, buffer + offset, sizeof(free_space_end));
    offset += sizeof(free_space_end);

    memcpy(&table_id, buffer + offset, sizeof(table_id));
    offset += sizeof(table_id);
    printf(
            "start special : %d\nfree space start : %d\nfree space end : "
            "%d\ntable id : %d\n",
            start_special, free_space_start, free_space_end, table_id);
    cout << "void PageHeaderData::read_from_IOBuffer(char *buffer, int &offset) {  end" << offset << endl;

}


