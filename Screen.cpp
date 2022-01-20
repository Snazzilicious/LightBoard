
#include "Screen.h"
#include <iostream>

using namespace std;

int initialize_screen(int numOfFaders) {
	initscr();
	noecho();

	int x,y;
	
	mvprintw(0, 0, "%s", "F1: CLEAR INPUT  F2: SAVE AS CUE  F3: EXIT");
	
	//prints channel labels
	attron(A_UNDERLINE);
	for (y=0; y<(MAX_CHANNELS / 2); y+=3) {
		
		for (x=4; x<=24; x+=4){
			mvprintw(y+2, x, "%d", (2*y) + (x/4));
		}
			
	}
	
	
	y+=3;
	//prints each of the faders on the control board being used
	for (x=2; x < 2+(numOfFaders*4); x+=4){
		mvprintw(y, x, "F%d", ((x-2)/4) + 1);
		
	}
	attroff(A_UNDERLINE);
	
	y+=5; //prints message
	mvprintw(y, 2, "Program Initialized");
	
	
	
	refresh();
	
	return y+2;
}





//prints the percentages of all the channels
//still needs the channels to be numbered
void print_screen(vector<Group*> cuesOnFaders, int Percents[], char messageToUser[], Command commandLine, int &endline, int faderPercents[], int chanIn[], int Load){
	
	int x,y,i;
	Group temporary;
	
	//prints the percentages of all the channels
	//that were sent to the dimmer
	for (y=1; y<(MAX_CHANNELS / 2)+1; y+=3){
		move(y+2, 0);
		clrtoeol();
		for (x=4; x<=24; x+=4){
			i=2*(y-1) + (x/4)-1;
			
			//highlights if the keyboard has input something
			if(chanIn[i]){
				attron(A_STANDOUT);
				mvprintw(y+2, x, "%d", Percents[ i ]);
				attroff(A_STANDOUT);
			}
			else{
				mvprintw(y+2, x, "%d", Percents[ i ]);
			}
		
		}
	
	}
	
	y+=3;
	move(y, 0);
	clrtoeol();
	//prints fader assignments
	for (x=2, i=0; x < 2+(cuesOnFaders.size()*4); x+=4, i++){
		
		if(i == Load){
			attron(A_STANDOUT);
		}
		
		if (cuesOnFaders[i] != NULL){
			temporary = *cuesOnFaders[i];
			mvprintw(y, x, "%d", temporary.name);
		}
		else {
			mvprintw(y, x, " ");
		}
		
		attroff(A_STANDOUT);
	}
	
	//prints the percentage the fader is at currently
	y+=2;
	move(y, 0);
	clrtoeol();
	for (x=2, i=0; i<cuesOnFaders.size(); x+=4, i++){
		
		mvprintw(y, x, "%d", faderPercents[i]);
		
	}
	
	
	
	//prints message to the user
	move(endline-2, 2);
	clrtoeol();
	mvprintw(endline-2, 2, "%s", messageToUser);
	
	if (commandLine.converted.size()<1){
		move(endline, 2);
		clrtoeol();
	}
		
	//prints the entered command
	for (int i=0; i<commandLine.converted.size(); i++){
			mvprintw(endline, 2+i, "%c", commandLine.converted[i]);
	}
	clrtoeol();
	
	
	
	
	refresh();
}

