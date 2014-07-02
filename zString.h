#ifndef _zString_H_
#define _zString_H_

#include <string>

namespace
{
	inline std::string& string_replace(std::string& str, const std::string& str_src, std::string& str_dst)
	{
		std::string::size_type pos(0);
		std::string::size_type src_len = str_src.size();
		std::string::size_type dst_len = str_dst.size();

		while ((pos = str.find(str_src, pos)) != std::string::npos)
		{
			str.replace(pos, src_len, str_dst);
			pos += dst_len;
		}
	}

	return str;
}

#endif
