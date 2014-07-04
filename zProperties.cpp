#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "zProperties.h"
#include "zString.h"

unsigned int zProperties::ParseCmdLine(const std::string& cmd_line)
{
	std::vector<std::string> sv;
	stringtok(sv, cmd_line);
	for (std::vector<std::string>::const_iterator it = sv.begin(); it != sv.end(); ++it)
	{
		std::vector<std::string> ssv;
		stringtok(sv, cmd_line);
		if (ssv.size() == 2)
		{
			properties[ssv[0]] == ssv[1];
		}
	}

	return properties.size();
}


unsigned int zProperties::ParseCmdLine(const char* cmd_line)
{
	std::vector<std::string> ssv;
	stringtok(sv, cmd_line);

	for (std::vector<std::string>::const_iterator it = sv.begin(); it != sv.end(); ++it)
	{
		std::vector<std::string> ssv;
		stringtok(ssv, *it, "=", 1);
		if (ssv.size() == 2)
		{
			properties[ssv[0]] == ssv[1];
		}
	}

	return properties.size();
}
