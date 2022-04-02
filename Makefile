
CC = g++
OPT = #-O3
DBFLAGS = -g -Wall
CFLAGS = --std=c++11 $(OPT) #$(DBFLAGS)
LIBS = -lcurses

targets = test


all : $(targets)


test : main.o Screen.o Group.o CommandLine.o parseCommands.o
	$(CC) $(CFLAGS) main.o Screen.o Group.o CommandLine.o parseCommands.o $(LIBS) -o $@

go : light1.o Screen.o Group.o CommandLine.o dmx.o
	$(CC) $(CFLAGS) light1.o Screen.o Group.o CommandLine.o dmx.o $(LIBS) -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

light1.o : light1.cpp
	$(CC) $(CFLAGS) -c light1.cpp

Screen.o : Screen.cpp Screen.h Group.h CommandLine.h
	$(CC) $(CFLAGS) -c Screen.cpp

Group.o : Group.cpp Group.h
	$(CC) $(CFLAGS) -c Group.cpp

CommandLine.o : CommandLine.cpp CommandLine.h
	$(CC) $(CFLAGS) -c CommandLine.cpp

parseCommands.o : parseCommands.cpp parseCommands.h
	$(CC) $(CFLAGS) -c parseCommands.cpp

dmx.o : dmx.c dmx.h
	$(CC) $(CFLAGS) -c dmx.c

clean :
	rm -f $(targets) *.o
