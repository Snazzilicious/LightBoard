
CC = g++
OPT = #-O3
CFLAGS = --std=c++11 $(OPT)
DBFLAGS = -g -Wall
LIBS = -lcurses

targets = test


all : $(targets)


test : main.o Screen.o Group.o
	$(CC) $(CFLAGS) main.o Screen.o Group.o $(LIBS) -o $@

go : light1.o Screen.o Group.o key_input.o dmx.o
	$(CC) $(CFLAGS) light1.o Screen.o Group.o key_input.o dmx.o $(LIBS) -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

light1.o : light1.cpp
	$(CC) $(CFLAGS) -c light1.cpp

Screen.o : Screen.cpp Screen.h
	$(CC) $(CFLAGS) -c Screen.cpp
	
Group.o : Group.cpp Group.h 
	$(CC) $(CFLAGS) -c Group.cpp

key_input.o : key_input.cpp key_input.h
	$(CC) $(CFLAGS) -c key_input.cpp

dmx.o : dmx.c dmx.h
	$(CC) $(CFLAGS) -c dmx.c

clean :
	rm -f $(targets) *.o
