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

class record;

int main(int argc, char const *argv[]){
    string input_path, output_path;
    int num_workers;

    fstream file;
    file.open("test.csv", ios::in);
    file.seekg(0, ios::end);
    int end = file.tellg();
    cout << end << endl;
    cout << sizeof(record) << endl;
    cout << (end / sizeof(record)) << endl;

    FILE *finput = fopen("test.csv", "r");
    fseek(finput, 0, SEEK_END);
	int lSize = ftell(finput);
	rewind(finput);
    cout << lSize << endl;
	int numOfRecords = (int) lSize/sizeof(record);
    cout << numOfRecords;
    return EXIT_SUCCESS;
}