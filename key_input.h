
#ifndef KEY_INPUT_H
#define KEY_INPUT_H

#include <vector>
#include "Group.h"

#define STANDARD 1
#define NAME_CUE 2
#define EXIT 3
#define LOAD_CUE 4

class Command {
	char convert(int);
	
	public:
	std::vector<char> converted;
	
	void add(int);
	void backspace(void);
	void clear(void);
};



int interpret_command(Command line, std::vector<Group> cueList, int percents[], int flag);

int get_int(std::vector<char> cStr, int start, int &stop, int size);


#endif
