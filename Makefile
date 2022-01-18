
CC = g++
OPT = #-O3
CFLAGS = --std=c++11 $(OPT)
DBFLAGS = -g -Wall
LIBS = -lcurses

targets = go


all : $(targets)


go : light1.o Screen.o lightStuff.o key_input.o dmx.o
	$(CC) $(CFLAGS) light1.o Screen.o lightStuff.o key_input.o dmx.o $(LIBS) -o $@

light1.0 : light1.cpp
	$(CC) $(CFLAGS) -c light1.cpp

Screen.o : Screen.cpp Screen.h
	$(CC) $(CFLAGS) -c Screen.cpp
	
lightStuff.o : lightStuff.cpp lightStuff.h 
	$(CC) $(CFLAGS) -c lightStuff.cpp

key_input.o : key_input.cpp key_input.h
	$(CC) $(CFLAGS) -c key_input.cpp

dmx.o : dmx.c dmx.h
	$(CC) $(CFLAGS) -c dmx.c

clean :
	rm -f $(targets) *.o
