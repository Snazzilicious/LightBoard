
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
		
		static const int NONE=0;
		static const int ENTER_PRESSED=1;
		static const int CLEAR_PRESSED=2;
		static const int F1_PRESSED=3;
		static const int F2_PRESSED=4;
		static const int F3_PRESSED=5;
};


#endif
