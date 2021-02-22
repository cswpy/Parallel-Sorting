#ifndef RECORD_H
#define RECORD_H

#include <string.h>

typedef struct Record;

int init_record(*Record, char*);
int compare_records(Record, Record);
void print_record(Record);
void write_record(Record, FILE);
int delete_record(*Record);

