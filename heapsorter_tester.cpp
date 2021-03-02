#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>

#include "record.hpp"
#include "heapsorter_tester.hpp"

using namespace std;

class record;


heapsorter::heapsorter(int size, int order_field, bool is_max_heap): size(size), order_field(order_field), is_max_heap(is_max_heap)
{}

void heapsorter::swap(int* record_heap, int a, int b){
    int tmp = record_heap[a];
    record_heap[a] = record_heap[b];
    record_heap[b] = tmp;
}

void heapsorter::sort(int* record_heap){
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

void heapsorter::heapify(int* record_heap, int n, int record_idx){
    int largest = record_idx;
    int l = 2 * record_idx + 1;
    int r = 2 * record_idx + 2;

    if(l < n && record_heap[l] > record_heap[largest])
        largest = l;
    else if(r < n && (record_heap[r] > record_heap[largest]))
        largest = r;
    
    if(largest != record_idx) {
        swap(record_heap, largest, record_idx);
        //cout << "Swapped " << largest << " " << record_idx;
        heapify(record_heap, n, largest);
    }
}