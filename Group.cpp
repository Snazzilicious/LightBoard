

#include "Group.h"

//constructor(s)
Group::Group(){} // this is just to allow deserialization

Group::Group(int ID, int levels[]){
	name = ID;
	masterVal = 0;
	
	for (int i=0; i<MAX_CHANNELS; i++) channelMax[i]=levels[i];
}

//adds a channel
void Group::add_channel(int chan, int maxPercent){
		channelMax[chan-1] = maxPercent;
}


// sets master level
void Group::set_val(int percent){
	//checks for overflow or underflow
	if(percent > 100){
		percent = 100;
	}
	else if(percent < 0){
		percent = 0;
	}
	
	masterVal=percent;
}


int Group::getName(){
	return name;
}


int Group::getPercent(int chan){
	return (channelMax[chan]*masterVal)/100;
}

