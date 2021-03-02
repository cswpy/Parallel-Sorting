#include<iostream>
#include<string>
#include<fstream>
#include<sys/times.h>
#include <unistd.h>
#include <sys/types.h>

#include "heapsorter_tester.hpp"
#include "record.hpp"

class record;
class heapsorter;

using namespace std;

int main(int argc, char *argv[]){
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);
    
    // fstream test_file;
    // test_file.open("test.csv", ios::in);
    // if(!test_file)
    //     cout << "File not found" << endl;

    // int num_records = 20;
    // record* my_record[num_records];
    // int cnt = 0;
    // int id, num_dependent, income, zipcode;

    // string first_name, last_name;
    // while(test_file >> id >> first_name >> last_name >> num_dependent >> income >> zipcode){
    //     my_record[cnt] = new record(id, first_name, last_name, num_dependent, income, zipcode);
    //     cnt++;
    // }

    // heapsorter new_hs(cnt, 4, true);
    // record a(40, "Zach", "Snyder", 2, 2000, 800);
    // record b(30, "Harry", "Potter", 3, 1000, 5000);

    // cout << a.compare_records(&b,0) << endl;
    // cout << a.compare_records(&b,3);
    // cout << a.compare_records(&b,4);
    // cout << a.compare_records(&b,5);
    // cout << b.compare_records(&a,0);
    // cout << b.compare_records(&a,3);
    // cout << b.compare_records(&a,4);
    // cout << b.compare_records(&a,5);


    // cout << new_hs.order_field << endl;
    
    int new_array[10] = {1,6,2,3,0,12,9,18,5,4};
    heapsorter int_sort(10, 0, true);
    
    
    // new_hs.sort(my_record);
    int_sort.sort(new_array);
    for(int i = 0; i < 10; i++){
        cout << new_array[i];
    }
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) -
    (tb1.tms_utime + tb1.tms_stime));
    printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time).\n",
    (t2 - t1) / ticspersec, cpu_time / ticspersec);
    return EXIT_SUCCESS;
}