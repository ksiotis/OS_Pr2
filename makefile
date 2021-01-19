#old makefile, to be changed
CC=g++
CFLAGS=-std=c++11 -Wall -g3

main: builder memoryentry.o hashentry.o ./include/list.hpp ./include/hashtable.hpp
	$(CC) $(CFLAGS) main.cpp ./builder/*.o ./include/list.hpp ./include/hashtable.hpp -o emulation

memoryentry.o: builder ./include/memoryentry.cpp ./include/memoryentry.hpp
	$(CC) $(CFLAGS) -c ./include/memoryentry.cpp -o ./builder/memoryentry.o

hashentry.o: builder ./include/hashentry.cpp ./include/hashentry.hpp
	$(CC) $(CFLAGS) -c ./include/hashentry.cpp -o ./builder/hashentry.o

builder: 
	mkdir -p ./builder

clean:
	rm -df emulation ./builder/memoryentry.o ./builder/hashentry.o ./builder
