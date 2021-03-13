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
#include "merger.hpp"
#include "record.hpp"

#define ROW_LENGTH 52

using namespace std;

int main(int argc, char *argv[]){
    // Call merger using ./merger.o numWorkers a|d order_field rootPid
    bool is_desc = false;
    if(strcmp(argv[2], "d") == 0)
        is_desc = true;
    
    int order_field = atoi(argv[3]);
    pid_t rootPid = atoi(argv[4]);
    
    
    cout << "[INFO] This is merger node, received numWorkers: " << argv[1] << endl;


    int numWorkers = atoi(argv[1]);

    string pipename;
    int fifo[numWorkers + 1];
    struct pollfd fds[numWorkers];

    
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
    while(1){
        if(numWorkerRead >= numWorkers)
            break;
        ret = poll(fds, numWorkers, timeval);
        if(ret > 0) { // Some pipe is ready to be read
            for(int i = 0; i < numWorkers; i++){ // i represents the pipe/fds index
                //cout << fds[i].revents << endl;
                if((fds[i].revents & POLLIN) == POLLIN){
                    int numRead;
                    if((numRead = read(fifo[i], &num_record_sorter[i], sizeof(int))) != 0){
                        recordStringMatrix[i] = new string*[num_record_sorter[i]];
                        printf("[INFO] Going to read %d records from pipe.\n", num_record_sorter[i]);
                        for(int j = 0; j < num_record_sorter[i]; j++){ // j represents the index of record
                            //cout << "[INFO] Printing the " << j << "th record of sorter " << i << endl;
                            read(fifo[i], content, ROW_LENGTH);
                            string *receivedString = new string;
                            *receivedString = "";
                            for(int k=0; k < ROW_LENGTH; k++)
                                *receivedString += content[k];
                            recordStringMatrix[i][j] = receivedString;
                        }
                    }
                    numWorkerRead++;
                    close(fifo[i]);
                }
            }
          }
    }
    int numRecords = 0;
    for(int i = 0; i < numWorkers; i++){
        numRecords += num_record_sorter[i];
    }
    cout << "[INFO] Merger now merging " << numRecords << " records in total" << endl;

    // for(int i=0; i < numWorkers; i++){
    //     cout << "-----Sorter " << i << "-----" << endl;
    //     for(int j=0; j < num_record_sorter[i]; j++)
    //         cout << *(recordStringMatrix[i][j]) << endl;
    // }

    for(int i=0; i < numWorkers; i++){
        pipename = "tmp/sorter" + to_string(i);
        unlink(pipename.c_str());
    }

    // Merging the partial sorted records into one
    string** recordStringFinal = new string*[numRecords+5];
    // Copying the string addresses of first sorter to the final output
    for(int i = 0; i < num_record_sorter[0]; i++)
        recordStringFinal[i] = recordStringMatrix[0][i];
    

    int n1 = num_record_sorter[0];
    // cout << "Number of workers: "<<numWorkers<<endl;
    cout << "[INFO] Merging at order_field: " << order_field << " is_desc: " << is_desc << endl;
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
    
    for(int i=0; i < numRecords; i++)
        cout << *recordStringFinal[i] << endl;

    // Deallocating memory
    for(int i=0; i<numWorkers; i++){
        for(int j=0; j<num_record_sorter[i]; j++){
            delete recordStringMatrix[i][j];
        }
        delete [] recordStringMatrix[i];
    }
    delete [] recordStringMatrix;

    delete [] recordStringFinal;

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