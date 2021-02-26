#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>

#include "heapsorter.hpp"
#include "record.hpp"

using namespace std;

class record;

heapsorter::heapsorter(int size, int order_field, bool is_max_heap): size(size), order_field(order_field), is_max_heap(is_max_heap)
{}

void heapsorter::sort(record** record_heap){
    
}

void heapsorter::heapify(record** record_heap, int record_idx){
    int largest = record_idx;
    int l = 2 * record_idx + 1;
    int r = 2 * record_idx + 2;

    if(l < size && record_heap[l]->compare_records(record_heap[largest], order_field))
}


int main(int argc, char *argv[]){
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);
    for (i = 0; i < 100000000; i++)
        sum += i;
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) -
    (tb1.tms_utime + tb1.tms_stime));
    printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time).\n",
    (t2 - t1) / ticspersec, cpu_time / ticspersec);
}

void heapsort()