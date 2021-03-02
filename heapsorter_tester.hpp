#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>

#include "record.hpp"

class record;


class heapsorter{
    public:
        int size;
        int order_field;
        bool is_max_heap;
        heapsorter(int size, int order_field, bool is_max_heap);
        void sort(int*);
        void swap(int*, int, int);
        void heapify(int*,int, int);
};