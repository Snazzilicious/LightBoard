
#ifndef GROUP_H
#define GROUP_H

#include "MaxChannels.h"

//cues from old board
class Group {
	private:
		int channelMax[MAX_CHANNELS];
//		unsigned char chanVals[MAX_CHANNELS];
		
		int name;
		
		int masterVal;
	
	public:
		Group();
		Group(int ID, int levels[]);
		
		void add_channel(int, int);
		
		void set_val(int);
		
		int getName();
		int getPercent(int chan);
};


#endif
