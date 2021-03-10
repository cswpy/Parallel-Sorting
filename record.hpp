#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <fstream>

class record{
    public:
        int ID;
        std::string first_name;
        std::string last_name;
        int num_dependent;
        float income;
        int zipcode;
        record(int, std::string, std::string, int, float, int);
        bool compare_records(record*, int, bool);
        void print_record();
        void write_record(std::fstream FILE); 
};
#endif
