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
#include "heapsorter.hpp"
#include "record.hpp"

#define ROW_LENGTH 52

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
    // To call heapsorter: ./heapsorter.o inputFile startingRowNum numRows a|d field_order sorterId rootpid NULL
    // e.g: ./heapsorter.o Data/1batch-1000.csv 10 10 a 4 sorterId rootpid

    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);
    
    fstream inFile;
    inFile.open(argv[1], ios::in);
    if(!inFile){
        perror("[ERROR] Failed to open the file in heapsorter");
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

    heapsorter new_hs(cnt, order_field, is_desc);    
    new_hs.sort(my_record);
    
    // Sorting is finished, send the sorted results & time stats to merger send SIGUSR1 to rootpid
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
    double real_time_sec = (t2 - t1) / ticspersec;
    double cpu_time_sec = cpu_time / ticspersec;

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
    return EXIT_SUCCESS;
}