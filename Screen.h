
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
	
	void update(int Percents[], int chanIn[], Group* cuesOnFaders[], int faderLoad, int faderPercents[], char* msg);
};


#endif
