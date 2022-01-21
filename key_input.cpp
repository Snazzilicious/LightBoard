
#include "key_input.h"
#include <ctype.h>
#include <curses.h>
#include <linux/input.h>

void Command::add(int newElem){
	converted.push_back( convert(newElem) );
}


void Command::backspace(void) {
	if(converted.size()>=1) converted.pop_back();
}

void Command::clear(void) {
	converted.clear();
}

char Command::convert(int input){
	char key;
	
	
	switch (input) {
		case KEY_C:
			key = 'C';
			break;
		case KEY_A:
			key = 'A';
			break;
		case KEY_MINUS:
			key = '-';
			break;
		case KEY_COMMA:
			key = ',';
			break;
		case KEY_F:
			key = 'F';
			break;
		case KEY_UP:
			key = '^';
			break;
		case KEY_DOWN:
			key = 'v';
			break;
		case KEY_LEFT:
			key = '<';
			break;
		case KEY_RIGHT:
			key = '>';
			break;
		case KEY_1:
			key = '1';
			break;
		case KEY_2:
			key = '2';
			break;
		case KEY_3:
			key = '3';
			break;
		case KEY_4:
			key = '4';
			break;
		case KEY_5:
			key = '5';
			break;
		case KEY_6:
			key = '6';
			break;
		case KEY_7:
			key = '7';
			break;
		case KEY_8:
			key = '8';
			break;
		case KEY_9:
			key = '9';
			break;
		case KEY_0:
			key = '0';
			break;
		default:
			key = '#';
		}
	
	
	return key;
}






int interpret_command (Command line, std::vector<Group> cueList, int percents[], int flag){
	std::vector<bool> chanSelect;
	chanSelect.resize(MAX_CHANNELS, false);
	int stopIndex, temp, through;
	int percentVal=0;
	int startIndex=0;
	
	
	//no command entered
	if (line.converted.size()<1){
		return -1;
	}
	
	if(flag == STANDARD){
	
		//start loop
		while (startIndex < line.converted.size()){
	
			if (isdigit(line.converted[startIndex])){
		
				//gets the number of the first or only channel
				temp = get_int(line.converted, startIndex, stopIndex, line.converted.size());
				if ((temp<=MAX_CHANNELS) && ((temp-1) >= 0)){
					chanSelect[temp-1]=true;
				}
				else {
					//channel out of range
					return -1;
				}
		
				//switch determines what to do with the next piece of the command
				switch (line.converted[stopIndex]) {
			
					case 'A':
						startIndex=stopIndex+1;
			
						if (isdigit(line.converted[startIndex])){
							percentVal = get_int(line.converted, startIndex, stopIndex, line.converted.size());
						}
						else {
							return -1;
						}
				
				
				
						//sets all previously inputted channels to inputted percent
						//						stop != start may not be necessary due to
						//							isdigit(line.converted[stopIndex+1]) above
						if ((percentVal<=100) && (stopIndex > startIndex)){
			
							for (int i=0; i<MAX_CHANNELS; i++){
						
								if (chanSelect[i]){
									percents[i]=percentVal;
									chanSelect[i]=false;
								}
							}
							startIndex=stopIndex;
						}	
						else {
							//percentage invalid
							return -1;
						}
						break;
			
					case 'F':
						percentVal = 100;
						//sets all activated channels to Full
						for (int i=0; i<MAX_CHANNELS; i++){
						
							if (chanSelect[i]){
								percents[i]=percentVal;
								chanSelect[i]=false;
							}
						}
				
						startIndex=stopIndex+1;
						break;
				
			
					case ',':
						startIndex=stopIndex+1;
						break;
			
			
					case '-':
						startIndex=stopIndex+1;
				
						through = get_int(line.converted, startIndex, stopIndex, line.converted.size());
				
						if (startIndex < stopIndex){
					
							if ((through <= MAX_CHANNELS) && (through-1 >= 0)) {
						
								if (through >= temp){
							
									for (int a=temp-1; a<through; a++){
								
										chanSelect[a]=true;
									}
							
								}
								else {
									//invalid range
									return -1;
								}
							}
							else {
								//channel out of range
								return -1;
							}
						}
						else {
							//end of range invalid
							return -1;
						}
				
				
						//startIndex=stopIndex;
						break;
			
					default:
						//invalid character
						return -1;
				}
		
		
		
		
			}
			else if(line.converted[startIndex] == ','){
				startIndex++;
			}
			else {
				return -1;
			}
	
		//end of loop
		}
		
	}
	else if(flag == NAME_CUE || flag == LOAD_CUE){
		temp = get_int(line.converted, 0, stopIndex, line.converted.size());
		if (stopIndex != 0){
			return temp;
		}
		else {
			//number was invalid
			return -1;
		}
	}
	else if(flag == EXIT){
		return 1;
	}
	
	
	return 0;
}



//converts an integer found in a c string to an actual integer
//also returns the index of where the integer stopped
int get_int(std::vector<char> cStr, int start, int &stop, int size){
	
	int i, temp, sum;
	std::vector<int> digits;
	
	//counts and stores the digits in the string
	//until it encounters a non-digit
	for (i=start; i<size; i++){
		if (isdigit(cStr[i])){
			temp = (int) cStr[i];
			digits.push_back( temp-48 );
		}
		else {
			
			break;
		}
	}
	stop = i;
	
	sum=0;
	temp = digits.size();
	
	//adds up the digits after shifting
	//them by the proper power of ten
	int mult = 1;
	for (i=temp-1; i>=0; i--){
		
		sum += digits[i]*mult;
		mult *= 10;
	}
	
	return sum;
}
