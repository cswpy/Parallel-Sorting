#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include "merger.hpp"
#include "record.hpp"

#define ROW_LENGTH 52

using namespace std;

int main(int argc, char *argv[]){
    // Call merger using ./merger.o numWorkers 
    cout << "This is merger node, received numWorkers: " << argv[1] << endl;

    int numWorkers = atoi(argv[1]);

    string pipename;
    int fifo[numWorkers + 1];
    struct pollfd fds[numWorkers];

    
    for(int i = 0; i < numWorkers; i++){
        pipename = "tmp/sorter" + to_string(i);
        if(mkfifo(pipename.c_str(), 0777) != 0){
            perror("[ERROR] Failed to mkfifo.");
            exit(1);
        }
        fifo[i] = open(pipename.c_str(), O_RDONLY | O_NONBLOCK);
        fds[i].fd = fifo[i];
        fds[i].events = POLLIN; // We only read when POLLHUP/POLLERR/POLLNVAL occurs
    }

    int ret;
    int numSorterRead = 0;

    char content[ROW_LENGTH+1], length[20];
    string* recordStringPtr[numWorkers];
    int 

    int timeval = 1000;
    while(1){
        if(numSorterRead >= numWorkers)
            break;
        ret = poll(fds, numWorkers, timeval);
        if(ret > 0) { // Some pipe is ready to be read
            cout << "Detected available pipe: " << ret << endl;
            numSorterRead += ret;
            for(int i = 0; i < numWorkers; i++){
                //cout << fds[i].revents << endl;
                if((fds[i].revents&POLLIN) == POLLIN){
                    int numRead, numRecord;
                    if((numRead = read(fifo[i], &numRecord, sizeof(int))) != 0){
                        string recordString[numRecord];
                        printf("[INFO] Going to read %d records from pipe.\n", numRecord);
                        for(int j = 0; j < numRecord; j++){
                            read(fifo[i], content, ROW_LENGTH);
                            recordString[j] = string(content);
                        }
                        recordStringPtr[i] = recordString;
                    }
                    close(fifo[i]);
                }
            }
          }
    }
    

    for(int i=0; i < numWorkers; i++){
        pipename = "tmp/sorter" + to_string(i);
        unlink(pipename.c_str());
    }

    // for (int i=0; i < numWorkers; i++){
    //     ifstream inFile;
    //     string pipename = "sorter" + to_string(i);
    //     inFile.open(pipename.c_str(), ios::in);
    //     string line;
    //     inFile >> line;
    //     cout << line;
    //     close(fifo[i]);
    //     pipename = "sorter" + to_string(i);
    //     unlink(pipename.c_str());
    // }
    return EXIT_SUCCESS;
    /* Merger should open the file in O_NONBLOCKING so that it immediately returns if the writing end is 
    not openned yet and proceed to open the remaining fds for reading. The sorter should poll the fd first
    then if it indicates it's ready for writing, open the file in O_WRONLY mode.
    */

}