
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>
#include <list>

#define MAX_HIST_LEN 10

class CommandLine {
	private:
		int cursorPos;
		std::vector<char> line;
		
		std::list<std::vector<char>>::iterator curHist;
		std::list<std::vector<char>> hist;
	public:
		CommandLine();
		int keyPressed(int k);
		void clear();
		
		std::vector<char>::iterator lineBegin();
		std::vector<char>::iterator lineEnd();
		
		int getCursorPos();
};


#endif
