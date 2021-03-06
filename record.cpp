#include <iostream>
#include <cstring>
#include <string>
#include "record.hpp"

using namespace std;

record::record(int ID, string first_name, string last_name, int num_dependent, int income, int zipcode): ID(ID), first_name(first_name), last_name(last_name), num_dependent(num_dependent), income(income), zipcode(zipcode)
{}

bool record::compare_records(record* rec, int field, bool reverse = false) {
    bool ans;
    if(field == 0)
        ans = this->ID > rec->ID;
    else if(field == 3)
        ans = this->num_dependent > rec->num_dependent;
    else if(field == 4)
        ans = this->income > rec->income;
    else if(field == 5)
        ans = this->zipcode > rec->zipcode;
    else
        ans = NULL;
    if(reverse)
        return !ans;
    else
        return ans;
}

void record::print_record(){
    cout << ID << "\t" << first_name << "\t" << last_name << "\t" << num_dependent << "\t" << income << "\t" << zipcode << endl;
}

void record::write_record(fstream FILE) {
    FILE << ID << " " << first_name << " " << last_name << " " << num_dependent << " " << income << " " << zipcode << endl;
}
