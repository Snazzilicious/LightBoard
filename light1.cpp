/* 6/8/2015 */

//still need get input function probably for groups
//and individual channels separately
//
//also a way to add groups and channels to the group
//
//
#include "MaxChannels.h"

#include "dmx.h"
#include "Group.h"
#include <vector>
#include <iostream>
#include <fstream>

#include <curses.h>
#include "Screen.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#include <linux/input.h>

#include "key_input.h"

#define JOY_DEV "/dev/input/js0"
#define KEY_DEV "/dev/input/event18"



using namespace std;




void sum_percents(int chans[], vector<Group> cues, int sum[]);

void send_packet(int percents[]);

void init_dmx(int);

void clearChannels(int chans[]);

int save_cuelist(vector<Group> list);

int load_cuelist(vector<Group> &list);


int main() {
	int i, dummy, eos, flag, temp;
	bool completeCommand = false;
	bool endProgram = false;
	flag = STANDARD;
	
	double percentage;
	
	int buttonPressed=-1;

	int key_fd;
	struct input_event key;
	
	int joy_fd;
	struct js_event js;
	
	
	if( ( key_fd = open( KEY_DEV , O_RDONLY)) == -1 )
	{
		cout << "Couldn't open Keyboard\n";
		return -1;
	}
	//opening in non-blocking mode so the main
	//program can keep spinning
	fcntl( key_fd, F_SETFL, O_NONBLOCK );
	int rcode = ioctl( key_fd, EVIOCGRAB, 1 );
	
	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		cout << "Couldn't open Joystick\n";
		return -1;
	}
	fcntl( joy_fd, F_SETFL, O_NONBLOCK );
	
	int numOfAxis=2, numOfButtons=2;
	
	//ioctl( joy_fd, JSIOCGAXES, &numOfAxis );
	//ioctl( joy_fd, JSIOCGBUTTONS, &numOfButtons );
	
	vector<Group*> cuesOnFaders;
	cuesOnFaders.resize(numOfAxis, NULL);
	Group temporaryGroup;
	
	int nameOfCues[2] = {-1};
	fill_n(nameOfCues, 2, -1);
	
	int faderPercents[2] = {0};
	
	//list of the valus of the channels
	//as unsigned chars/ubytes
	//unsigned char chanVals[24] ={0};
	int chanPercents[MAX_CHANNELS] = {0};
	clearChannels(chanPercents);
	
	//list of all the cues
	vector<Group> cues;
	
	//sum of cue output and and channel output
	int currentSum[MAX_CHANNELS] = {0};
	clearChannels(currentSum);
	
	Command Input;
	
	
	char *message;
	char msg1[] = "YOU'RE DOING GREAT!";
	message = &msg1[0];
	
	char prompt2[] = "Cue Number: ";
	char prompt3[] = "Are you sure you want to exit?";
	char prompt4[] = "Load which cue?";
	
	
	
	
	init_dmx(MAX_CHANNELS);
	eos = initialize_screen(numOfAxis);
	dummy = load_cuelist(cues);
	//cout <<1<<endl;
	/* WHILE BEGINS HERE */
	while(!endProgram){
	
		//get input from keyboard and joystick
		
		while (read(key_fd, &key, sizeof(struct input_event)) > 0){
			
			//store keyboard in a vector once per press of key
			if (key.value == 1 && (key.code != KEY_RESERVED)){
				//cout << 2 << endl;
				if (key.code == KEY_BACKSPACE) {
					Input.backspace();
				}
				else if(key.code == KEY_ENTER){
					completeCommand = true;
					break;
				}
				else if(key.code == KEY_F1){
					Input.clear();
					clearChannels(chanPercents);
					flag = STANDARD;
				}
				else if(key.code == KEY_F2){
					
					
					flag = NAME_CUE;
					message = &prompt2[0];
					//and then prompt for name
					
				}
				else if(key.code == KEY_F3){
					flag = EXIT;
					message = &prompt3[0];
				}
				else if(key.code == KEY_ESC){
					flag = STANDARD;
					message = &msg1[0];
				}
				else {
					Input.add(key.code);
				}
			}
			
		}
		
		
		
		//reading Joystick's input buffer
		while (read(joy_fd, &js, sizeof(struct js_event)) > 0){
			
			if ((js.type == JS_EVENT_BUTTON) && (js.value != KEY_RESERVED)){
				temp = (int) js.number;
				//cout << 3 << endl;
				//corresponds the number of the button pressed
				//to its index in the cue pointer vector
				if (temp == 7){
					buttonPressed = 0;
				}
				else if (temp == 9){
					
					buttonPressed = 1;
				}
				
				if (temp == 7 || temp == 9){
					flag = LOAD_CUE;
					message = &prompt4[0];
				
				}
				
			}
			else if (js.type == JS_EVENT_AXIS){
				//cout << 4 << endl;
				//correspond number to index in pointer vector
				temp = (int) js.number;
				
				if (temp == 2){
					dummy = 0;
				}
				else if (temp == 4){
					
					dummy = 1;
				}
				
				if (temp == 2 || temp == 4){
					//set value of the pointed to cue to the fader percentage
					//cout << "?" << endl;
					percentage = (-js.value + 32767)/655.34;
					faderPercents[dummy] = percentage;
					
					if (cuesOnFaders[dummy] != NULL){
						temporaryGroup = *cuesOnFaders[dummy];
						temporaryGroup.set_val(percentage);
				
						*cuesOnFaders[dummy] = temporaryGroup;
					
					}
				
				}
			}
			
			
		}
		
		
		
		//interpret command if done
		//then clears the command and 
		//prepares for more
		if (completeCommand){
			dummy = interpret_command(Input, cues, chanPercents, flag);
			//cout << 5;
			if (flag == STANDARD){
				message=&msg1[0];
				flag = STANDARD;
			}
			else if ((flag == NAME_CUE) && (dummy >= 0)){
			
				sum_percents(chanPercents, cues, currentSum);
			
				//see if cue already exists
				for (int d=0; d<cues.size(); d++){
					//if it does delete old cue
					if (cues[d].name == dummy){
						cues.erase(cues.begin()+d);
	//					break;
					}
				}
				
					
					
				//then the following
			
				
					
				cues.resize(cues.size()+1);
				temp = cues.size()-1;
				cues[temp].name = dummy;
				for (int d=0; d<MAX_CHANNELS; d++){
						
					if(currentSum[d]){
							
						cues[temp].add_channel(d+1, currentSum[d]);
							
					}
						
				}
				
				//re-points to the cues assigned to the faders
				//when a vector is resized the location of the
				//elements in memory changes
				for (int i=0; i<numOfAxis; i++){
					
					for (int j=0; j<cues.size(); j++){
					
						if (nameOfCues[i] == cues[j].name){
						
							cuesOnFaders[i] = &cues[j];
						}
					
					}
				}
				
				
				message=&msg1[0];
				flag = STANDARD;
			}
			else if (flag == EXIT){
				endProgram=true;
			}
			else if((flag == LOAD_CUE) && (dummy >= 0)){
				
				
				
				for(temp=0; temp<cues.size(); temp++){
					
					if(cues[temp].name == dummy){
						
						if (cuesOnFaders[buttonPressed] != NULL){
							temporaryGroup = *cuesOnFaders[buttonPressed];
							temporaryGroup.set_val(0.0);
							*cuesOnFaders[buttonPressed] = temporaryGroup;
						}
						
						cuesOnFaders[buttonPressed] = &cues[temp];
						nameOfCues[buttonPressed] = cues[temp].name;
						
						break;
					
						
					}
					
				}
				
				buttonPressed = -1;
				message=&msg1[0];
				flag = STANDARD;
			}
			
			Input.clear();
			completeCommand=false;
			
		}
		
		
		sum_percents(chanPercents, cues, currentSum);
		
		//updates the screen generated by ncurses
		print_screen(cuesOnFaders, currentSum, message, Input, eos, faderPercents, chanPercents, buttonPressed);
		
		send_packet(currentSum);
		//currentSum = chanPercents
				
		
	//end of while
	}
	
	rcode = ioctl( key_fd, EVIOCGRAB, 0 );
	close(key_fd);
	close(joy_fd);
	dummy = save_cuelist(cues);
	endwin();
	dmxClose();
	return 0;
}







//this will start up the connection to the
//dmx deamon btw write a script to start it
void init_dmx(int numOfChans){
	int error;
	
	error = dmxOpen();
	dmxSetMaxChannels(numOfChans);
	
}




void sum_percents(int chans[], vector<Group> cues, int sum[]){
	
	int i, j, temp;
	
	for(i=0; i<MAX_CHANNELS; i++){
		sum[i]=chans[i];
	}
	
	
	for (i=0; i<cues.size(); i++){
		
		for (j=0; j<MAX_CHANNELS; j++){
			
			temp = (int) ((cues[i].masterVal*cues[i].channelMax[j])/100);
			if (temp>sum[j]){
				sum[j]=temp;
			}
			
		}
		
	}
	
}






//writes to memory
//
//channel addresses are 1 indexed
//citing the README file in DMXWheel/dmx
void send_packet(int percents[]){
	
	unsigned char value;
	

	long int last;
	long int recent;
	
	static struct timeval prev;
	struct timeval now;
	

	gettimeofday(&now, NULL);
	
	last = 1000000*prev.tv_sec + prev.tv_usec;
	recent = 1000000*now.tv_sec + now.tv_usec;
	
	

	if ((recent - last) > 5000){
		prev = now;
	
		for (unsigned char a=0; a<MAX_CHANNELS; a++){
		
			value = (unsigned char) ((255 * percents[a])/100);
		
//			dmxSetValue(chan, value);
			dmxSetValue(a, value);
		}
		
	}
}



//sets all channel values to zero
void clearChannels(int chans[]){
	for (int i=0; i<MAX_CHANNELS; i++){
		
		chans[i]=0;
		
	}
	
}




int save_cuelist(vector<Group> list){
	
	int size=sizeof(Group);
	int numberOfGroups=list.size();
	
	ofstream out("Save", ofstream::binary);
	
	out.write((char *)&numberOfGroups, sizeof(int));
	out.write((char *)&size, sizeof(int));
	
	
	for (int i=0; i<numberOfGroups; i++){
		out.write((char *)&list[i], size);
	
	}
	

	out.close();
	
	return 0;
}




int load_cuelist(vector<Group> &list) {
	
	int size;
	int numberOfGroups;
	
	Group container;
	
	ifstream in("Save", ifstream::binary);
	
	if (!in.is_open()){
		//could not open file
		return -1;
	}
	
	in.read((char *)&numberOfGroups, sizeof(int));
	in.read((char *)&size, sizeof(int));
	
	
	
	for (int i=0; i<numberOfGroups; i++){
		in.read((char *)&container, size);
		list.resize(list.size()+1, container);
	}
	
	
	in.close();
	
	return list.size();
}
