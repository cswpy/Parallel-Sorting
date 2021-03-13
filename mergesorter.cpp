#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include "record.hpp"
#include "mergesorter.hpp"
#define ROW_LENGTH 52

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
    // To call mergesorter: ./mergesorter.o inputFile startingRowNum numRows a|d field_order sorterId rootpid NULL
    // e.g: ./mergesorter.o Data/1batch-1000.csv 10 10 a 4 sorterId rootpid
    //cout << "Filepath: " << argv[1] <<" startingRowNum: " << argv[2] << " numRowsToSort: " << argv[3] << " Asc/Desc: " << argv[4] << " order_field: " << argv[5] << " fdNum: " << argv[6] << " rootpid: " << argv[7] << endl;
    
    // Starting the tie counter
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);
    
    fstream inFile;
    inFile.open(argv[1], ios::in);
    if(!inFile){
        perror("[ERROR] Failed to open the file in mergesorter");
        exit(1);
    }
    int num_records = atoi(argv[3]);
    int sorterId = atoi(argv[6]);
    int rootpid = atoi(argv[7]);

    record* my_record[num_records];
    
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
        getline(inFile, tmp);
        my_record[cnt] = new record(tmp);
        cnt++;
    }
    bool is_desc = false;
    if(strcmp(argv[4], "d") == 0)
        is_desc = true;
    int order_field = atoi(argv[5]);
    
    mergesorter new_ms(cnt, order_field, is_desc);
    new_ms.sort(my_record);
    
    // Sorting is finished, send the sorted results & time stats to merger send SIGUSR1 to rootpid
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
    double real_time_sec = (t2 - t1) / ticspersec;
    double cpu_time_sec = cpu_time / ticspersec;

    //cout << "[INFO] Sorter " << sorterId << " took " << real_time_sec << " to complete." << endl;

    string pipename = "tmp/sorter" + to_string(sorterId);

    
    int fd;
        
    while((fd = open(pipename.c_str(), O_WRONLY)) == -1){
        if(errno != ENOENT){
            perror("[ERROR] Failed to open fifo.");
            exit(1);
            break;
        }
    }
    //const char *numRecordsChar = to_string(num_records).c_str();
    write(fd, &num_records, sizeof(int));

    for(int i = 0; i < cnt; i++){
        char buf[ROW_LENGTH];
        strcpy(buf, my_record[i]->original_string.c_str());
        write(fd, buf, ROW_LENGTH);
        delete my_record[i];
    }

    // Transmitting the time stats to merger
    write(fd, &real_time_sec, sizeof(double));
    write(fd, &cpu_time_sec, sizeof(double));
    close(fd);

    kill(rootpid, SIGUSR1);
    cout << "Send SIGUSR1 to " << rootpid << endl;
    return 0; 
}