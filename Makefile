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
	gdb --args ./bin/testMe shakespeare.ged core

test: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	./bin/testMe testdata.ged

test2: src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c
	$(CC) $(CFLAGS) src/linkedlist.c src/GEDCOMparser.c src/test.c src/GEDCOMutilities.c -o bin/testMe
	./bin/testMe shakespeare.ged
