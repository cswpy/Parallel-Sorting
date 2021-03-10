#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <limits.h>
#include <cstdio>
#include <fstream>

using namespace std;

int numSorterSignal = 0;

void handleSIGUSR1(int sig){
    signal(SIGUSR1, handleSIGUSR1);
    cout << "SIGNAL RECEIVED" << endl;
    numSorterSignal++;
}

int argumentParser(int argc, char *argv[], int& input_filepath, int& output_filepath, int& num_workers, int& field_num, int& is_ascending, int& is_randomized){
    if(argc < 7 || argc > 12){
        cout << "Invalid arguments" << endl;
        return -1;
    }
    else{
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-i") == 0)
                input_filepath = ++i;
            else if(strcmp(argv[i], "-k") == 0)
                num_workers = ++i;
            else if(strcmp(argv[i], "-r") == 0)
                is_randomized = 1;
            else if(strcmp(argv[i], "-a") == 0)
                field_num = ++i;
            else if(strcmp(argv[i], "-o") == 0) {
                is_ascending = ++i;
            }
            else if(strcmp(argv[i], "-s") == 0)
                output_filepath = ++i;
            else{
                cout << "Does not recognize the argument " << argv[i] << endl;
                return -1;
            }
        }
        return 0;
    }

}

int main(int argc, char *argv[]){
    int input_filepath, output_filepath;
    int num_workers, field_num, is_ascending, is_randomized = 0;
    signal(SIGUSR1, handleSIGUSR1);
    cout << "[INFO] The root node has pid: " << getpid() << endl;

    if(argumentParser(argc, argv, input_filepath, output_filepath, num_workers, field_num, is_ascending, is_randomized) == -1)
        return EXIT_FAILURE;

    pid_t pid;
    int numOfLines;
    // char fifo[6] = "pipe1";
    // if(mkfifo(fifo, 0666) < 0){
    //     perror("Mkfifo failed.");
    //     exit(1);
    // }

    fstream finput;
    finput.open(argv[input_filepath], ios::in);
    if(finput.fail()){
        cout << "[ERROR] Input file does not exist." << endl;
        exit(1);
    }
    else{
        string tmp;
        while(getline(finput, tmp))
            numOfLines++;
        cout << "[INFO] The input file has " << numOfLines << " lines." << endl;
    }

    if((pid = fork()) == -1){
        perror("Fork failed.");
        exit(1);
    }
    else if(pid == 0){
        char const *ppid = to_string(getppid()).c_str();
        char const *num_lines = to_string(numOfLines).c_str();
        execl("./coord.o", "coord.o", argv[input_filepath], argv[num_workers], argv[field_num], argv[is_ascending], to_string(is_randomized).c_str(), argv[output_filepath], ppid, num_lines, NULL);
        perror("Exec failed.");
        exit(1);
    }
    else{
        //doing something
        cout << "Root is waiting" << endl;
        int status;
        int wpid;
        while((wpid = wait(&status)) > 0);
        cout << "[INFO] Number of SIGUSR1 arrived at root: " << numSorterSignal << endl;
        return EXIT_SUCCESS;
    }
    //./myhie.o -i Data/1batch-1000.csv -k 10 -r -a 4 -o a -s output.csv

    //./myhie.o -i test.csv -k 4 -r -a 4 -o a -s output.csv


}