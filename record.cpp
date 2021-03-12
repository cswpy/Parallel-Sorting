#include <iostream>
#include <cstring>
#include <string>

#include "record.hpp"

using namespace std;

record::record(string original_string): original_string(original_string){
    this->ID = stoi(original_string.substr(0, 7));
    this->num_dependent = stoi(original_string.substr(34, 1));
    this->income = stof(original_string.substr(38, 8));
    this->zipcode = stoi(original_string.substr(47, string::npos));
}

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
    cout << original_string << endl;
}

std::ostream& operator<<(std::ostream &os, const record& R){
    os << R.original_string;
    return os;
}
std::istream& operator>>(std::istream &is, record& R){
    is >> R.original_string;
    return is;
}