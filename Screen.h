
#ifndef SCREEN_H
#define SCREEN_H

#include "key_input.h"
#include "MaxChannels.h"
#include <vector>

class Screen{
    private:
	const int CHAN_START_X = 4;
	const int CHAN_START_Y = 2;
	
	const int H_SPAC = 4;
	const int V_SPAC = 3;
	
	int nRows;
	int nCols;
	
	int nFaders;
	
    public:
	Screen(int numFaders);
	
	void update(Group* cuesOnFaders[], int Percents[], int chanIn[]);
};


int initialize_screen(int);

void print_screen(std::vector<Group*>, int Percents[], char messageToUser[], Command commandLine, int &endline, int faderPercents[], int chanIn[], int Load);

#endif
