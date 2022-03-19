
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>
#include <list>
#include <string>

#define MAX_HIST_LEN 10

class CommandLine {
	private:
		int cursorPos;
		std::list<std::vector<char>>::iterator line;
		
		std::list<std::vector<char>> hist;
		std::list<std::vector<char>> wrkSpace;
		
		void clear();
		void resetWrkSpace();

		std::string lastCmd;
	public:
		CommandLine();
		int keyPressed(int k);
		
		std::vector<char>::iterator lineBegin();
		std::vector<char>::iterator lineEnd();
		
		int getCursorPos();

		std::string getLastCmd();
};


#endif
