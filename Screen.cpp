
#include "Screen.h"
#include <curses.h>
#include <cmath>


Screen::Screen(int numFaders){
	nFaders = numFaders;
	
	nRows = (int) std::sqrt( MAX_CHANNELS );
	nCols = (MAX_CHANNELS-1) / nRows + 1;
	
	//curses initialization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr,true);
	
	mvprintw(0, 0, "%s", "F2: CLEAR INPUT  F3: SAVE AS CUE  F4: EXIT");
	
	int x,y;
	//prints channel labels
	attron(A_UNDERLINE);
	for (int i=0; i<MAX_CHANNELS; i++){
		x = (i % nCols)*H_SPAC + CHAN_START_X;
		y = (i / nCols)*V_SPAC + CHAN_START_Y;
		
		mvprintw( y, x, "%d", i+1 );
	}
	
	y += H_SPAC;
	
	//prints each of the faders on the control board being used
	for (int i=0; i<nFaders; i++){
		x = i*H_SPAC + 2;
		
		mvprintw(y, x, "F%d", i + 1);
	}
	attroff(A_UNDERLINE);
	
	y += H_SPAC;
	
	//prints message
	mvprintw(y, 2, "Program Initialized");
	
	y += 1;
	move(y, 2);
	
	//refresh();
}



void Screen::update(int Percents[], int chanIn[], Group* cuesOnFaders[], int faderLoad, int faderPercents[], char* msg, CommandLine cmd){
	int x,y;
	
	// clear old values
	for (int i=0; i<nRows; i++){
		y = i*V_SPAC + CHAN_START_Y + 1;
		move(y,0);
		clrtoeol();
	}
	
	// Print Channel Percents
	for (int i=0; i<MAX_CHANNELS; i++){
		x = (i % nCols)*H_SPAC + CHAN_START_X;
		y = (i / nCols)*V_SPAC + CHAN_START_Y + 1;
		
		if (chanIn[i]) attron(A_STANDOUT);
		mvprintw( y, x, "%d", Percents[i] );
		attroff(A_STANDOUT);
	}
	
	y += H_SPAC;
	
	// Print cuesOnFaders
	move(y,0);
	clrtoeol();
	for (int i=0; i<nFaders; i++){
		x = i*H_SPAC + 2;
		
		if (i == faderLoad) attron(A_STANDOUT);
		if (cuesOnFaders[i] != nullptr){
			mvprintw(y, x, "%d", cuesOnFaders[i]->getName());
		}
		else {
			mvprintw(y, x, "__");
		}
		attroff(A_STANDOUT);
	}
	
	y+=1;
	
	//prints the percentage of each fader
	move(y, 0);
	clrtoeol();
	for (int i=0; i<nFaders; i++){
		x = i*H_SPAC + 2;
		
		mvprintw(y, x, "%d", faderPercents[i]);
	}
	
	y += H_SPAC-2;
	
	//prints message
	move(y, 0);
	clrtoeol();
	mvprintw(y, 2, msg);
	
	y += 1;
	
	//prints the entered command
	move(y,0);
	clrtoeol();
	x=2;
	for (std::vector<char>::iterator it=cmd.lineBegin(); it != cmd.lineEnd(); it++){
		mvprintw(y, x, "%c", *it);
		x++;
	}
	move(y,cmd.getCursorPos()+2);
	
	//refresh();
}




