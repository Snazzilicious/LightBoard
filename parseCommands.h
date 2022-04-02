
#ifndef PARSE_COMMANDS_H
#define PARSE_COMMANDS_H

#include <vector>
#include <string>

#define SUCCESS 0
#define FAILURE -1

class ParsedCMD {
	public:
		ParsedCMD();
		
		int status;
		
		std::string errMsg;
	
		std::vector<int> chans;
		std::vector<int> levs;
};

ParsedCMD getChannelsAndLevels( std::string inp );

int getSingleInt( std::string inp );

#endif /* PARSE_COMMANDS */
