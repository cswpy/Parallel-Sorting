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

void heapsorter::swap(record** record_heap, int a, int b){
    record* tmp = record_heap[a];
    record_heap[a] = record_heap[b];
    record_heap[b] = tmp;
}

void heapsorter::sort(record** record_heap){
    for(int i = size/2 - 1; i >= 0; i--){
        heapify(record_heap, size, i);
    }
    // for(int i = 0; i < size; i++){
    //     record_heap[i]->print_record();
    // }


    for(int i = size - 1; i >= 0; i--){
        swap(record_heap, 0, i);
        heapify(record_heap, i, 0);
    }
}

void heapsorter::heapify(record** record_heap, int n, int record_idx){
    int largest = record_idx;
    int l = 2 * record_idx + 1;
    int r = 2 * record_idx + 2;

    if(l < n && record_heap[l]->compare_records(record_heap[largest], order_field, is_max_heap))
        largest = l;
    if(r < n && record_heap[r]->compare_records(record_heap[largest], order_field, is_max_heap))
        largest = r;
    
    if(largest != record_idx) {
        swap(record_heap, largest, record_idx);
        heapify(record_heap, n, largest);
    }
}

int main(int argc, char *argv[]){
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);
    
    fstream test_file;
    test_file.open("test.csv", ios::in);
    if(!test_file)
        cout << "File not found" << endl;

    int num_records = 20;
    record* my_record[num_records];
    int cnt = 0;
    int id, num_dependent, income, zipcode;

    string first_name, last_name;
    while(test_file >> id >> first_name >> last_name >> num_dependent >> income >> zipcode){
        my_record[cnt] = new record(id, first_name, last_name, num_dependent, income, zipcode);
        cnt++;
    }
    heapsorter new_hs(cnt, 4, true);
    cout << new_hs.order_field << endl;
    
    new_hs.sort(my_record);
    for(int i = 0; i < 20; i++){
        my_record[i]->print_record();
        delete my_record[i];
    }
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) -
    (tb1.tms_utime + tb1.tms_stime));
    printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time).\n",
    (t2 - t1) / ticspersec, cpu_time / ticspersec);
    return 0;
}