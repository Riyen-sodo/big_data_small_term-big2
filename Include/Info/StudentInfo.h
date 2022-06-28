//
// Created by Flyoung on 2022/6/27.
//

#ifndef DEMO2_STUDENTINFO_H
#define DEMO2_STUDENTINFO_H

#include "PublicIncludes.h"
#include "InfoObject.h"

struct StudentInfo : public InfoObject {
    int sno;
    string sname;
    int sage;

    StudentInfo() {}

    StudentInfo(int sno, string sname, int sage) : sno(sno), sname(sname), sage(sage) {}

    vector<string> to_string_vec();
};

#endif //DEMO2_STUDENTINFO_H
