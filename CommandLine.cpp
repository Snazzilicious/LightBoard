
#include "CommandLine.h"
#include <curses.h>

#define KEY_ESC 27

CommandLine::CommandLine(){
	cursorPos = 0;
}

void CommandLine::clear(){
	line.clear();
	cursorPos = 0;
}

int CommandLine::keyPressed(int k){
	
	char key;
	
	switch (k) {
		case 99:
			key = 'C';
			break;
		case 97:
			key = 'A';
			break;
		case 45:
			key = '-';
			break;
		case 44:
			key = ',';
			break;
		case 102:
			key = 'F';
			break;
		case KEY_UP:
			key = '^';
			return 0;
		case KEY_DOWN:
			key = 'v';
			return 0;
		case KEY_LEFT:
			if (cursorPos > 0) cursorPos-- ;
			return 0;
		case KEY_RIGHT:
			if (cursorPos < line.size()) cursorPos++ ;
			return 0;
		case 49:
			key = '1';
			break;
		case 50:
			key = '2';
			break;
		case 51:
			key = '3';
			break;
		case 52:
			key = '4';
			break;
		case 53:
			key = '5';
			break;
		case 54:
			key = '6';
			break;
		case 55:
			key = '7';
			break;
		case 56:
			key = '8';
			break;
		case 57:
			key = '9';
			break;
		case 48:
			key = '0';
			break;
		case KEY_F(1):
			key = '0';
			return 0;
		case KEY_F(2):
			key = '0';
			return 0;
		case KEY_F(3):
			key = '0';
			return 0;
		case KEY_BACKSPACE:
			if (cursorPos > 0){
				cursorPos-- ;
				line.erase( line.begin()+cursorPos );
			}
			return 0;
		case KEY_ESC:
			clear();
			return 0;
		case KEY_ENTER:
			clear();
			return 0;
		default:
			return 0;
		}
	
	line.insert(line.begin()+cursorPos++, key);
	
	return 0;
}

std::string CommandLine::getString(){
	return "w.i.p";
}