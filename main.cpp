/* 01/19/2022 Ian Holloway */

#include "MaxChannels.h"

#include "Group.h"
#include <unordered_map>
#include <iostream>
#include <fstream>

#include <curses.h>
#include "Screen.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "CommandLine.h"
#include "parseCommands.h"

#define JOY_DEV "/dev/input/js0"


#define ENTER_CHANNELS 0
#define SAVE_CUE 1
#define LOAD_CUE 2
#define EXITING 3


void sum_percents(int chans[], int numFaders, Group** cues, int sum[]);

void clearChannels(int chans[]);

int save_cuelist(const std::unordered_map<int,Group>& list);

int load_cuelist(std::unordered_map<int,Group>& list);

int setChannelsAndLevels( ParsedCMD parsed, int chanLevs[] );

void saveScene( int name, std::unordered_map<int,Group>& cues, int levels[] );


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
	char msg1[] = "YOU'RE DOING GREAT!";
	char prompt2[] = "Cue Number: ";
	char prompt3[] = "Are you sure you want to exit? [y]/n";
	char prompt4[] = "Load which cue?";
	char *message = &msg1[0];
	
	
	// Containers for channel percents and scenes and fader assignents
	std::unordered_map<int,Group> cues;
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
	
	// Initialize command line
	CommandLine cmd;
	
	// For getting input chans and levels
	ParsedCMD parsed;
	
	timeout(100);
	int ch = 0;
	int stat = CommandLine::NONE;
	int mode = ENTER_CHANNELS;
	bool running = true;
	int info=0;
	
	while ( running ){
		ch = getch();
		if( ch == 103 ) running = false;
//		printw("%d",ch);
//		printw("%c",cmd.keyPressed(ch));
//		sleep(1);
		stat = cmd.keyPressed(ch);
		
		switch( mode ){
			case SAVE_CUE :
				switch( stat ){
					case CommandLine::ENTER_PRESSED :
						
						// Get the cue number
						info = getSingleInt( cmd.getLastCmd() );
						if( info >= 0 ) saveScene( info, cues, chanPerc );
						
						// go to ENTER_CHANNELS
					case CommandLine::F2_PRESSED :
					case CommandLine::F4_PRESSED :
						
						mode = ENTER_CHANNELS;
						message = &msg1[0];
						break;
						
					default :
						// do nothing
						break;
				}
			
			case LOAD_CUE :
			case EXITING :
				
				if( ch == 121 /* Y */ || stat == CommandLine::ENTER_PRESSED ){
					running = false;
				} else if( ch == 110 /* N */ || stat == CommandLine::F2_PRESSED || stat == CommandLine::F4_PRESSED ){
					mode = ENTER_CHANNELS;
					message = &msg1[0];
				}
				break;
				
			case ENTER_CHANNELS :
				switch( stat ){
					case CommandLine::ENTER_PRESSED :
						
						// Parse the command and set any channel levels
						parsed = getChannelsAndLevels( cmd.getLastCmd() );
						info = setChannelsAndLevels( parsed, chanInp );
						
						sum_percents( chanInp, numFaders, cueOnFader, chanPerc );
						break;
						
					case CommandLine::F2_PRESSED :
						
						clearChannels( chanInp );
						sum_percents( chanInp, numFaders, cueOnFader, chanPerc );
						break;
						
					case CommandLine::F3_PRESSED :
						
						mode = SAVE_CUE;
						message = &prompt4[0];
						break;
						
					case CommandLine::F4_PRESSED :
						
						mode = EXITING;
						message = &prompt3[0];
						break;
						
					default :
						// do nothing
						break;
				}
			default :
				// An error has occured - should exit loop
				break;
		}
		scr.update(chanPerc, chanInp, cueOnFader, loadFader, faderPerc, message, cmd);
	}
	
	
	delete[] cueOnFader;
	delete[] faderPerc;
//	close(joy_fd);
	endwin();
	return 0;
}





void sum_percents(int chans[], int numFaders, Group** cues, int sum[]){
	for(size_t i=0; i<MAX_CHANNELS; i++)
		sum[i]=chans[i];
	
	for (size_t i=0; i<numFaders; i++){
		if( cues[i] == nullptr ) continue;
		
		for (size_t j=0; j<MAX_CHANNELS; j++){
			int temp = cues[i]->getPercent(j);
			if (temp>sum[j]) sum[j]=temp;
		}
	}
	
}



//sets all channel values to zero
void clearChannels(int chans[]){
	for (size_t i=0; i<MAX_CHANNELS; i++) chans[i]=0;
}




int save_cuelist(const std::unordered_map<int,Group> &list){
	
	int size=sizeof(Group);
	int numberOfGroups=list.size();
	
	std::ofstream out("Save", std::ofstream::binary);
	
	out.write((char *)&numberOfGroups, sizeof(int));
	out.write((char *)&size, sizeof(int));
		
	
	for ( auto it : list ) out.write((char *)&it.second, size);

	
	out.close();
	
	return 0;
}

int load_cuelist(std::unordered_map<int,Group> &list) {
	
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
		list.insert( std::pair<int,Group>(container.getName(), container) );
	}
	
	
	in.close();
	
	return list.size();
}



int setChannelsAndLevels( ParsedCMD parsed, int chanLevs[] ){
	for( size_t i=0; i<parsed.chans.size(); i++ ){
		int c = parsed.chans[i]-1;
		int l = parsed.levs[i];
		
		if( c>=0 && c<MAX_CHANNELS && l>=0 && l<=100 )
			chanLevs[ c ] = l;
	}
	return 0;
}


void saveScene( int name, std::unordered_map<int,Group>& cues, int levels[] ){
	// Check if scene exists already
	std::unordered_map<int,Group>::iterator it = cues.find( name );
	
	if( it == cues.end() ){
		cues.insert( std::pair<int,Group>( name, Group(name, levels) ) );
	} else {
		it->second.overwriteChanMax( levels );
	}
}








