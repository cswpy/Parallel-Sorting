#ifndef MERGER_HPP
#define MERGER_HPP

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>
#include "record.hpp"

class merger{
    public:
        int num_of_worker;
        bool is_desc;
        merger();
        void sort();
        void merge();
};

#endif