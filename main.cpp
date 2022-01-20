/* 01/19/2022 Ian Holloway */

#include "MaxChannels.h"

#include "lightStuff.h"
#include <vector>
#include <iostream>
#include <fstream>

#include <curses.h>
#include "Screen.h"

#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "key_input.h"

#define JOY_DEV "/dev/input/js0"




void sum_percents(int chans[], std::vector<Group> cues, int sum[]);

void clearChannels(int chans[]);

int save_cuelist(std::vector<Group> list);

int load_cuelist(std::vector<Group> &list);


int main() {

/*	
	int joy_fd;
	struct js_event js;
	if( ( joy_fd = open( JOY_DEV , O_RDONLY | O_NONBLOCK )) == -1 )
	{
		cout << "Couldn't open Joystick\n";
		return -1;
	}
*/
	
	char *message;
	char msg1[] = "YOU'RE DOING GREAT!";
	message = &msg1[0];
	
	char prompt2[] = "Cue Number: ";
	char prompt3[] = "Are you sure you want to exit?";
	char prompt4[] = "Load which cue?";
	
	
	int eos = initialize_screen(2);
	
	timeout(100);
	
	int ch = 0;
	int i=0;
	while ( ch != 103 ){
		ch = getch();
		printw("%d",ch);
		sleep(1);
	}
	
//	close(joy_fd);
	endwin();
	return 0;
}




void sum_percents(int chans[], std::vector<Group> cues, int sum[]){
	
	for(size_t i=0; i<MAX_CHANNELS; i++){
		sum[i]=chans[i];
	}
	
	
	for (size_t i=0; i<cues.size(); i++){
		for (size_t j=0; j<MAX_CHANNELS; j++){
			
			int temp = (int) ((cues[i].masterVal*cues[i].channelMax[j])/100);
			if (temp>sum[j]) sum[j]=temp;
		}
	}
	
}



//sets all channel values to zero
void clearChannels(int chans[]){
	for (size_t i=0; i<MAX_CHANNELS; i++) chans[i]=0;
}




int save_cuelist(std::vector<Group> list){
	
	int size=sizeof(Group);
	int numberOfGroups=list.size();
	
	std::ofstream out("Save", std::ofstream::binary);
	
	out.write((char *)&numberOfGroups, sizeof(int));
	out.write((char *)&size, sizeof(int));
	
	
	for (size_t i=0; i<numberOfGroups; i++){
		out.write((char *)&list[i], size);
	}
	

	out.close();
	
	return 0;
}




int load_cuelist(std::vector<Group> &list) {
	
	int size;
	int numberOfGroups;
	
	Group container;
	
	std::ifstream in("Save", std::ifstream::binary);
	
	if (!in.is_open()){
		//could not open file
		return -1;
	}
	
	in.read((char *)&numberOfGroups, sizeof(int));
	in.read((char *)&size, sizeof(int));
	
	if (size != sizeof(container)){
		// Different number of channels probably
		return -1;
	}
	
	
	for (size_t i=0; i<numberOfGroups; i++){
		in.read((char *)&container, size);
		list.push_back(container);
	}
	
	
	in.close();
	
	return list.size();
}
