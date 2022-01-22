
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>
#include <string>

class CommandLine {
	private:
		std::vector<char> line;
		int cursorPos;
	public:
		CommandLine();
		int keyPressed(int k);
		void clear();
		
		std::string getString();
};


#endif