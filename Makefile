CC = gcc
CFLAGS = -Wall -g -Iinclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11 
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

debug: src/linkedlist.c src/GEDCOMparser.c src/test.c
	$(CC) $(CFLAGS)  src/linkedlist.c src/GEDCOMparser.c src/test.c -o bin/testMe -g
	gdb --args ./bin/testMe testdata.ged

test: src/linkedlist.c src/GEDCOMparser.c src/test.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c -o bin/testMe
	./bin/testMe testdata.ged
