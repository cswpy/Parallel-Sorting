# Parallel Sorting

@author: Phillip Wang

## Running the Program

Make sure that there is a tmp folder under the current folder so that FIFOs can be properly stored.
```bash
make all
./myhie.o -i <input filepath> -k <number of workers> -a <the field to sort on> -o <a|d indicating ascending|descending> -s <output filepath>
```
Optional flag:
-r indicating the random assignment of record to sort

## Design
### myhie
Myhie(also root node) will parse the input argument, examine the validity of input filepath and spawn the coord node. Before concluding, it will receive signals passed by the sorters and the merger and report them.

### coord
Coord coordinates between the root node and all other utility nodes. It parses the arguments and decide the number of lines each sorter will sort, the type of the sorter etc. Then, it spawns the merger node and sorter nodes. It will wait for all the sorters and merger to complete.

### sorter
Sorter nodes are either mergesorter or heapsorter. They only differ in the algorithms used to sort the records. Each sorter will read the designated lines from the file, sort them, and then write them to a FIFO that will be read by the merger. In addition, it will record the time elapsed and send signal to the root.

### merger
Merger node will open all the pipes in the beginning, poll the file descriptors about their status, and read in the sorted results from the pipe as well as their time stastistics. Then the merger will merge them into one sorted list and write them to the specified filepath. It will print the time for every sorter to complete and the time to merge all the records. Finally it sends a signal to the root.

### record
Record is a helper class to store the information of each sorter and has a method to compare two records based on the field to sort on and asc|desc.

## P.S.
The number of signal the root node(myhie) receives is often less than the number of sorters. This is because it is not reliable and it is not portable across platforms.