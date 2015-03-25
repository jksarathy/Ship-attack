CC=gcc
CFLAGS=-g3 -Wall -ansi -pedantic

all: testShips
field.o: field.c
ships.o: ships.c ships.h
testShips.o: testShips.c 
testShips: testShips.o ships.o field.o
	$(CC) $(CFlAGS) -o $@ $^
test: testShips
	./testShips
clean:
	$(RM) testShips *.o
	
