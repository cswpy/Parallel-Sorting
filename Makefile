CC = g++ --std=c++11
all = mergesorter heapsorter merger coord myhie
all:
	make $(all)

clean:
	rm tmp/*

mergesorter: mergesorter.cpp record.cpp
	$(CC) mergesorter.cpp record.cpp -o mergesorter.o

heapsorter: heapsorter.cpp record.cpp
	$(CC) heapsorter.cpp record.cpp -o heapsorter.o

merger: merger.cpp record.cpp
	$(CC) merger.cpp record.cpp -o merger.o

coord: coord.cpp
	$(CC) coord.cpp -o coord.o

myhie: myhie.cpp
	$(CC) myhie.cpp -o myhie.o

.PHONY : clean