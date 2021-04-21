# quick and dirty makefile that needs to be better prepared for cross-platform stuff
CC = gcc
CFLAGS += -ansi -O3 -pedantic -Wall

.PHONY: all clean

all: romwak

romwak: romwak.o

clean:
	rm -f *.o *.obj
