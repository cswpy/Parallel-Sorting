CC = g++ --std=c++11
all = mergesorter merger coord myhie
all:
	make $(all)

clean:
	rm tmp/*

mergesorter:
	$(CC) mergesorter.cpp record.cpp -o mergesorter.o

merger:
	$(CC) merger.cpp record.cpp -o merger.o

coord:
	$(CC) coord.cpp -o coord.o

myhie:
	$(CC) myhie.cpp -o myhie.o

.PHONY : clean