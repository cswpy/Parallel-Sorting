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

int main(int argc, char *argv[]){
    string input_path, output_path;
    int num_workers, num_records;

    // Initialize an array to store the number of records to be sorted by each sorter
    num_workers = atoi(argv[2]);
    num_records = atoi(argv[8]); //The 8th argument specifies the number of lines in the file
    int num_for_sorter[num_workers];

    // cout << num_workers << num_records << endl;
    cout << "This is child process" << endl;

    // Calculate the number of records to sort for each sorter
    int num_records_remain = num_records;
    if(strcmp(argv[5], "1") == 0){
        srand(time(NULL));
        for(int i = 0; i < num_workers - 1; i++){
            num_for_sorter[i] = rand() % (num_records_remain - num_workers + i) + 1;
            num_records_remain -= num_for_sorter[i];
        }
        num_for_sorter[num_workers - 1] = num_records_remain;
    }
    else{
        int num_records_avg;
        num_records_avg = num_records_remain / num_workers;
        for(int i = 0; i < num_workers - 1; i++){
            num_for_sorter[i] = num_records_avg;
            num_records_remain -= num_records_avg;
        }
        num_for_sorter[num_workers - 1] = num_records_remain;
    }
    


    // for(int i = 0; i < num_workers; i++)
    //     cout << num_for_sorter[i] << " ";

    //int fifo[num_workers + 1];
    int startingRowNum = 0;
    string pipename;
    for(int i = 0; i < num_workers; i++){
        pipename = "sorter" + to_string(i);
        // if(fifo[i] = mkfifo(pipename.c_str(), 0666) != 0){
        //     perror("[ERROR] Failed to mkfifo.");
        //     exit(1);
        // }
        pid_t pid;
        if((pid = fork()) == -1){
            perror("[ERROR] Failed to fork in coord.");
            exit(1);
        }
        // Calling the appropriate sorter if child
        else if(pid == 0) {
            char const *startingRowNumChar = to_string(startingRowNum).c_str();
            char const *numRowsToSortChar = to_string(num_for_sorter[i]).c_str();
            char const *sorterIdChar = to_string(i).c_str();
            execl("./mergesorter.o", "mergesorter.o", argv[1], startingRowNumChar, numRowsToSortChar, argv[4], argv[3], sorterIdChar, argv[7], NULL);
            perror("Failed to exec in coord.");
            exit(1);
        }
        else{
            // Parent process
            startingRowNum += num_for_sorter[i];
        }
    }

    // Spawning the merger node
    pid_t mergerpid;
    if((mergerpid = fork()) == -1){
        perror("[ERROR] Failed to fork in coord.");
        exit(1);
    }
    else if(mergerpid == 0){
        char const *numWorkersChar = to_string(num_workers).c_str();
        execl("./merger.o", "merger.o", numWorkersChar, NULL);
        perror("Failed to exec in coord.");
        exit(1);
    }
    else{
        pid_t wpid;
        int status;
        while((wpid = wait(&status)) > 0);

        // for(int i=0; i < num_workers; i++){
        //     pipename = "sorter" + to_string(i);
        //     unlink(pipename.c_str());
        // }

        return EXIT_SUCCESS;
    }
}