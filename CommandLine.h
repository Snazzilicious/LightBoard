
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>

class CommandLine {
	private:
		std::vector<char> line;
		int cursorPos;
	public:
		CommandLine();
		int keyPressed(int k);
		void clear();
};


#endif
