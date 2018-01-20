CC = gcc
CFLAGS = -Wall -g -Iinclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11 
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

listTests:  
	$(CC)  $(CFLAGS) 

debug: 
	$(CC) $(CFLAGS) 



test: studentCode/linkedlist.c
	$(CC) $(CFLAGS)
