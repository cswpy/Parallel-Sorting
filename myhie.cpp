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

using namespace std;

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
                is_randomized = i;
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
            return 0;
        }
    }
    

}

int main(int argc, char *argv[]){
    int input_filepath, output_filepath;
    int num_workers, field_num, is_ascending, is_randomized = 0;
    

    if(argumentParser(argc, argv, input_filepath, output_filepath, num_workers, field_num, is_ascending, is_randomized) == -1)
        return EXIT_FAILURE;

    pid_t pid;

    char fifo[6] = "pipe1";
    if(mkfifo(fifo, 0666) < 0){
        perror("Mkfifo failed.");
        exit(1);
    }

    if((pid = fork()) == -1){
        perror("Fork failed.");
        exit(1);
    }
    else if(pid == 0){
        execl("./coord.o", "coord.o", argv[input_filepath], argv[num_workers], argv[field_num], argv[is_ascending], to_string(is_randomized).c_str() ,"-h", argv[output_filepath], fifo, NULL);
        perror("Exec failed.");
        exit(1);
    }
    else{
        cout << "Waiting to read the child " << pid << "." << endl;
        while(1){
        int fd = open(fifo, O_RDONLY);
        char strbuffer[20];
        read(fd, strbuffer, 11);
        close(fd);
        cout << strbuffer << endl;
        }
        wait(NULL);
        cout << "Child process ended" << endl;
        return EXIT_SUCCESS;
    }
    //./myhie -i test.csv -k 10 -r -a 4 -o a -s output.csv 


}