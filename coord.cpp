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
#include <iostream>
#include <fstream>
#include "record.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    while(1){
        char fifo[6] = "pipe1";
        int fd = open(fifo, O_WRONLY);
        write(fd, "Hello papa", 11);
        close(fd);
    }
    return EXIT_SUCCESS;
}