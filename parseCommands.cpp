
#include "parseCommands.h"


ParsedCMD::ParsedCMD(){
	status = SUCCESS;
	
	errMsg = "Success";
}

#include<regex>



std::regex patt_at("([0-9]+[0-9,\-]+)A([0-9]{1,3})");
std::regex patt_full("([0-9]+[0-9,\-]+)F");
std::regex patt_single("[0-9]+");
std::regex patt_range("([0-9]+)-([0-9]+)");

ParsedCMD getChannelsAndLevels( std::string inp ){
	std::smatch matches;
	ParsedCMD ret;
	
	bool foundMatch = true;
	while(foundMatch){
		foundMatch = false;

		int newLevel;
		std::string chanstr;

		// Get list of channels to set to a percent of full
		if (std::regex_search( inp, matches, patt_at )){

			std::stringstream(matches[2]) >> newLevel;

			chanstr = matches[1];

			inp = matches.suffix();
			foundMatch = true;

		} else if (std::regex_search( inp, matches, patt_full )){

			newLevel = 100;

			chanstr = matches[1];

			inp = matches.suffix();
			foundMatch = true;
		}

		// Get channels from chanstr
		if (foundMatch){
			std::stringstream chanstrm(chanstr);
			while(chanstrm.good()){
				
				std::string buf;
				std::getline(chanstrm, buf, ',');
				
				// need to check if single or range
				if (std::regex_search( buf, matches, patt_range )){

					int firstChan;
					std::stringstream(matches[1]) >> firstChan;
					int lastChan;
					std::stringstream(matches[2]) >> lastChan;
					
					for( int newChan=firstChan; newChan<=lastChan; newChan++){
						ret.chans.push_back( newChan );
						ret.levs.push_back( newLevel );
					}

				} else if (std::regex_search( buf, matches, patt_single )){
					
					int newChan;
					std::stringstream(buf) >> newChan;
					ret.chans.push_back( newChan );
					ret.levs.push_back( newLevel );
					
				}
				
			}
		}

	}
	
	return ret;
}


int getSingleInt( std::string inp ){
	std::smatch m;
	if (std::regex_search( inp, m, patt_single )){
		int val;
		std::stringstream(m[0]) >> val;
		return val;
	}
	return FAILURE;
}
