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

#define LENGTH 200

using namespace std;

int main(int argc, char *argv[]){
    // Call merger using ./merger.o numWorkers 
    cout << "This is merger node, received numWorkers: " << argv[1] << endl;

    int numWorkers = atoi(argv[1]);

    string pipename;
    int fifo[numWorkers + 1];
    struct pollfd fds[numWorkers];

    
    for(int i = 0; i < numWorkers; i++){
        pipename = "sorter" + to_string(i);
        fifo[i] = open(pipename.c_str(), O_RDONLY);
        cout << "Opened file." << endl;
        fds[i].fd = fifo[i];
        fds[i].events = POLLIN; // We only read when POLLHUP/POLLERR/POLLNVAL occurs
    }

    int ret;
    int numSorterRead = 0;

    char *content[LENGTH], *length[20];


    // while(1){
    //     cout << numSorterRead << " " << numWorkers << endl;
        
    //     if(numSorterRead >= numWorkers)
    //         break;
    //     cout << "Test" << endl;
    //     // ret = poll(fds, numWorkers, -1);
    //     ret = 1; 
    //     cout << "Detected number of files ready to read: " << ret << endl;
    //     if(ret > 0) { // Some pipe is ready to be read
    //         for(int i=0; i < numWorkers; i++){
    //             cout << "t1" << endl;
    //             //if(fds[i].revents & POLLIN){
    //                 cout << "t2" << endl;
    //                 read(fifo[i], content, 13);
    //                 cout << *content << endl;
    //             //}
    //         }
    //       }
    // }



    for (int i=0; i < numWorkers; i++){
        ifstream inFile;
        string pipename = "sorter" + to_string(i);
        inFile.open(pipename.c_str(), ios::in);
        string line;
        inFile >> line;
        cout << line;
        close(fifo[i]);
        pipename = "sorter" + to_string(i);
        unlink(pipename.c_str());
    }
    return EXIT_SUCCESS;
    /* Merger should open the file in O_NONBLOCKING so that it immediately returns if the writing end is 
    not openned yet and proceed to open the remaining fds for reading. The sorter should poll the fd first
    then if it indicates it's ready for writing, open the file in O_WRONLY mode.
    */

}