#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "record.hpp"

using namespace std;

class record;

int main(int argc, char const *argv[]){
    string input_path, output_path;
    int num_workers, num_records;

    // Initialize an array to store the number of records to be sorted by each sorter
    num_workers = atoi(argv[2]);
    num_records = atoi(argv[8]);
    int num_for_sorter[num_workers];

    // If the random flag is received
    if(strcmp(argv[5], "1")){
        srand(time(NULL));
        int num_records_remain = num_records;
        for(int i = 0; i < num_workers - 1; i++){
            num_for_sorter[i] = rand() % (num_records_remain - num_workers + i + i) + 1;
            num_records_remain -= num_for_sorter[i];
        }
        num_for_sorter[num_workers - 1] = num_records_remain;
    }
    
    for(int i = 0; i < num_workers - 1; i++)
        cout << num_for_sorter[i] << " ";


    return EXIT_SUCCESS;
}