#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <fstream>

class record{
    private:
        int ID;
        std::string first_name;
        std::string last_name;
        int num_dependent;
        int income;
        int zipcode;
    public:
        record(int, std::string, std::string, int, int, int);
        bool compare_records(record, int);
        void print_record();
        void write_record(std::fstream FILE); 
};

