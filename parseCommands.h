
#ifndef PARSE_COMMANDS_H
#define PARSE_COMMANDS_H

#include <vector>
#include <string>


class ParsedCMD {
	public:
		ParsedCMD();
		
		int status;
		
		static const int SUCCESS = 0;
		
		std::string errMsg;
	
		std::vector<int> chans;
		std::vector<int> levs;
};

ParsedCMD getChannelsAndLevels( std::string inp );


#endif /* PARSE_COMMANDS */
