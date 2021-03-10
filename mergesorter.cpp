#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>
#include "record.hpp"
#include "mergesorter.hpp"

using namespace std;

class record;

mergesorter::mergesorter(int size, int order_field, bool is_desc): size(size), order_field(order_field), is_desc(is_desc) 
{}

void mergesorter::sort(record** record_array) {
    mergeSort(record_array, 0, size-1);
}

void mergesorter::mergeSort(record** record_array, int left, int right) {
    if(left >= right)
        return;
    int middle = left + (right - left)/2;
    mergeSort(record_array, left, middle);
    mergeSort(record_array, middle+1, right);
    merge(record_array, left, middle, right);
}

void mergesorter::merge(record** record_array, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    record* L[n1];
    record* R[n2];

    for(int i = 0; i < n1; i++)
        L[i] = record_array[left + i];
    for(int j = 0; j < n2; j++)
        R[j] = record_array[middle + 1 + j];
    
    int i = 0;
    int j = 0;
    int k = left;

    while(i < n1 && j < n2){
        if(R[j]->compare_records(L[i], order_field, is_desc)){
            record_array[k] = L[i];
            i++;
        }
        else {
            record_array[k] = R[j];
            j++;
        }
        k++;
    }

    while(i < n1){
        record_array[k] = L[i];
        i++;
        k++;
    }
    while(j < n2){
        record_array[k] = R[j];
        j++;
        k++;
    }
}

int main(int argc, char *argv[]){
    // To call mergesorter: ./mergesorter.o inputFile startingRowNum numRows a|d field_order fdNum NULL
    // e.g: ./mergesorter.o test.csv 10 10 a 4 1 

    fstream inFile;
    inFile.open(argv[1], ios::in);
    if(!inFile){
        cout << "[ERROR] Failed to open the file in mergesorter." << endl;
        exit(1);
    }
    int num_records = atoi(argv[3]);

    record* my_record[num_records];
    int id, num_dependent, zipcode;
    float income;

    string first_name, last_name;
    
    int lineCnt = 1;
    int numRows = atoi(argv[3]);
    int startRow = atoi(argv[2]);
    string tmp;

    while(lineCnt < startRow){
        getline(inFile, tmp);
        lineCnt++;
    }
    int cnt = 0;
    while(cnt < numRows){
        inFile >> id >> first_name >> last_name >> num_dependent >> income >> zipcode;
        my_record[cnt] = new record(id, first_name, last_name, num_dependent, income, zipcode);
        cnt++;
    }
    bool is_desc = false;
    if(strcmp(argv[4], "d") == 0)
        is_desc = true;
    int order_field = atoi(argv[5]);
    
    mergesorter new_ms(cnt, order_field, is_desc);
    new_ms.sort(my_record);
    
    int fd = atoi(argv[6]);
    for(int i = 0; i < cnt; i++){
        my_record[i]->print_record();
        delete my_record[i];
    }
    

    return 0;
}