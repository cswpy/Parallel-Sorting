#ifndef MERGESORTER_HPP
#define MERGESORTER_HPP

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>
#include "record.hpp"

class record;

class mergesorter{
    public:
        int size;
        int order_field;
        bool is_asc;
        mergesorter(int size, int order_field, bool is_asc);
        void sort(record** record_array);
        void mergeSort(record** record_array, int, int);
        void merge(record** record_array, int, int, int);
};

#endif