

#include "Group.h"

using namespace std;


//constructor
Group::Group(){
	int i;
	masterVal=0;
	for (i=0; i<MAX_CHANNELS; i++){
		activeChannels[i]=false;
		channelMax[i]=0;
		chanVals[i]=0;
	}
	
	
}

//adds a channel duh
void Group::add_channel(int chan, int maxPercent){
		activeChannels[chan-1] = true;
		channelMax[chan-1] = maxPercent;
}


//casting from a percent to a ubyte may
//not be necessary depending on how
//input comes in
void Group::set_val(double percent){
	//checks for overflow or underflow
	if(percent > 100.0){
		percent = 100.0;
	}
	else if(percent < 0.0){
		percent = 0.0;
	}
	
	masterVal=percent;
	
	for (int i=0; i<MAX_CHANNELS; i++){
		if(activeChannels[i]){
			chanVals[i] = (unsigned char) ((255*channelMax[i]*masterVal)/10000);
		}
		
	}
	
}

