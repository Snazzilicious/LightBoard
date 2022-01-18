
#ifndef _SCREEN_H
#define _SCREEN_H

#include <curses.h>
#include "key_input.h"
#include "MaxChannels.h"
#include <vector>

int initialize_screen(int);

void print_screen(std::vector<Group*>, int Percents[], char messageToUser[], Command commandLine, int &endline, int faderPercents[], int chanIn[], int Load);

#endif