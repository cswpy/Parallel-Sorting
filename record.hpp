#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <fstream>

class record{
    public:
        int ID;
        std::string original_string;
        int num_dependent;
        float income;
        int zipcode;
        record(std::string);
        bool compare_records(record* rec, int field, bool reverse = false);
        bool compare_records(record rec, int field, bool reverse = false);
        void print_record();
        friend std::ostream& operator<<(std::ostream &os, const record& R);
        friend std::istream& operator>>(std::ostream &is, const record& R);
};
#endif
