
#ifndef _LIGHT_STUFF
#define _LIGHT_STUFF

#include <vector>
#include "MaxChannels.h"

//using namespace std;

//cues from old board
class Group {
	
	
	public:
		bool activeChannels[MaxChannels];
		int channelMax[MaxChannels];
		unsigned char chanVals[MaxChannels];
		
		int name;
		
		int masterVal;
		
		Group();
		
		void add_channel(int, int);
		
		void set_val(double);
};


#endif