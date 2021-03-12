CC = g++ --std=c++11

.PHONY : clean

clean:
	rm tmp/*

mergesorter:
	$(CC) mergesorter.cpp record.cpp -o mergesorter.o

merger:
	$(CC) merger.cpp -o merger.o

coord:
	$(CC) coord.cpp -o coord.o

myhie:
	$(CC) myhie.cpp -o myhie.o