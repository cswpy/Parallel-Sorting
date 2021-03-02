#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>
#include "record.hpp"
#include "mergesorter.hpp"

using namespace std;

class record;

mergesorter::mergesorter(int size, int order_field, bool is_asc): size(size), order_field(order_field), is_asc(is_asc) 
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
    int k = 1;

    while(i < n1 && j < n2){
        if(L[i]->compare_records(R[j], order_field)){
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

int main(){
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
    mergesorter new_ms(cnt, 1, true);
    new_ms.sort(my_record);
    for(int i = 0; i < cnt; i++){
        my_record[i]->print_record();
        //cout << my_record[i]->income << endl;
        //delete my_record[i];
    }
}