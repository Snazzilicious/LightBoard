
#ifndef GROUP_H
#define GROUP_H

#include "MaxChannels.h"

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
