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
#include "merger.hpp"
#include "record.hpp"

using namespace std;

int main(int argc, char *argv[]){
    // Call merger using ./merger.o numWorkers 
    int numWorkers = atoi(argv[1]);
    string pipename;
    int fifo[numWorkers + 1];
    struct pollfd fds[numWorkers];

    
    for(int i = 0; i < numWorkers; i++){
        pipename = "sorter" + to_string(i);
        fifo[i] = open(pipename.c_str(), O_RDONLY);
        fds[i].fd = fifo[i];
        fds[i].events = POLL
    }
    
    /* Merger should open the file in O_NONBLOCKING so that it immediately returns if the writing end is 
    not openned yet and proceed to open the remaining fds for reading. The sorter should poll the fd first
    then if it indicates it's ready for writing, open the file in O_WRONLY mode.
    */

}