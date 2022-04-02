
#include "CommandLine.h"
#include <curses.h>

// #define KEY_ESC 27
#define KEY_C_AS_ESC 99
#define KEY_RETURN 10

CommandLine::CommandLine(){
	cursorPos = 0;
	wrkSpace.push_front(std::vector<char>());
	line = wrkSpace.begin();
}

void CommandLine::clear(){
	line->clear();
	cursorPos = 0;
}

void CommandLine::resetWrkSpace(){
	wrkSpace = hist;
	wrkSpace.push_front(std::vector<char>());
	line = wrkSpace.begin();
	cursorPos = 0;
}

int CommandLine::keyPressed(int k){

	char key;
	
	switch (k) {
//		case 99:
//			key = 'C';
//			break;
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
//		case 110:
//			key = 'N';
//			break;
//		case 121:
//			key = 'Y';
//			break;
		case KEY_UP:
			line++;
			if (line != wrkSpace.end()){
				cursorPos = line->size();
			} else {
				line--;
			}
			return NONE;
		case KEY_DOWN:
			if (line != wrkSpace.begin()){
				line--;
				cursorPos = line->size();
			}
			return NONE;
		case KEY_LEFT:
			if (cursorPos > 0) cursorPos-- ;
			return NONE;
		case KEY_RIGHT:
			if (cursorPos < line->size()) cursorPos++ ;
			return NONE;
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
		case KEY_F(2):
			key = '0';
			return F2_PRESSED;
		case KEY_F(3):
			key = '0';
			return F3_PRESSED;
		case KEY_F(4):
			key = '0';
			return F4_PRESSED;
		case KEY_BACKSPACE:
			if (cursorPos > 0){
				cursorPos-- ;
				line->erase( line->begin()+cursorPos );
			}
			return NONE;
//		case KEY_ESC: THIS COMES WITH A ~1 sec timer if using keypad()
//			resetWrkSpace();
//			return 0;
		case KEY_C_AS_ESC:
			resetWrkSpace();
			return NONE;
		case KEY_RETURN:
			lastCmd = std::string( line->data(), line->size() );

			hist.push_front(*line);
			if (hist.size() > MAX_HIST_LEN) hist.pop_back();
			resetWrkSpace();
			
			return ENTER_PRESSED;
		default:
			return NONE;
		}
	
	line->insert(line->begin()+cursorPos++, key);
	
	return NONE;
}


std::vector<char>::iterator CommandLine::lineBegin(){
	return line->begin();
}
std::vector<char>::iterator CommandLine::lineEnd(){
	return line->end();
}

int CommandLine::getCursorPos(){
	return cursorPos;
}


std::string CommandLine::getLastCmd(){
	return lastCmd;
}
