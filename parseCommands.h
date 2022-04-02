
#ifndef PARSE_COMMANDS_H
#define PARSE_COMMANDS_H

#include <vector>
#include <string>

class pair {
	public:
		pair(int c, int l);
		
		int chan;
		int level;
};

std::vector<pair> getChannelsAndLevels( std::string inp );


#endif /* PARSE_COMMANDS */
