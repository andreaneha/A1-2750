CC = gcc
CFLAGS = -Wall -g -Iinclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11 
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

debug: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS)  src/linkedlist.c src/GEDCOMparser.c src/test.c  src/GEDCOMutilities.c -o bin/testMe -g
	gdb --args ./bin/testMe testdata.ged core

test: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	./bin/testMe testdata.ged

test2: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	./bin/testMe shakespeare.ged

val: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	valgrind --leak-check=full --show-leak-kinds=all ./bin/testMe shakespeare.ged 

val2: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	valgrind --leak-check=full --show-leak-kinds=all ./bin/testMe testdata.ged 
