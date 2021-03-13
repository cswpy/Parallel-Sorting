#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <signal.h>

#include "merger.hpp"
#include "record.hpp"

#define ROW_LENGTH 52

using namespace std;

int main(int argc, char *argv[]){
    // Call merger using ./merger.o numWorkers a|d order_field rootPid outputFilePath
    
    // Staring the clock
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;

    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);

    // Parsing arguments
    bool is_desc = false;
    if(strcmp(argv[2], "d") == 0)
        is_desc = true;
    int numWorkers = atoi(argv[1]);
    int order_field = atoi(argv[3]);
    pid_t rootPid = atoi(argv[4]);
    string outputFilePath = string(argv[5]);
    
    string pipename;
    int fifo[numWorkers + 1];
    struct pollfd fds[numWorkers];

    // Initializing pipes and pollfds to monitor pipe statuses    
    for(int i = 0; i < numWorkers; i++){
        pipename = "tmp/sorter" + to_string(i);
        if(mkfifo(pipename.c_str(), 0777) != 0){
            perror("[ERROR] Failed to mkfifo");
            exit(1);
        }
        fifo[i] = open(pipename.c_str(), O_RDONLY | O_NONBLOCK);
        fds[i].fd = fifo[i];
        fds[i].events = POLLIN; // We only read when POLLHUP/POLLERR/POLLNVAL occurs
    }

    int ret;
    int numWorkerRead = 0;

    char content[ROW_LENGTH], length[20];
    string*** recordStringMatrix = new string**[numWorkers]; 

    int timeval = 1000;
    int num_record_sorter[numWorkers] = {};
    double real_time_sec[numWorkers] = {};
    double cpu_time_sec[numWorkers] = {};

    // Keep reading from the pipes until all pipes are read
    while(1){
        if(numWorkerRead >= numWorkers)
            break;
        ret = poll(fds, numWorkers, timeval);
        if(ret > 0) { // Some pipe is ready to be read
            for(int i = 0; i < numWorkers; i++){ // i represents the pipe/fds index
                //cout << fds[i].revents << endl;
                if((fds[i].revents & POLLIN) == POLLIN){
                    int numRead;
                    if((numRead = read(fifo[i], &num_record_sorter[i], sizeof(int))) > 0){
                        recordStringMatrix[i] = new string*[num_record_sorter[i]];
                        printf("[INFO] Going to read %d records from pipe %d\n", num_record_sorter[i], i);
                        for(int j = 0; j < num_record_sorter[i]; j++){ // j represents the index of record
                            //cout << "[INFO] Printing the " << j << "th record of sorter " << i << endl;
                            while(read(fifo[i], content, ROW_LENGTH) == -1){
                                if(errno == EAGAIN)
                                    continue;
                                perror("[ERROR] Failed to read string from pipe");
                                exit(1);
                            }
                            string *receivedString = new string;
                            *receivedString = "";
                            for(int k = 0; k < ROW_LENGTH - 1; k++)
                                *receivedString += content[k];
                            recordStringMatrix[i][j] = receivedString;
                        }
                        while(read(fifo[i], &real_time_sec[i], sizeof(double)) == -1){
                            if(errno == EAGAIN)
                                continue;
                            perror("[ERROR] Failed to read double from pipe");
                            exit(1);
                        }
                        while(read(fifo[i], &cpu_time_sec[i], sizeof(double)) == -1){
                            if(errno == EAGAIN)
                                continue;
                            perror("[ERROR] Failed to read double from pipe");
                            exit(1);
                        }
                    }
                    else if(numRead == -1){
                        perror("[ERROR] Failed to read numRecords from pipe");
                        exit(1);
                    }
                    numWorkerRead++;
                    close(fifo[i]);
                }
            }
        }
        else if(ret == -1){
            perror("[ERROR] Failed to poll");
            exit(1);
        }
    }
    
    // Printing the time statistics of each sorter using nominal time, cpu time is also available in cpu_time_sec
    int numRecords = 0;
    for(int i = 0; i < numWorkers; i++){
        printf("[STATS] Sorter %d took %lfs to sort %d records\n", i, real_time_sec[i], num_record_sorter[i]);
        numRecords += num_record_sorter[i];
    }
    cout << "[INFO] Merger now merging " << numRecords << " records in total" << endl;

    // Deleting the FIFO files
    for(int i=0; i < numWorkers; i++){
        pipename = "tmp/sorter" + to_string(i);
        unlink(pipename.c_str());
    }

    // Merging the partial sorted records into one
    
    string** recordStringFinal = new string*[numRecords + 5];
    // Copying the string addresses of first sorter to the final output
    for(int i = 0; i < num_record_sorter[0]; i++)
        recordStringFinal[i] = recordStringMatrix[0][i];
    
    int n1 = num_record_sorter[0];

    for(int z = 1; z < numWorkers; z++){
        int n2 = num_record_sorter[z];
        string** recordString1 = new string*[n1];
        for(int j = 0; j < n1; j++)
            recordString1[j] = recordStringFinal[j];        
        int x = 0;
        int y = 0;
        int k = 0;
        while(x < n1 && y < n2){
            if(record(*recordStringMatrix[z][y]).compare_records(record(*recordString1[x]), order_field, is_desc)){
                recordStringFinal[k] = recordString1[x];
                x++;
            }
            else{
                recordStringFinal[k] = recordStringMatrix[z][y];
                y++;
            }
            k++;
        }
        while(x < n1){
            recordStringFinal[k] = recordString1[x];
            x++;
            k++;
        }
        while(y < n2){
            recordStringFinal[k] = recordStringMatrix[z][y];
            y++;
            k++;
        }
        n1 += n2;
        delete [] recordString1;
    }
    
    // Writing the result into the output file
    ofstream outFile;
    outFile.open(outputFilePath, ios::in | ios::trunc);
    if(outFile.fail()){
        perror("[ERROR] Failed to open the file");
        exit(1);
    }
    for(int i = 0; i < numRecords; i++){
        outFile << *recordStringFinal[i] << endl;
    }
    cout << "[INFO] Transfered sorted data to specified path" << endl;

    // Stop the counter and print the turnaround time
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
    printf("[STATS] Merger run time was %lf sec, CPU time was %lf sec.\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);

    // Sending signal to the root node
    kill(rootPid, SIGUSR2);

    // Deallocating memory
    for(int i=0; i<numWorkers; i++){
        for(int j=0; j<num_record_sorter[i]; j++){
            delete recordStringMatrix[i][j];
        }
        delete [] recordStringMatrix[i];
    }
    delete [] recordStringMatrix;

    delete [] recordStringFinal;

    return EXIT_SUCCESS;
    /* Merger should open the file in O_NONBLOCKING so that it immediately returns if the writing end is 
    not openned yet and proceed to open the remaining fds for reading. The sorter should poll the fd first
    then if it indicates it's ready for writing, open the file in O_WRONLY mode.
    */

}