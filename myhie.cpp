#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <limits.h>

using namespace std;

int argumentParser(int argc, char *argv[], string& input_filepath, string& output_filepath, int& num_workers, int& field_num, bool& is_ascending, bool& is_randomized){
    if(argc < 7 || argc > 14){
        cout << "Invalid arguments" << endl;
        return -1;
    }
}

int main(int argc, char *argv[]){
    string input_filepath, output_filepath;
    int num_workers, field_num;
    bool is_ascending, is_randomized;


    


}