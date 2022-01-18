
#ifndef KEY_INPUT
#define KEY_INPUT

#include <linux/input.h>
#include <vector>
#include "lightStuff.h"
#include "MaxChannels.h"

#define STANDARD 1
#define NAME_CUE 2
#define EXIT 3
#define LOAD_CUE 4

class Command {
	char convert(__u16);
	
	public:
	std::vector<__u16> raw;
	std::vector<char> converted;
	
	void add(__u16);
	void backspace(void);
	void clear(void);
};



int interpret_command (Command line, std::vector<Group> cueList, int percents[], int flag);

int get_int(std::vector<char> cStr, int start, int &stop, int size);


#endif