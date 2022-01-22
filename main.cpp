/* 01/19/2022 Ian Holloway */

#include "MaxChannels.h"

#include "Group.h"
#include <vector>
#include <iostream>
#include <fstream>

#include <curses.h>
#include "Screen.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "CommandLine.h"

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
	}
*/
	int numFaders = 2;
	
	// Messages to send to the user
	char *message;
	char msg1[] = "YOU'RE DOING GREAT!";
	message = &msg1[0];
	
	char prompt2[] = "Cue Number: ";
	char prompt3[] = "Are you sure you want to exit?";
	char prompt4[] = "Load which cue?";
	
	// Containers for channel percents and scenes and fader assignents
	std::vector<Group> cues;
	Group** cueOnFader = new Group*[numFaders];
	int* faderPerc = new int[numFaders];
	for (int i=0; i<numFaders; i++){
		cueOnFader[i] = nullptr;
		faderPerc[i] = 0;
	}
	int chanInp[MAX_CHANNELS] = {0};
	int chanPerc[MAX_CHANNELS] = {0};
	int loadFader = -1;
	
	
	
	// Initialize display
	Screen scr(numFaders);
	scr.update(chanPerc, chanInp, cueOnFader, loadFader, faderPerc, message);
	
	timeout(100);
	printw("%d",KEY_A);
	int ch = 0;
	while ( ch != 103 ){
		ch = getch();
		printw("%d",ch);
		sleep(1);
	}
	
	delete[] cueOnFader;
	delete[] faderPerc;
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
			
			int temp = cues[i].getPercent(j);
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
	
	std::ifstream in("Save", std::ifstream::binary);
	if (!in.is_open()) return -1; //could not open file
	
	
	int size;
	int numberOfGroups;
	Group container;
	
	
	in.read((char *)&numberOfGroups, sizeof(int));
	in.read((char *)&size, sizeof(int));
	
	if (size != sizeof(container)) return -1; // Different number of channels probably
	
	
	for (size_t i=0; i<numberOfGroups; i++){
		in.read((char *)&container, size);
		list.push_back(container);
	}
	
	
	in.close();
	
	return list.size();
}
