#include <iostream>
#include <cstring>
#include <string>
#include "record.hpp"

using namespace std;

record::record(int ID, string first_name, string last_name, int num_dependent, int income, int zipcode): ID(ID), first_name(first_name), last_name(last_name), num_dependent(num_dependent), income(income), zipcode(zipcode)
{}

bool record::compare_records(record* rec, int field) {
    if(field == 0)
        return this->ID > rec->ID;
    else if(field == 3)
        return this->num_dependent > rec->num_dependent;
    else if(field == 4)
        return this->income > rec->income;
    else if(field == 5)
        return this->zipcode > rec->zipcode;
    else
        return NULL;
}

void record::print_record(){
    cout << ID << " " << first_name << " " << last_name << " " << num_dependent << " " << income << " " << zipcode << endl;
}

void record::write_record(fstream FILE) {
    FILE << ID << " " << first_name << " " << last_name << " " << num_dependent << " " << income << " " << zipcode << endl;
}
