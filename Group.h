
#ifndef _LIGHT_STUFF
#define _LIGHT_STUFF

#include <vector>
#include "MaxChannels.h"

//using namespace std;

//cues from old board
class Group {
	
	
	public:
		bool activeChannels[MAX_CHANNELS];
		int channelMax[MAX_CHANNELS];
		unsigned char chanVals[MAX_CHANNELS];
		
		int name;
		
		int masterVal;
		
		Group();
		
		void add_channel(int, int);
		
		void set_val(double);
};


#endif
