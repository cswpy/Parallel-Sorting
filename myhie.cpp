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

using namespace std;

int argumentParser(int argc, char *argv[], string& input_filepath, string& output_filepath, int& num_workers, int& field_num, bool& is_ascending, bool& is_randomized){
    if(argc < 7 || argc > 12){
        cout << "Invalid arguments" << endl;
        return -1;
    }
    else{
        for(int i = 0; i < argc; i++){
            if(strcmp(argv[i], "-i") == 0)
                input_filepath = string(argv[++i]);
            else if(strcmp(argv[i], "-k") == 0)
                num_workers = atoi(argv[++i]);
            else if(strcmp(argv[i], "-r") == 0)
                is_randomized = true;
            else if(strcmp(argv[i], "-a") == 0)
                field_num = atoi(argv[++i]);
            else if(strcmp(argv[i], "-o") == 0) {
                char order = *(argv[++i]);
                if(order == 'd')
                    is_ascending = false;
                else
                    is_ascending = true;
            }
            else if(strcmp(argv[i], "-s") == 0)
                output_filepath = string(argv[++i]);
            else{
                cout << "Does not recognize the argument " << argv[i] << endl;
                return -1;
            }
            return 0;
        }
    }
    

}

int main(int argc, char *argv[]){
    string input_filepath, output_filepath;
    int num_workers, field_num;
    bool is_ascending, is_randomized = false;
    
    if(argumentParser(argc, argv, input_filepath, output_filepath, num_workers, field_num, is_ascending, is_randomized) == -1)
        return EXIT_FAILURE;
    
    

    


}